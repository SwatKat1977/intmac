/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite )
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2023 Integrated Test Management Suite Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/
#include <chrono>
#include <signal.h>
#include <sstream>
#include "Platform.h"
#if ITEMS_PLATFORM == ITEMS_PLATFORM_WINDOWS_CORE || \
    ITEMS_PLATFORM == ITEMS_PLATFORM_WINDOWS_MSVC
#  include <winsock2.h>
#endif
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "LoggerSettings.h"
#include "ServiceContext.h"

#define LOGGER_THREAD_SIZE 8192
#define LOGGER_THREAD_COUNT 1
#define LOGGER_NAME "logger"

#define LOGGER spdlog::get (LOGGER_NAME)

using namespace std::chrono_literals;

namespace items
{
    namespace serviceFramework
    {
        static ServiceContext *m_instance;

        static void StopSignalHandler (int signal)
        {
            LOGGER->info ("Service shutdown signal has been caught...");
            m_instance->NotifyShutdownRequested ();
        }

        ServiceContext::ServiceContext (std::string contextName) :
            m_contextName(contextName),
            m_initialised(false),
            m_initLayout(nullptr),
            m_shutdownRequested(false),
            m_usingIniConfig(false)
        {
            m_instance = this;

            // Create json object mapper.
            m_objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared ();
        }

        bool ServiceContext::Initialise (SectionsMap *initLayout,
                                         std::string iniFile)
        {
            m_initLayout = initLayout;

            if (!iniFile.empty ())
            {
                m_usingIniConfig = true;
                m_iniFile = iniFile;
            }

            if (!ReadIniConfiguration ())
            {
                return false;
            }

            if (!InitialiseLogger ())
            {
                return false;
            }

            oatpp::base::Environment::init ();

#if ITEMS_PLATFORM == ITEMS_PLATFORM_WINDOWS_CORE || \
    ITEMS_PLATFORM == ITEMS_PLATFORM_WINDOWS_MSVC
            int iResult;
            WSADATA wsaData;

            // Initialize Winsock
            iResult = WSAStartup (MAKEWORD (2, 2), &wsaData);
            if (iResult != 0) {
                printf ("Failed to initialise Winsock..\n");
                m_initialised = false;
            }
#endif

            if (m_modules.size() == 0)
            {
                return true;
            }

            for (auto init = m_modules.begin ();
                init != m_modules.end ();
                init++)
            {
                m_initialised = (*init)->Initialise ();

                if (!m_initialised)
                    break;
            }

            return m_initialised;
        }

        void ServiceContext::AddServiceModule (ServiceModule* newModule)
        {
            for (auto module = m_modules.begin ();
                module != m_modules.end ();
                module++)
            {
                if ((*module)->Name () == newModule->Name ())
                {
                    std::string err = "Duplicate service module '" +
                                      newModule->Name() + "'";
                    throw new std::invalid_argument(err);
                }
            }

            newModule->SetContext (this);

            m_modules.push_back (newModule);
        }

        void ServiceContext::AddServiceProvider (std::string name,
                                                 std::string address,
                                                 int networkPort,
                                                 ServiceNetworkType networkType)
        {
            for (auto provider = m_providers.begin ();
                provider != m_providers.end ();
                provider++)
            {
                if ((*provider).second.name == name &&
                    (*provider).second.address == address &&
                    (*provider).second.networkPort == networkPort &&
                    (*provider).second.networkType == networkType)
                {
                    std::string err = "Duplicate service provider!";
                    throw std::invalid_argument (err);
                }
            }

            auto addrType = networkType == SERVICENETWORKTYPE_IPV4 ?
                oatpp::network::Address::IP_4 : oatpp::network::Address::IP_6;
            auto provider = oatpp::network::tcp::server::ConnectionProvider::createShared (
                { address, v_uint16 (networkPort), addrType });

            auto router = oatpp::web::server::HttpRouter::createShared ();
            ServiceProviderEntry entry = {
                name,
                address,
                v_uint16 (networkPort),
                networkType,
                provider,
                router,
                oatpp::web::server::HttpConnectionHandler::createShared (router),
            };
            m_providers[name] = entry;
        }

        void ServiceContext::AddRoute (std::string providerName,
            HTTPRequestMethod method,
            std::string endpoint,
            ApiRoute* route)
        {
            ProvidersMap::iterator provider = m_providers.find (providerName);
            if (provider == m_providers.end ())
            {
                std::string errStr = std::string ("Unknown provider '") +
                                     providerName + "'";
                throw std::runtime_error (errStr);
            }

            auto routeEntry = std::make_shared<RouteHandler> (
                route, m_objectMapper);
            route->SetObjectMapper (m_objectMapper);
            m_routes[route->Name ()] = routeEntry;

            // Add route to the provider.
            (*provider).second.router->route (HttpRequestMethodToStr(method),
                                              endpoint,
                                              routeEntry);
        }

