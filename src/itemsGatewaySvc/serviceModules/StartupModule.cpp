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

The following is based on Ogre3D code:
* GetEnv from os-int.h
-----------------------------------------------------------------------------
*/
#include <filesystem>
#include "spdlog/spdlog.h"
#include "ConfigurationLayout.h"
#include "Definitions.h"
#include "StartupModule.h"
#include "Logger.h"
#include "Version.h"
#include "routes/CasesRoutes.h"
#include "routes/HandshakeRoutes.h"
#include "routes/ProjectsRoutes.h"
#include "apis/accountsSvc/AccountsSvcClient.h"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

namespace items { namespace gatewaySvc {

    using namespace serviceFramework;

    // ++++++++++++++++++++++++++++++++++++++++++++++++
    // +++++++++++++++ Handshake routes +++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++
    const std::string HANDSHAKE_BASE = "handshake/";

    // Route : Basic authentication
    const std::string BASIC_AUTH_BASE = "auth/basic/";
    const std::string BASIC_AUTHENTICATE_ROUTE = HANDSHAKE_BASE +
        BASIC_AUTH_BASE + "authenticate";
    const std::string BASIC_AUTHENTICATE_ROUTE_NAME = "basicAuth_auth";

    // Route : Logout user session
    const std::string LOGOUT_ROUTE = HANDSHAKE_BASE + "logout";
    const std::string LOGOUT_ROUTE_NAME = "logout";

    // Route : Is Valid User Token
    const std::string ISVALIDUSERTOKEN_ROUTE = HANDSHAKE_BASE + "is_valid_user_token";
    const std::string ISVALIDUSERTOKEN_ROUTE_NAME = "isvalidusertoken";

    // +++++++++++++++++++++++++++++++++++++++++++++++
    // +++++++++++++++ Projects routes +++++++++++++++
    // +++++++++++++++++++++++++++++++++++++++++++++++
    const std::string PROJECTS_BASE = "projects/";

    // Route : Get Projects
    const std::string GETPROJECTS_ROUTE = PROJECTS_BASE + "get_projects";
    const std::string GETPROJECTS_ROUTE_NAME = "getprojects";

    // Route : Get Project
    const std::string GETPROJECT_ROUTE = PROJECTS_BASE + "get_project/{project_id}";
    const std::string GETPROJECT_ROUTE_NAME = "getprojects";

    // ++++++++++++++++++++++++++++++++++++++++++++
    // +++++++++++++++ Cases routes +++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++
    const std::string CASES_BASE = "cases/";

    // Route : Get Cases
    const std::string GETCASES_ROUTE = CASES_BASE + "get_cases";
    const std::string GETCASES_ROUTE_NAME = "getcases";

    // Route : Get Projects
    const std::string GETCASE_ROUTE = CASES_BASE + "get_case/{case_id}";
    const std::string GETCASE_ROUTE_NAME = "getcase";

    StartupModule::StartupModule (std::string name)
        : ServiceModule (name)
    {
        m_sessionsManager = std::shared_ptr<SessionsManager>(new SessionsManager ());
    }

