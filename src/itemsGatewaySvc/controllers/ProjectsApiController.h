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
#ifndef CONTROLLERS_PROJECTSAPICONTROLLER_H_
#define CONTROLLERS_PROJECTSAPICONTROLLER_H_
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "Logger.h"
#include "DTOs/ProjectsDTOs.h"

namespace items { namespace gatewaySvc { namespace controllers {

#include OATPP_CODEGEN_BEGIN(ApiController)

class ProjectsApiController : public serviceFramework::ApiEndpointController {
 public:
    ProjectsApiController() : ApiEndpointController() { }

    ENDPOINT("GET", "projects/get_projects", projectsGetProjects) {
        LOGGER->critical("get_projects returns a hard-coded list");
        LOGGER->critical("=> doesn't check user, usertoken or auth key");
        LOGGER->critical("=> NOT CURRENTLY IMPLEMENTED!!!!");

        auto response = GetProjectsResponseDTO::createShared();

        response->header = ResponseListHeaderDTO::createShared();
        // HARD-CODED HEADER FOR NOW!
        response->header->offset = 0;
        response->header->limit = 250;
        response->header->size = 2;

        oatpp::List<oatpp::Object<ProjectDTO>> projectsList({});

        auto projectEntry1 = ProjectDTO::createShared();
        projectEntry1->id = 1;
        projectEntry1->name = "test project #1";
        projectEntry1->description = "This is test project #1";
        projectsList->emplace(projectsList->end(), projectEntry1);

        auto projectEntry2 = ProjectDTO::createShared();
        projectEntry2->id = 2;
        projectEntry2->name = "test project #2";
        projectEntry2->description = "This is test project #2";
        projectsList->emplace(projectsList->end(), projectEntry2);

        response->projects = projectsList;

        return createDtoResponse(Status::CODE_200, response);
    }

    ENDPOINT("GET", "projects/get_project/{project_id}", projectsGetProject,
        PATH(Int64, project_id)) {
        printf("Project ID : %lld\n", *project_id);

        auto projectEntry = ProjectDTO::createShared();
        projectEntry->id = 2;
        projectEntry->name = "test project #2";
        projectEntry->description = "This is test project #2";

        return createDtoResponse(Status::CODE_200, projectEntry);
    }
};

#include OATPP_CODEGEN_END(ApiController)

}   // namespace controllers
}   // namespace gatewaySvc
}   // namespace items

#endif  // CONTROLLERS_PROJECTSAPICONTROLLER_H_
