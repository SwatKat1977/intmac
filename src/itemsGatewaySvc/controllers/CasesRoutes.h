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
#ifndef CASESROUTES_H
#define CASESROUTES_H
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace items { namespace gatewaySvc { namespace controllers {

    using namespace serviceFramework;

    class CasesApiController : public serviceFramework::ApiEndpointController
    {
    public:

        CasesApiController () : ApiEndpointController() { }

        ENDPOINT("GET", "cases/get_cases", casesGetCases)
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_400, "NOT INPLEMENTED");
        }

        ENDPOINT("GET", "get_case/{case_id}", casesGetCase.
            PATH(Int64, case_id))
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_400, "NOT INPLEMENTED");
        }
    };

#include OATPP_CODEGEN_END(ApiController)

} } }   // namespace items::gatewaySvc::controllers

#endif // #ifndef CASESROUTES_H