    bool StartupModule::Initialise ()
    {
        LOGGER->info ("ITEMS Gateway Microservice V{0}.{1}.{2}-{3}",
            common::VERSION_MAJOR, common::VERSION_MINOR,
            common::VERSION_PATCH, common::VERSION_TAG);
        LOGGER->info (common::COPYRIGHT_TEXT);
        LOGGER->info (common::LICENSE_TEXT);

        LOGGER->info ("|=====================|");
        LOGGER->info ("|=== Configuration ===|");
        LOGGER->info ("|=====================|");

        LOGGER->info ("[LOGGING]");
        LOGGER->info ("-> Log Level      : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_level").c_str ());
        LOGGER->info ("-> Log To Console : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_to_console").c_str ());
        LOGGER->info ("-> Log Filename   : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_filename").c_str ());
        LOGGER->info ("-> Max File Size  : {0:d}",
            m_context->GetConfigManager ().GetIntEntry (
                "logging", "max_file_size"));
        LOGGER->info ("-> Max File Count : {0:d}",
            m_context->GetConfigManager ().GetIntEntry (
                "logging", "max_file_count"));
        LOGGER->info ("-> Log Format     : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_format").c_str ());

        LOGGER->info ("[SESSIONS]");
        LOGGER->info ("-> Session Timeout (secs) : {0:d}",
            m_context->GetConfigManager ().GetIntEntry (
                "sessions", "timeout_secs"));

        LOGGER->info ("[INTERNAL APIS]");
        LOGGER->info ("-> Accounts Service API : {0}:{1:d}",
            m_context->GetConfigManager ().GetStringEntry (
                SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_HOST).c_str (),
            m_context->GetConfigManager ().GetIntEntry (
                SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_PORT));
        LOGGER->info ("-> CMS Service API      : {0}:{1:d}",
            m_context->GetConfigManager ().GetStringEntry (
                SECTION_INTERNAL_APIS, INTERNAL_APIS_CMS_SVC_HOST).c_str (),
            m_context->GetConfigManager ().GetIntEntry (
                SECTION_INTERNAL_APIS, INTERNAL_APIS_CMS_SVC_PORT));

        if (!AddServiceProviders ())
        {
            return false;
        }

        CreateAccountsSvcClient ();

        if (!AddHandshakeRoutes ()) return false;
        if (!AddProjectsRoutes ()) return false;
        if (!AddCasesRoutes ()) return false;

        return true;
    }

    bool StartupModule::AddServiceProviders ()
    {
        try
        {
            m_context->AddServiceProvider (
                SERVICE_PROVIDER_API_NAME,
                "0.0.0.0",
                SERVICE_PROVIDER_API_PORT,
                SERVICENETWORKTYPE_IPV4);
        }
        catch (std::runtime_error &e)
        {
            LOGGER->critical ("Unable to create service provider '{0}' : {1}",
                SERVICE_PROVIDER_API_NAME, e.what ());
            return false;
        }

        return true;
    }

    bool StartupModule::AddHandshakeRoutes ()
    {
        auto* basicAuthRoute = new routes::handshake::BasicAuthenticate (
            BASIC_AUTHENTICATE_ROUTE_NAME, m_accountsSvcClient,
            m_sessionsManager);
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_POST,
                BASIC_AUTHENTICATE_ROUTE,
                basicAuthRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                BASIC_AUTHENTICATE_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added basic auth route '{0}' to '{1}' provider",
            BASIC_AUTHENTICATE_ROUTE, SERVICE_PROVIDER_API_NAME);

        auto* logoutRoute = new routes::handshake::Logout (
            LOGOUT_ROUTE_NAME,
            m_sessionsManager,
            m_context->GetConfigManager ());
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_POST,
                LOGOUT_ROUTE,
                logoutRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                LOGOUT_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added logout route '{0}' to '{1}' provider",
            LOGOUT_ROUTE, SERVICE_PROVIDER_API_NAME);

        auto* isValidUserTokenRoute = new routes::handshake::IsValidUserToken (
            ISVALIDUSERTOKEN_ROUTE_NAME,
            m_sessionsManager,
            m_context->GetConfigManager ());
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_POST,
                ISVALIDUSERTOKEN_ROUTE,
                isValidUserTokenRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                ISVALIDUSERTOKEN_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added is valid user token route '{0}' to '{1}' provider",
            ISVALIDUSERTOKEN_ROUTE, SERVICE_PROVIDER_API_NAME);

        return true;
    }

    bool StartupModule::AddProjectsRoutes ()
    {
        auto* getProjectsRoute = new routes::projects::GetProjects (GETPROJECTS_ROUTE_NAME);
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_GET,
                GETPROJECTS_ROUTE,
                getProjectsRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                GETPROJECTS_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added get projects route '{0}' to '{1}' provider",
            GETPROJECTS_ROUTE, SERVICE_PROVIDER_API_NAME);

        auto* getProjectRoute = new routes::projects::GetProject (GETPROJECT_ROUTE_NAME);
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_GET,
                GETPROJECT_ROUTE,
                getProjectRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                GETPROJECTS_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added get project route '{0}' to '{1}' provider",
            GETPROJECT_ROUTE, SERVICE_PROVIDER_API_NAME);

        return true;
    }

    bool StartupModule::AddCasesRoutes ()
    {
        auto* getCasesRoute = new routes::cases::GetCases (GETCASES_ROUTE_NAME);
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_GET,
                GETCASES_ROUTE,
                getCasesRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                GETCASES_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added get cases route '{0}' to '{1}' provider",
            GETCASES_ROUTE, SERVICE_PROVIDER_API_NAME);

        auto* getCaseRoute = new routes::cases::GetCase (GETCASE_ROUTE_NAME);
        try
        {
            m_context->AddRoute (
                SERVICE_PROVIDER_API_NAME,
                HTTPRequestMethod_GET,
                GETCASE_ROUTE,
                getCaseRoute);
        }
        catch (std::runtime_error& e)
        {
            LOGGER->critical ("Unable to create route '{0}' : {1}",
                GETCASE_ROUTE_NAME, e.what ());
            return false;
        }
        LOGGER->info ("Added get case route '{0}' to '{1}' provider",
            GETCASE_ROUTE, SERVICE_PROVIDER_API_NAME);

        return true;
    }

    void StartupModule::CreateAccountsSvcClient ()
    {
        std::string accountsSvcHost = m_context->GetConfigManager ().GetStringEntry (
            SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_HOST).c_str ();
        int accountsSvcPort = m_context->GetConfigManager ().GetIntEntry (
            SECTION_INTERNAL_APIS, INTERNAL_APIS_ACCOUNTS_SVC_PORT);

        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared ();
        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared (
            { accountsSvcHost, (v_uint16)accountsSvcPort });
        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared (connectionProvider);

        m_accountsSvcClient = AccountsSvcClient::createShared (requestExecutor, objectMapper);
        LOGGER->info ("Created client for Account Svc api");
    }

} }   // namespace items::gatewaySvc
