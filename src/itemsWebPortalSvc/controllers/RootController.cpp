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

std::string RootController::DetermineServerHost(
    std::shared_ptr<IncomingRequest> request) {
    // Get the host and port
    auto hostHeader = request->getHeader(Header::HOST);

    // Determine the scheme (http or https) based on the request's
    // connection or headers. Assume http by default
    std::string scheme = "http";

    // If you're running behind a proxy or load balancer, you might need to
    // check for "X-Forwarded-Proto" header
    auto forwardedProto = request->getHeader("X-Forwarded-Proto");
    if (forwardedProto && forwardedProto == "https") {
        scheme = "https";
    }

    // Construct the full URL
    std::string serverUrl = scheme + "://" + hostHeader->c_str();

    return serverUrl;
}

ResponseSharedPtr RootController::HandleLoginGet(
    std::shared_ptr<IncomingRequest> request) {
    inja::json data;

    std::string serverHost = DetermineServerHost(request);
    data["static_css_dir"] = serverHost + "/static/css/";

    std::string templateDir = configManager_.GetStringEntry(
        "html", "html_directory");
    inja::Environment env(templateDir);
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
