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
#include <memory>
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "spdlog/spdlog.h"
#include "ConfigurationLayoutDefinitions.h"
#include "controllers/RootController.h"
#include "Definitions.h"
#include "StartupModule.h"
#include "Logger.h"
#include "Version.h"

namespace items { namespace webPortalSvc {

using oatpp::parser::json::mapping::ObjectMapper;
using oatpp::web::client::HttpRequestExecutor;
using oatpp::network::tcp::client::ConnectionProvider;
using serviceFramework::AccountsSvcClient;

StartupModule::StartupModule(std::string name)
    : ServiceModule(name) { }

bool StartupModule::Initialise() {
    LOGGER->info("ITEMS Web Portal Microservice V{0}.{1}.{2}-{3}",
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

    LOGGER->info("[HTML]");
    LOGGER->info("-> HTML Directory : {0}",
        m_context->GetConfigManager().GetStringEntry(
            "html", "html_directory").c_str());

    LOGGER->info("[Internal APIs]");
    LOGGER->info("-> Gateway Service API : {0}:{1}",
        m_context->GetConfigManager().GetStringEntry(
            "internal_apis", "gateway_svc_host").c_str(),
        m_context->GetConfigManager().GetIntEntry(
            "internal_apis", "gateway_svc_port"));

    if (!AddServiceProviders()) return false;

    CreateGatewaySvcClient();

    if (!AddRootController()) return false;

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
    catch(std::runtime_error& e) {
        LOGGER->critical("Unable to create service provider '{0}' : {1}",
            "SERVICE_PROVIDER_API_NAME", e.what());
        return false;
    }

    return true;
}

bool StartupModule::AddRootController() {
    try {
        auto controller = std::make_shared<
            controllers::RootController>(gateway_service_client_,
                                         m_context->GetConfigManager());
        m_context->AddApiController(SERVICE_PROVIDER_API_NAME,
                                    controller);
    }
    catch (std::runtime_error& e) {
        LOGGER->critical(
            "Unable to create root controller, reason {0}",
            e.what());
        return false;
    }
    LOGGER->info("Added root controller");

    return true;
}

void StartupModule::CreateGatewaySvcClient() {
    std::string svcHost = m_context->GetConfigManager()
        .GetStringEntry(SECTION_INTERNAL_APIS,
                        INTERNAL_APIS_GATEWAY_SVC_HOST).c_str();
    int svcPort = m_context->GetConfigManager().GetIntEntry(
        SECTION_INTERNAL_APIS, INTERNAL_APIS_GATEWAY_SVC_PORT);

    auto objectMapper = ObjectMapper::createShared();
    auto connectionProvider = ConnectionProvider::createShared(
        { svcHost, (v_uint16)svcPort });
    auto requestExecutor = HttpRequestExecutor::createShared(
        connectionProvider);

    gateway_service_client_ = GatewaySvcClient::createShared(
        requestExecutor, objectMapper);
    LOGGER->info("Created client for Gateway Service api");
}

}   // namespace webPortalSvc
}   // namespace items
