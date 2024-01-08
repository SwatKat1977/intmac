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
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

namespace items
{
    namespace accountsSvc
    {

#include OATPP_CODEGEN_BEGIN(DTO)

        // Definition of the basic_auth authenticate request.
        class BasicAuthenticateRequest : public oatpp::DTO
        {
            DTO_INIT (BasicAuthenticateRequest, DTO)

            DTO_FIELD (String, email_address);
            DTO_FIELD (String, password);
        };

        // Definition of the basic_auth authenticate response.
        class BasicAuthenticateResponse : public oatpp::DTO
        {
            DTO_INIT (BasicAuthenticateResponse, DTO)

            DTO_FIELD (Int32, status);
            DTO_FIELD (String, error);
        };

#include OATPP_CODEGEN_END(DTO)

    }   // namespace accountsSvc
}   // namespace items

