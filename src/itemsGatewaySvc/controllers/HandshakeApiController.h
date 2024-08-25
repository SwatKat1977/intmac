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
-----------------------------------------------------------------------------
*/
#ifndef CONTROLLERS_HANDSHAKEAPICONTROLLER_H_
#define CONTROLLERS_HANDSHAKEAPICONTROLLER_H_
#include <memory>
#include <string>
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "ApiController.h"
#include "DTOs/CommonDTOs.h"
#include "DTOs/HandshakeDTOs.h"
#include "apis/accountsSvc/AccountsSvcClient.h"
#include "SessionsManager.h"
#include "UUID.h"

namespace items { namespace gatewaySvc { namespace controllers {

using common::apis::accountsSvc::AccountsSvcClient;
using serviceFramework::AccountsSvcBasicAuthRequestDto;
using common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto;

#include OATPP_CODEGEN_BEGIN(ApiController)

class HandshakeApiController : public serviceFramework::ApiEndpointController {
 public:
    const std::string HEADERKEY_TOKEN = "auth_token";

    HandshakeApiController(
        std::shared_ptr<AccountsSvcClient> accountsSvcClient,
        std::shared_ptr<SessionsManager> sessionManager,
        serviceFramework::ConfigManager configManager)
        : ApiEndpointController(),
          m_accountsSvcClient(accountsSvcClient),
          m_configManager(configManager),
          m_sessionManager(sessionManager) { }

    bool IsValidAuthToken(oatpp::String headerToken, std::string authToken) {
        if (!headerToken || headerToken->empty()) {
            return false;
        }

        return (headerToken == authToken);
    }

    ENDPOINT("POST", "handshake/auth/basic/authenticate",
       handshakeBasicAuthenticate,
       BODY_DTO(Object<BasicAuthenticateRequestDTO>, request)) {
        auto response = BasicAuthenticateResponseDTO::createShared();

        // ============== STEP 1 ==============
        // Validate the message body
        // ====================================
        if ((!request.get()->email_address) ||
            (!request.get()->password)) {
            response->status = RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return createDtoResponse(Status::CODE_200, response);
        }

        std::string emailAddress = request.get()->email_address;
        std::string password = request.get()->password;

        // ============== STEP 2 ==============
        // Request authentication verification from accounts service.
        // ====================================
        auto requestBody = AccountsSvcBasicAuthRequestDto::createShared();
        requestBody->email_address = emailAddress;
        requestBody->password = request.get()->password;

        oatpp::Object<AccountsSvcBasicAuthResponseDto> authResponse;

        try {
            authResponse = m_accountsSvcClient
                ->basicAuthenticateDTO(requestBody)
                ->readBodyToDto<
                oatpp::Object<AccountsSvcBasicAuthResponseDto>>(
                    m_objectMapper.get());
        }
        catch (std::runtime_error& ex) {
            LOGGER->error("Cannot connect to accounts service API, reason: {0}",
                ex.what());
            response->status = RESPONSE_STATUS_BAD;
            response->error = "Internal error";
            return createDtoResponse(Status::CODE_200, response);
        }

        if (authResponse->status == RESPONSE_STATUS_BAD) {
            response->status = RESPONSE_STATUS_BAD;
            response->error = "Invalid username/password";
            return createDtoResponse(Status::CODE_200, response);
        }

        std::string userToken = common::UUID::New().ToString();

        bool hasSession = m_sessionManager->HasSession(emailAddress);

        m_sessionManager->AddSession(
            emailAddress,
            userToken,
            AuthenticationType_MANUAL);

        std::string loginType = hasSession ? "re-logged in" : "logged in";
        LOGGER->info("User '{0}' {1}", emailAddress, loginType);

        response->status = RESPONSE_STATUS_OK;
        response->error = "";
        response->userToken = userToken;
        return createDtoResponse(Status::CODE_200, response);
    }

    ENDPOINT("POST", "handshake/logout", handshakeLogout,
       BODY_DTO(Object<LogoutRequestDTO>, requestBody),
       REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto response = LogoutResponseDTO::createShared();

        // ============== STEP 1 ==============
        // Validate the authentication token
        // ====================================
        auto headerToken = request->getHeader(HEADERKEY_TOKEN.c_str());

        std::string authToken = m_configManager.GetStringEntry(
            SECTION_AUTHENTICATION, AUTHENTICATION_TOKEN);

        if (!IsValidAuthToken(headerToken, authToken)) {
            return ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_401, "");
        }

        // ============== STEP 2 ==============
        // Validate the message body
        // ====================================
        if ((!requestBody.get()->email_address) ||
            (!requestBody.get()->token)) {
            response->status = RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string emailAddress = requestBody.get()->email_address;
        std::string userToken = requestBody.get()->token;

        if (!m_sessionManager->IsValidSession(emailAddress,
            userToken)) {
            response->status = RESPONSE_STATUS_BAD;
            response->error = "Invalid user session";
            return ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        m_sessionManager->DeleteSession(emailAddress);
        LOGGER->info("User '{0}' logged out", emailAddress);

        response->status = RESPONSE_STATUS_OK;
        response->error = "";
        return ApiResponseFactory::createResponse(
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    ENDPOINT("GET", "handshake/is_valid_user_token",
             handshakeIsValidUserToken,
        QUERY(String, emailAddress, "email_address"),
        QUERY(String, userToken, "token"),
        BODY_DTO(Object<EmptyDTO>, unused),
        REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto response = IsValidUserTokenResponseDTO::createShared();

        // ============== STEP 1 ==============
        // Validate the authentication token
        // ====================================
        auto headerToken = request->getHeader(HEADERKEY_TOKEN.c_str());

        std::string authToken = m_configManager.GetStringEntry(
            SECTION_AUTHENTICATION, AUTHENTICATION_TOKEN);

        if (!IsValidAuthToken(headerToken, authToken)) {
            auto response = createResponse(Status::CODE_401, "Not authorised");
            response->putHeader(Header::CONTENT_TYPE, "text/plain");
            return response;
        }

        // ============== STEP 2 ==============
        // Validate query parameters
        // ====================================
        if (emailAddress->empty() || userToken->empty()) {
            response->status = RESPONSE_STATUS_BAD;
            response->error = "Missing email or token";
            response->is_valid = false;
            return createDtoResponse(Status::CODE_200, response);
        }

        bool validSession = m_sessionManager->IsValidSession(
            emailAddress, userToken);

        response->status = RESPONSE_STATUS_OK;
        response->error = "";
        response->is_valid = validSession;

        return ApiResponseFactory::createResponse(
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

 private:
    std::shared_ptr<AccountsSvcClient> m_accountsSvcClient;
    serviceFramework::ConfigManager m_configManager;
    std::shared_ptr < SessionsManager> m_sessionManager;
};

#include OATPP_CODEGEN_END(ApiController)

}   // namespace controllers
}   // namespace gatewaySvc
}   // namespace items

#endif  // CONTROLLERS_HANDSHAKEAPICONTROLLER_H_
