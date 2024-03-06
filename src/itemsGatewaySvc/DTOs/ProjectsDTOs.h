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
#ifndef DTOS_PROJECTSDTOS_H_
#define DTOS_PROJECTSDTOS_H_
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "DTOs/CommonDTOs.h"

namespace items { namespace gatewaySvc {

    const int BASIC_AUTH_RESPONSE_STATUS_BAD = 0;
    const int BASIC_AUTH_RESPONSE_STATUS_OK = 1;

#include OATPP_CODEGEN_BEGIN(DTO)

// Definition of a project DTO for GetProjects response.
class ProjectDTO : public oatpp::DTO {
    DTO_INIT(ProjectDTO, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(String, description);
};

// Definition of GetProjects response.
class GetProjectsResponseDTO : public oatpp::DTO {
    DTO_INIT(GetProjectsResponseDTO, DTO)

    DTO_FIELD(Object<ResponseListHeaderDTO>, header);
    DTO_FIELD(List<Object<ProjectDTO>>, projects);
};

#include OATPP_CODEGEN_END(DTO)

}   // namespace gatewaySvc
}   // namespace items

#endif  // DTOS_PROJECTSDTOS_H_
