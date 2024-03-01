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
#ifndef HANDSHAKEAPICONTROLLER_H
#define HANDSHAKEAPICONTROLLER_H
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "ApiController.h"
#include "DTOs/HandshakeDTOs.h"
#include "apis/accountsSvc/AccountsSvcClient.h"
#include "SessionsManager.h"
#include "UUID.h"

namespace items { namespace gatewaySvc { namespace controllers {

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

    class HandshakeApiController : public serviceFramework::ApiEndpointController //  public oatpp::web::server::api::ApiController
    {
    public:

        const std::string HEADERKEY_TOKEN = "auth_token";

        HandshakeApiController (
            std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> accountsSvcClient,
            std::shared_ptr < SessionsManager> sessionManager)
            : ApiEndpointController(),
              m_accountsSvcClient(accountsSvcClient),
              m_sessionManager(sessionManager) { }

#ifndef __MUTED__
        bool IsValidAuthToken (oatpp::String headerToken, std::string authToken)
        {
            if (!headerToken || headerToken->empty ())
            {
                return false;
            }

            return (headerToken == authToken);
        }

        ENDPOINT("POST", "handshake/auth/basic/", root,
           BODY_DTO(Object<BasicAuthenticateRequestDTO>, request))
        {
            auto response = BasicAuthenticateResponseDTO::createShared ();

            // ============== STEP 1 ==============
            // Validate the message body
            // ====================================
            if ((!request.get ()->email_address) ||
                (!request.get ()->password))
            {
                response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
                response->error = "Invalid request format";
                return createDtoResponse(Status::CODE_200, response);
            }

            std::string emailAddress = request.get ()->email_address;
            std::string password = request.get ()->password;

            // ============== STEP 2 ==============
            // Request authentication verification from accounts service.
            // ====================================
            auto requestBody = common::apis::accountsSvc::AccountsSvcBasicAuthRequestDto::createShared ();
            requestBody->email_address = emailAddress;
            requestBody->password = request.get ()->password;
            
            oatpp::Object<common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto> authResponse;

            try
            {
                authResponse = m_accountsSvcClient
                    ->basicAuthenticateDTO (requestBody)
                    ->readBodyToDto<oatpp::Object<common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto>> (objectMapper.get ());
            }
            catch (std::runtime_error& ex)
            {
                LOGGER->error ("Cannot connect to accounts service API, reason: {0}",
                    ex.what ());
                response->status = BASIC_AUTH_RESPONSE_STATUS_BAD; 
                response->error = "Internal error";
                return createDtoResponse(Status::CODE_200, response);
            }

            if (authResponse->status == BASIC_AUTH_RESPONSE_STATUS_BAD)
            {
                response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
                response->error = "Invalid username/password";
                return createDtoResponse(Status::CODE_200, response);
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
            return createDtoResponse(Status::CODE_200, response);
        }

#ifdef __MUTED__
        ENDPOINT("GET", "/hello/*", root,
           REQUEST(std::shared_ptr<IncomingRequest>, request))
        {
            auto emailAddress = request->getQueryParameter ("email_address");
            auto userToken = request->getQueryParameter ("token");

#ifdef FOO
            auto dto = MessageDto::createShared();
            dto->statusCode = 200;
            dto->message = "Hello World!";
#endif
            return createDtoResponse(Status::CODE_200, "dto");
        }
#endif
#endif

    private:
        std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> m_accountsSvcClient;
        std::shared_ptr < SessionsManager> m_sessionManager;
    };

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

} } }   // namespace items::gatewaySvc::controllers

#endif  //  #ifndef HANDSHAKEAPICONTROLLER_H
