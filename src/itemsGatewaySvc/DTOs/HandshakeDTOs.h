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
#ifndef DTOS_HANDSHAKEDTOS_H_
#define DTOS_HANDSHAKEDTOS_H_
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "DTOs/CommonDTOs.h"

namespace items { namespace gatewaySvc {

#include OATPP_CODEGEN_BEGIN(DTO)

// Definition of the basic authenticate request.
class BasicAuthenticateRequestDTO : public oatpp::DTO {
    DTO_INIT(BasicAuthenticateRequestDTO, DTO)

    DTO_FIELD(String, email_address);
    DTO_FIELD(String, password);
};

// Definition of the basic authenticate response.
class BasicAuthenticateResponseDTO : public oatpp::DTO {
    DTO_INIT(BasicAuthenticateResponseDTO, DTO)

    DTO_FIELD(Int8, status);
    DTO_FIELD(String, error);
    DTO_FIELD(String, userToken);
};

// Definition of the logout request.
class LogoutRequestDTO : public oatpp::DTO {
    DTO_INIT(LogoutRequestDTO, DTO)

    DTO_FIELD(String, email_address);
    DTO_FIELD(String, token);
};

class LogoutResponseDTO : public oatpp::DTO {
    DTO_INIT(LogoutResponseDTO, DTO)

    DTO_FIELD(Int8, status);
    DTO_FIELD(String, error);
};

// Definition of the is valid user token response.
class IsValidUserTokenResponseDTO : public oatpp::DTO {
    DTO_INIT(IsValidUserTokenResponseDTO, DTO)

    DTO_FIELD(Int8, status);
    DTO_FIELD(String, error);
    DTO_FIELD(Int8, is_valid);
};

#include OATPP_CODEGEN_END(DTO)

}   // namespace gatewaySvc
}   // namespace items

#endif  // DTOS_HANDSHAKEDTOS_H_
