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
#ifndef COMMONDTOS_H
#define COMMONDTOS_H
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

namespace items { namespace gatewaySvc {

    const int RESPONSE_STATUS_BAD = 0;
    const int RESPONSE_STATUS_OK = 1;

#include OATPP_CODEGEN_BEGIN(DTO)

    // Definition of SelectProject response.
    class ResponseListHeaderDTO : public oatpp::DTO
    {
        DTO_INIT (ResponseListHeaderDTO, DTO)

        DTO_FIELD (Int32, offset);
        DTO_FIELD (Int16, limit);
        DTO_FIELD (Int8, size);
    };

#include OATPP_CODEGEN_END(DTO)

} }   // namespace items::gatewaySvc

#endif
