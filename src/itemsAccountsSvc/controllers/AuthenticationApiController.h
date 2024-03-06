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
#ifndef CONTROLLERS_AUTHENTICATIONAPICONTROLLER_H_
#define CONTROLLERS_AUTHENTICATIONAPICONTROLLER_H_
#include <string>
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "DTOs/BasicAuthDTOs.h"
#include "Logger.h"
#include "SqliteInterface.h"

namespace items { namespace accountsSvc { namespace controllers {

using serviceFramework::ApiResponseStatus;
using serviceFramework::ApiResponseFactory;

const int BASIC_AUTH_RESPONSE_STATUS_BAD = 0;
const int BASIC_AUTH_RESPONSE_STATUS_OK = 1;

#include OATPP_CODEGEN_BEGIN(ApiController)

class AuthenticationApiController
    : public serviceFramework::ApiEndpointController {
 public:
    explicit AuthenticationApiController(SqliteInterface *sqlite)
        : ApiEndpointController(), sqlite_(sqlite) {}

    ENDPOINT("POST", "/basic_auth/authenticate",
        authenticationBasicAuthenticate,
       BODY_DTO(Object<BasicAuthenticateRequest>, body)) {
        auto response = BasicAuthenticateResponse::createShared();

        if ((!body.get()->email_address) ||
            (!body.get()->password)) {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string emailAddress = body.get()->email_address;
        std::string password = body.get()->password;

        int userId = sqlite_->GetUserIdForUser(
            emailAddress, LogonType_BASIC);

        if (userId == -1) {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid username/password";
            return ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        bool authStatus = sqlite_->BasicAuthenticateUser(userId, password);

        if (authStatus) {
            response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
            response->error = "";
        } else {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid username/password";
        }

        return ApiResponseFactory::createResponse(
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

 private:
    SqliteInterface* sqlite_;
};

#include OATPP_CODEGEN_END(ApiController)

}   // namespace controllers
}   // namespace accountsSvc
}   // namespace items

#endif  // CONTROLLERS_AUTHENTICATIONAPICONTROLLER_H_
