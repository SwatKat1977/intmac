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

The following is based on Ogre3D code:
* GetEnv from os-int.h
-----------------------------------------------------------------------------
*/
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "LoggerSettings.h"
#include "Platform.h"
#include "ServiceContext.h"

#define LOGGER_THREAD_SIZE 8192
#define LOGGER_THREAD_COUNT 1
#define LOGGER_NAME "loggername"

namespace items
{
    namespace serviceFramework
    {

        ServiceContext::ServiceContext (std::string contextName) :
            m_contextName(contextName),
            m_initialised(false),
            m_shutdownRequested(false),
            m_usingIniConfig(false),
            m_initLayout(nullptr)
        {
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

            //m_initialised = ServiceInitialise ();
            return m_initialised;
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
                printf ("Logging to console\n");
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

    }   // namespace serviceFramework
}   // namespace items
