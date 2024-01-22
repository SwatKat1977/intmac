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
#include "Logger.h"
#include "DTOs/ProjectsDTOs.h"
#include "routes/ProjectsRoutes.h"

namespace items { namespace gatewaySvc { namespace routes { namespace projects {

    GetProjects::GetProjects (std::string name) : ApiRoute (name)
    {
    }

    ApiOutResponsePtr GetProjects::Route (const ApiIncomingReqPtr& request)
    {
        LOGGER->critical ("GetProjects route currently returns a hard-coded list");
        LOGGER->critical ("GetProjects does not check user, usertoken or auth key - NOT CURRENTLY IMPLEMENTED!!!!");

        auto name = request->getQueryParameter("name");
        if (name)
        {
            printf("[UNUSED] Project name : %s\n", name->c_str());
        }

        auto response = GetProjectsResponseDTO::createShared ();

        response->header = ResponseListHeaderDTO::createShared ();
        // HARD-CODED HEADER FOR NOW!
        response->header->offset = 0;
        response->header->limit = 250;
        response->header->size = 2;

        oatpp::List<oatpp::Object<ProjectDTO>> projectsList ({});

        auto projectEntry1 = ProjectDTO::createShared ();
        projectEntry1->id = 1;
        projectEntry1->name = "test project #1";
        projectEntry1->description = "This is test project #1";
        projectsList->emplace (projectsList->end (), projectEntry1);

        auto projectEntry2 = ProjectDTO::createShared ();
        projectEntry2->id = 2;
        projectEntry2->name = "test project #2";
        projectEntry2->description = "This is test project #2";
        projectsList->emplace (projectsList->end (), projectEntry2);

        response->projects = projectsList;

        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    GetProject::GetProject (std::string name) : ApiRoute (name)
    {
    }

    ApiOutResponsePtr GetProject::Route (const ApiIncomingReqPtr& request)
    {
        auto projectId = request->getPathVariables().getVariable("project_id");
        printf("Project ID : %s\n", projectId->c_str());

        auto projectEntry = ProjectDTO::createShared();
        projectEntry->id = 2;
        projectEntry->name = "test project #2";
        projectEntry->description = "This is test project #2";

        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, projectEntry,
            m_objectMapper);
    }

} } } }   // namespace items::gatewaySvc::routes::projects
