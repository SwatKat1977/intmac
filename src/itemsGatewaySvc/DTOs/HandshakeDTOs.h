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
#ifndef HANDSHAKEDTOS_H
#define HANDSHAKEDTOS_H
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

namespace items { namespace gatewaySvc {

    const int BASIC_AUTH_RESPONSE_STATUS_BAD = 0;
    const int BASIC_AUTH_RESPONSE_STATUS_OK = 1;

#include OATPP_CODEGEN_BEGIN(DTO)

    // Definition of the basic authenticate request.
    class BasicAuthenticateRequestDTO : public oatpp::DTO
    {
        DTO_INIT (BasicAuthenticateRequestDTO, DTO)

        DTO_FIELD (String, email_address);
        DTO_FIELD (String, password);
    };

    // Definition of the basic authenticate response.
    class BasicAuthenticateResponseDTO : public oatpp::DTO
    {
        DTO_INIT (BasicAuthenticateResponseDTO, DTO)

        DTO_FIELD (Int8, status);
        DTO_FIELD (String, error);
    };

#include OATPP_CODEGEN_END(DTO)

} }   // namespace items::gatewaySvc

#endif