        void ServiceContext::Execute ()
        {
            // Windows does not handle SIGINT properly, so disable... see MSDN:
            // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/signal?view=msvc-170
#if ITEMS_PLATFORM == ITEMS_PLATFORM_LINUX
            LOGGER->info ("Setting up signal handler...");
            struct sigaction sigIntHandler;
            sigIntHandler.sa_handler = StopSignalHandler;
            sigemptyset (&sigIntHandler.sa_mask);
            sigIntHandler.sa_flags = 0;
            sigaction (SIGINT, &sigIntHandler, NULL);
#endif

            for (auto provider = m_providers.begin ();
                 provider != m_providers.end ();
                 provider++)
            {
                std::stringstream msg;
                msg << "Server started listening "
                    << (*provider).second.address << ":"
                    << (*provider).second.networkPort;
                LOGGER->info (msg.str());
                std::shared_ptr<oatpp::network::Server> server;
                server = std::make_shared<oatpp::network::Server> (
                    (*provider).second.provider,
                    (*provider).second.connectionHandler);

                std::thread *serviceThread = new std::thread ([&server]
                {
                    server->run ();
                });

                ServerThreadEntry serverThreadEntry = { server, serviceThread };
                m_servers[(*provider).first] = serverThreadEntry;
            }

            while (!m_shutdownRequested)
            {
                std::this_thread::sleep_for (1ms);

                for (auto module = m_modules.begin ();
                    module != m_modules.end ();
                    module++)
                {
                    (*module)->Execute ();
                }
            }

            for (auto module = m_modules.begin ();
                module != m_modules.end ();
                module++)
            {
                (*module)->Shutdown ();
            }

            for (auto provider = m_providers.begin ();
                provider != m_providers.end ();
                provider++)
            {
                // Stop the ServerConnectionProvider so we don't accept any new
                // connections.
                provider->second.provider->stop ();

                ServerThreadEntry entry = m_servers[provider->first];

                // Check to see if server is still running and stop if needed.
                if (entry.server->getStatus () == oatpp::network::Server::STATUS_RUNNING)
                {
                    entry.server->stop ();
                }

                // Finally, stop the ConnectionHandler and wait until all
                // running connections are closed.
                provider->second.connectionHandler->stop ();

                // Check if the server-thread has already stopped or if we need
                // to wait for the server to stop .
                if (entry.serverThread->joinable ())
                {
                    // Wait until the thread is done.
                    entry.serverThread->join ();
                }

                delete entry.serverThread;
            }

            oatpp::base::Environment::destroy ();
        }

        void ServiceContext::NotifyShutdownRequested ()
        {
            m_shutdownRequested = true;
        }

        bool ServiceContext::InitialiseLogger ()
        {
            try
            {
                GET_LOGGING_LOG_TO_CONSOLE;
                GET_LOGGING_LOG_FILENAME;
                GET_LOGGING_MAX_FILE_SIZE;
                GET_LOGGING_MAX_FILE_COUNT;
                GET_LOGGING_LOG_FORMAT;
            }
            catch (const std::invalid_argument& ex)
            {
                printf ("[FATAL ERROR] %s\n", ex.what ());
                return false;
            }

            spdlog::init_thread_pool (LOGGER_THREAD_SIZE, LOGGER_THREAD_COUNT);

            std::vector<spdlog::sink_ptr> sinks;

            if (GET_LOGGING_LOG_TO_CONSOLE == LOGGING_LOG_TO_CONSOLE_YES)
            {
                auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt > ();
                sinks.push_back (stdout_sink);
            }

            if (!GET_LOGGING_LOG_FILENAME.empty ())
            {
                if (GET_LOGGING_MAX_FILE_SIZE)
                {
                    try
                    {
                        auto sink = std::make_shared<
                            spdlog::sinks::rotating_file_sink_mt> (
                                GET_LOGGING_LOG_FILENAME,
                                GET_LOGGING_MAX_FILE_SIZE,
                                GET_LOGGING_MAX_FILE_COUNT);
                        sink->set_level (spdlog::level::debug);
                        sinks.push_back (sink);
                    }
                    catch (const spdlog::spdlog_ex& ex)
                    {
                        std::string errStr =
                            std::string ("Unable to initialise rotating file log sink, reason: ") +
                            std::string (ex.what ());
                        throw std::runtime_error (errStr);
                    }
                }
                else
                {
                    try
                    {
                        const bool truncate = true;
                        auto sink = std::make_shared<
                            spdlog::sinks::basic_file_sink_mt> (
                                GET_LOGGING_LOG_FILENAME.c_str (),
                                truncate);
                        sink->set_level (spdlog::level::debug);
                        sinks.push_back (sink);
                    }
                    catch (const spdlog::spdlog_ex& ex)
                    {
                        std::string errStr =
                            std::string ("Unable to initialise file log sink, reason: ") +
                            std::string (ex.what ());
                        throw std::runtime_error (errStr);
                    }
                }
            }

            auto logger = std::make_shared<spdlog::async_logger> (
                LOGGER_NAME,
                sinks.begin (),
                sinks.end (),
                spdlog::thread_pool (),
                spdlog::async_overflow_policy::block);
            spdlog::register_logger (logger);

            // Set the log format, based on the formatting pattern flags:
            // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
            spdlog::set_pattern (GET_LOGGING_LOG_FORMAT);

            return true;
        }

        bool ServiceContext::ReadIniConfiguration ()
        {
            ConfigSetup layout = ConfigSetup (*m_initLayout);

            m_configManager.Configure (&layout,
                m_iniFile,
                m_usingIniConfig);

            return m_configManager.processConfig ();
        }

        std::string ServiceContext::HttpRequestMethodToStr (HTTPRequestMethod method)
        {
            switch (method)
            {
                case HTTPRequestMethod_GET: return "GET";
                case HTTPRequestMethod_HEAD: return "HEAD";
                case HTTPRequestMethod_POST: return "POST";
                case HTTPRequestMethod_PUT: return "PUT";
                case HTTPRequestMethod_DELETE: return "DELETE";
                case HTTPRequestMethod_CONNECT: return "CONNECT";
                case HTTPRequestMethod_OPTIONS: return "OPTIONS";
                case HTTPRequestMethod_TRACE: return "TRACE";
                case HTTPRequestMethod_PATCH: return "PATCH";
            }

            throw std::runtime_error ("Invalid HttpRequestMethod");
        }


    }   // namespace serviceFramework
}   // namespace items
