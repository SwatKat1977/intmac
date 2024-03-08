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
#ifndef CONTROLLERS_ROOTCONTROLLER_H_
#define CONTROLLERS_ROOTCONTROLLER_H_
#include <memory>
#include <string>
#include <vector>
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "WebRoute.h"

namespace items { namespace webPortalSvc { namespace controllers {

#include OATPP_CODEGEN_BEGIN(ApiController)

using serviceFramework::ApiResponseFactory;
using serviceFramework::ApiResponseStatus;

class RootController : public WebRoute {
 public:
    RootController() : WebRoute() {}

    ENDPOINT("GET", "/", root,
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        printf("Homepage entry\n");

        // get cookie header from request
        auto cookieHeader = request->getHeader("Cookie");

        printf("Homepage getCookie\n");

        if (!cookieHeader) {
            printf("Does NOT have cookies\n");
        } else {
            printf("Cookie header : %s\n", cookieHeader.get()->c_str());
            std::vector<std::string> cookieValues = ParseCookieHeader(
                cookieHeader);

            printf("HasAuthCookies : %d\n", HasAuthCookies(cookieValues));
        }

        return ApiResponseFactory::createResponse(
            ApiResponseStatus::CODE_200, "response");
    }
};

#include OATPP_CODEGEN_END(ApiController)

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items

#endif  // CONTROLLERS_ROOTCONTROLLER_H_
