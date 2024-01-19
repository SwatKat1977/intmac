/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2024 Integrated Test Management Suite Development Team

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
#include "HandshakeRoutes.h"
#include "ConfigurationLayout.h"
#include "DTOs/HandshakeDTOs.h"
#include "Logger.h"
#include "UUID.h"

namespace items { namespace gatewaySvc {

    const std::string HEADERKEY_TOKEN = "auth_token";

    bool IsValidAuthToken (oatpp::String headerToken, std::string authToken)
    {
        if (!headerToken || headerToken->empty ())
        {
            return false;
        }

        return (headerToken == authToken);
    }

    BasicAuthenticate::BasicAuthenticate (
        std::string name,
        std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> accountsSvcClient,
        std::shared_ptr < SessionsManager> sessionManager)
        : ApiRoute (name),
          m_accountsSvcClient(accountsSvcClient),
          m_sessionManager(sessionManager)
    {
    }

    ApiOutResponsePtr BasicAuthenticate::Route (const ApiIncomingReqPtr& request)
    {
        auto response = BasicAuthenticateResponseDTO::createShared ();

        // ============== STEP 1 ==============
        // Validate the message body
        // ====================================
        auto requestMsg = request->readBodyToDto<oatpp::Object
            <BasicAuthenticateRequestDTO>> (m_objectMapper.get ());

        if ((!requestMsg.get ()->email_address) ||
            (!requestMsg.get ()->password))
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string emailAddress = requestMsg.get ()->email_address;
        std::string password = requestMsg.get ()->password;

        // ============== STEP 2 ==============
        // Request authentication verification from accounts service.
        // ====================================
        auto requestBody = common::apis::accountsSvc::AccountsSvcBasicAuthRequestDto::createShared ();
        requestBody->email_address = emailAddress;
        requestBody->password = requestMsg.get ()->password;
        
        oatpp::Object<common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto> authResponse;

        try
        {
            authResponse = m_accountsSvcClient
                ->basicAuthenticateDTO (requestBody)
                ->readBodyToDto<oatpp::Object<common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto>> (m_objectMapper.get ());
        }
        catch (std::runtime_error& ex)
        {
            LOGGER->error ("Cannot connect to accounts service API, reason: {0}",
                ex.what ());
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD; 
            response->error = "Internal error";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        if (authResponse->status == BASIC_AUTH_RESPONSE_STATUS_BAD)
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid username/password";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string userToken = common::UUID::New ().ToString ();

        bool hasSession = m_sessionManager->HasSession (emailAddress);

        m_sessionManager->AddSession (
            emailAddress,
            userToken,
            AuthenticationType_MANUAL);

        std::string loginType = hasSession ? "re-logged in" : "logged in";
        LOGGER->info ("User '{0}' {1}", emailAddress, loginType);

        response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
        response->error = "";
        response->userToken = userToken;
        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    Logout::Logout (
        std::string name,
        std::shared_ptr < SessionsManager> sessionsManager,
        serviceFramework::ConfigManager configManager)
        : ApiRoute (name), m_sessionsManager (sessionsManager),
        m_configManager(configManager)
    {
    }

    ApiOutResponsePtr Logout::Route (const ApiIncomingReqPtr& request)
    {
        auto response = LogoutResponseDTO::createShared ();

        // ============== STEP 1 ==============
        // Validate the authentication token
        // ====================================

        auto headerToken = request->getHeader (HEADERKEY_TOKEN.c_str());

        std::string authToken = m_configManager.GetStringEntry (
            SECTION_AUTHENTICATION, AUTHENTICATION_TOKEN);

        if (!IsValidAuthToken (headerToken, authToken))
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_401, "");
        }

        // ============== STEP 2 ==============
        // Validate the message body
        // ====================================
        auto requestMsg = request->readBodyToDto<oatpp::Object
            <LogoutRequestDTO>> (m_objectMapper.get ());

        if ((!requestMsg.get ()->email_address) ||
            (!requestMsg.get ()->token))
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string emailAddress = requestMsg.get ()->email_address;
        std::string userToken = requestMsg.get ()->token;

        if (!m_sessionsManager->IsValidSession (emailAddress,
            userToken))
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid user session";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        m_sessionsManager->DeleteSession (emailAddress);
        LOGGER->info ("User '{0}' logged out", emailAddress);

        response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
        response->error = "";
        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    IsValidUserToken::IsValidUserToken (std::string name,
        std::shared_ptr < SessionsManager> sessionsManager,
        serviceFramework::ConfigManager configManager)
        : ApiRoute (name), m_sessionsManager (sessionsManager),
          m_configManager (configManager)
    {
    }

    ApiOutResponsePtr IsValidUserToken::Route (const ApiIncomingReqPtr& request)
    {
        auto response = IsValidUserTokenResponseDTO::createShared ();

        // ============== STEP 1 ==============
        // Validate the authentication token
        // ====================================

        auto headerToken = request->getHeader (HEADERKEY_TOKEN.c_str ());

        std::string authToken = m_configManager.GetStringEntry (
            SECTION_AUTHENTICATION, AUTHENTICATION_TOKEN);

        if (!IsValidAuthToken (headerToken, authToken))
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_401, "");
        }

        // ============== STEP 2 ==============
        // Validate the message body
        // ====================================
        auto requestMsg = request->readBodyToDto<oatpp::Object
            <IsValidUserTokenRequestDTO>> (m_objectMapper.get ());

        if ((!requestMsg.get ()->email_address) ||
            (!requestMsg.get ()->token))
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string emailAddress = requestMsg.get ()->email_address;
        std::string userToken = requestMsg.get ()->token;

        bool validSession = m_sessionsManager->IsValidSession (
            emailAddress, userToken);

        response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
        response->error = "";
        response->is_valid = validSession;

        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    GetProjects::GetProjects (std::string name) : ApiRoute (name)
    {
    }

    ApiOutResponsePtr GetProjects::Route (const ApiIncomingReqPtr& request)
    {
        LOGGER->critical ("GetProjects route currently returns a hard-coded list");
        LOGGER->critical ("GetProjects does not check user, usertoken or auth key - NOT CURRENTLY IMPLEMENTED!!!!");

        auto response = GetProjectsResponseDTO::createShared ();
        oatpp::List<oatpp::Object<GetProjectsProjectDTO>> projectsList ({});

        auto projectEntry = GetProjectsProjectDTO::createShared ();
        projectEntry->name = "test project #1";
        projectEntry->description = "This is test project #1";
        projectsList->emplace (projectsList->end (), projectEntry);

        projectEntry->name = "test project #2";
        projectEntry->description = "This is test project #2";
        projectsList->emplace (projectsList->end (), projectEntry);

        response->projects = projectsList;

        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    SelectProject::SelectProject (std::string name) : ApiRoute (name)
    {
    }

    ApiOutResponsePtr SelectProject::Route (const ApiIncomingReqPtr& request)
    {
        LOGGER->critical ("SelectProject route currently returns a hard-coded list");
        LOGGER->critical ("SelectProject does not check user, usertoken or auth key - NOT CURRENTLY IMPLEMENTED!!!!");

        auto response = SelectProjectResponseDTO::createShared ();

        response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
        response->error = "";

        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

} }   // namespace items::gatewaySvc
