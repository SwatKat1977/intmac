/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
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
#include <filesystem>
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "spdlog/spdlog.h"
#include "ConfigurationLayout.h"
#include "Definitions.h"
#include "StartupModule.h"
#include "Logger.h"
#include "Version.h"
#include "apis/accountsSvc/AccountsSvcClient.h"
#include "controllers/CasesApiController.h"
#include "controllers/HandshakeApiController.h"
#include "controllers/ProjectsApiController.h"

namespace items { namespace gatewaySvc {

    using oatpp::network::tcp::client::ConnectionProvider;
    using oatpp::web::client::HttpRequestExecutor;
    using oatpp::parser::json::mapping::ObjectMapper;

    StartupModule::StartupModule(std::string name) : ServiceModule(name) {
        sessions_manager_ = std::shared_ptr<SessionsManager>(
            new SessionsManager());
    }

    bool StartupModule::Initialise() {
        LOGGER->info("ITEMS Gateway Microservice V{0}.{1}.{2}-{3}",
            common::VERSION_MAJOR, common::VERSION_MINOR,
            common::VERSION_PATCH, common::VERSION_TAG);
        LOGGER->info(common::COPYRIGHT_TEXT);
        LOGGER->info(common::LICENSE_TEXT);

        LOGGER->info("|=====================|");
        LOGGER->info("|=== Configuration ===|");
        LOGGER->info("|=====================|");

        LOGGER->info("[LOGGING]");
        LOGGER->info("-> Log Level      : {0}",
            m_context->GetConfigManager().GetStringEntry(
                "logging", "log_level").c_str());
        LOGGER->info("-> Log To Console : {0}",
            m_context->GetConfigManager().GetStringEntry(
                "logging", "log_to_console").c_str());
        LOGGER->info("-> Log Filename   : {0}",
            m_context->GetConfigManager().GetStringEntry(
                "logging", "log_filename").c_str());
        LOGGER->info("-> Max File Size  : {0:d}",
            m_context->GetConfigManager().GetIntEntry(
                "logging", "max_file_size"));
        LOGGER->info("-> Max File Count : {0:d}",
            m_context->GetConfigManager().GetIntEntry(
                "logging", "max_file_count"));
        LOGGER->info("-> Log Format     : {0}",
            m_context->GetConfigManager().GetStringEntry(
                "logging", "log_format").c_str());

        LOGGER->info("[SESSIONS]");
        LOGGER->info("-> Session Timeout (secs) : {0:d}",
            m_context->GetConfigManager().GetIntEntry(
                "sessions", "timeout_secs"));

        LOGGER->info("[INTERNAL APIS]");
        LOGGER->info("-> Accounts Service API : {0}:{1:d}",
            m_context->GetConfigManager().GetStringEntry(
                SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_HOST).c_str(),
            m_context->GetConfigManager().GetIntEntry(
                SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_PORT));
        LOGGER->info("-> CMS Service API      : {0}:{1:d}",
            m_context->GetConfigManager().GetStringEntry(
                SECTION_INTERNAL_APIS, INTERNAL_APIS_CMS_SVC_HOST).c_str(),
            m_context->GetConfigManager().GetIntEntry(
                SECTION_INTERNAL_APIS, INTERNAL_APIS_CMS_SVC_PORT));

        if (!AddServiceProviders()) {
            return false;
        }

        CreateAccountsSvcClient();

        if (!AddHandshakeRoutes()) return false;
        if (!AddCasesRoutes()) return false;
        if (!AddProjectsRoutes()) return false;

        return true;
    }

    bool StartupModule::AddServiceProviders() {
        try {
            m_context->AddServiceProvider(
                SERVICE_PROVIDER_API_NAME,
                "0.0.0.0",
                SERVICE_PROVIDER_API_PORT,
                serviceFramework::SERVICENETWORKTYPE_IPV4);
        }
        catch (std::runtime_error &e) {
            LOGGER->critical("Unable to create service provider '{0}' : {1}",
                SERVICE_PROVIDER_API_NAME, e.what());
            return false;
        }

        return true;
    }

    bool StartupModule::AddHandshakeRoutes() {
        try {
            auto controller = std::make_shared<
                controllers::HandshakeApiController> (
                    accounts_service_client_, sessions_manager_,
                    m_context->GetConfigManager());
            m_context->AddApiController(SERVICE_PROVIDER_API_NAME,
                                        controller);
        }
        catch (std::runtime_error& e) {
            LOGGER->critical(
                "Unable to create handshake api controller, reason {0}",
                e.what());
            return false;
        }
        LOGGER->info("Added handshake api controller");

        return true;
    }

    bool StartupModule::AddProjectsRoutes() {
        try {
            auto controller = std::make_shared<
                controllers::ProjectsApiController> ();
            m_context->AddApiController(SERVICE_PROVIDER_API_NAME,
                                        controller);
        }
        catch (std::runtime_error& e) {
            LOGGER->critical(
                "Unable to create projects api controller, reason {0}",
                e.what());
            return false;
        }
        LOGGER->info("Added projects api controller");

        return true;
    }

    bool StartupModule::AddCasesRoutes() {
        try {
            auto controller = std::make_shared<
                controllers::CasesApiController>();
            m_context->AddApiController(SERVICE_PROVIDER_API_NAME,
                                        controller);
        }
        catch (std::runtime_error& e) {
            LOGGER->critical(
                "Unable to create cases api controller, reason {0}",
                e.what());
            return false;
        }
        LOGGER->info("Added cases api controller");

        return true;
    }

    void StartupModule::CreateAccountsSvcClient() {
        std::string accountsSvcHost = m_context->GetConfigManager()
            .GetStringEntry(SECTION_INTERNAL_APIS,
                            INTERNAL_APIS_ACCOUNTS_SVC_HOST).c_str();
        int accountsSvcPort = m_context->GetConfigManager().GetIntEntry(
            SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_PORT);

        auto objectMapper = ObjectMapper::createShared();
        auto connectionProvider = ConnectionProvider::createShared(
            { accountsSvcHost, (v_uint16)accountsSvcPort });
        auto requestExecutor = HttpRequestExecutor::createShared(
            connectionProvider);

        accounts_service_client_ = AccountsSvcClient::createShared(
            requestExecutor, objectMapper);
        LOGGER->info("Created client for Account Svc api");
    }

}   // namespace gatewaySvc
}   // namespace items
