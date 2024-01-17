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
#ifndef ACCOUNTSSVCCLIENT_H
#define ACCOUNTSSVCCLIENT_H
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "apis/accountsSvc/AccountsSvcDTOs.h"

namespace items { namespace common { namespace apis { namespace accountsSvc {

    class AccountsSvcClient : public oatpp::web::client::ApiClient {
#include OATPP_CODEGEN_BEGIN(ApiClient)

        API_CLIENT_INIT (AccountsSvcClient)

            // Basic authentication
            API_CALL (
                "POST",
                "basic_auth/authenticate",
                basicAuthenticateDTO,
                BODY_DTO (Object<AccountsSvcBasicAuthRequestDto>, body)
            )

#include OATPP_CODEGEN_END(ApiClient)
    };

 } } } } // namespace items::common::apis::accountsSvc

#endif
