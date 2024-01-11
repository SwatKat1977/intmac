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
#include "BasicAuthAuthenticate.h"
#include "Definitions.h"
#include "DTOs/BasicAuthDTOs.h"
#include "SqliteInterface.h"

namespace items
{
    namespace accountsSvc
    {
        const int BASIC_AUTH_RESPONSE_STATUS_BAD = 0;
        const int BASIC_AUTH_RESPONSE_STATUS_OK = 1;

        BasicAuthAuthenticate::BasicAuthAuthenticate (
            std::string name,
            SqliteInterface* sqlite) : ApiRoute (name)
        {
            m_sqlite = sqlite;
        }

        ApiOutResponsePtr BasicAuthAuthenticate::Route (const ApiIncomingReqPtr& request)
        {
            auto response = BasicAuthenticateResponse::createShared ();
            auto requestMsg = request->readBodyToDto<oatpp::Object
                <BasicAuthenticateRequest>> (m_objectMapper.get ());

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

            int userId = m_sqlite->GetUserIdForUser (
                emailAddress, LogonType_BASIC);

            if (userId == -1)
            {
                response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
                response->error = "Invalid username/password";
                return ApiResponseFactory::createResponse (
                    ApiResponseStatus::CODE_200, response,
                    m_objectMapper);
            }

            bool authStatus = m_sqlite->BasicAuthenticateUser (userId, password);

            if (authStatus)
            {
                response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
                response->error = "";
            }
            else
            {
                response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
                response->error = "Invalid username/password";
            }

            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }
    }   // namespace accountsSvc
}   // namespace items
