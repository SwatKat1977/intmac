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
#include "RootController.h"

namespace items { namespace webPortalSvc { namespace controllers {

RootController::RootController(
    std::shared_ptr<GatewaySvcClient> gatewaySvcClient,
    serviceFramework::ConfigManager configManager) :
    WebRoute(gatewaySvcClient, configManager) {
}

ResponseSharedPtr RootController::HandleLoginGet(
    std::shared_ptr<IncomingRequest> request) {
    json data;

    std::string serverHost = DetermineServerHost(request);
    data["static_css_dir"] = serverHost + "/static/css/";

    std::string templateDir = configManager_.GetStringEntry(
        "html", "html_directory");
    Environment env(templateDir);
    std::string renderedPage;

    try {
        renderedPage = env.render_file(TEMPLATE_LOGIN_PAGE, data);
    }
    catch(std::exception &ex) {
        printf("[ERROR] : %s\n", ex.what());
    }

    return ApiResponseFactory::createResponse(
        ApiResponseStatus::CODE_200, renderedPage);
}

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items
