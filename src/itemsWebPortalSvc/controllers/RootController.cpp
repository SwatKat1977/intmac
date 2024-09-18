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
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
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
    auto rawCookies = request->getHeader("Cookie");

    if (rawCookies) {
        auto headers = ParseCookieHeader(rawCookies);

        if (HasAuthCookies(headers) && IsValidSession(rawCookies)) {
            auto redirectUrl = GenerateRedirect("/", "");
            auto response = ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_200, redirectUrl);
            response->putHeader("Content-Type", "text/html");
            return response;
        }
    }

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

ResponseSharedPtr RootController::HandleLoginPost(
    std::shared_ptr<IncomingRequest> request) {

    auto body = request->readBodyToString();
    auto formData = oatpp::network::Url::Parser::parseQueryParams("?" + body);

    // Extract specific form entries
    auto userEmail = formData.get("user_email");
    auto password = formData.get("password");

    if (!userEmail || !password) {
        printf("something went wrong\n");
        return createResponse(Status::CODE_405, "TBD WRONG!");
    }

    std::string email_decoded = DecodeUrl(userEmail->c_str());
    std::string password_decoded = DecodeUrl(password->c_str());

    OATPP_LOGD("Root Controller", "User Email : %s",
               email_decoded.c_str());
    OATPP_LOGD("Root Controller", "Password   : %s",
               password_decoded.c_str());

    rapidjson::Document auth_body;
    auth_body.SetObject();
    rapidjson::Document::AllocatorType& allocator = auth_body.GetAllocator();

    auth_body.AddMember("email_address",
                        rapidjson::Value().SetString(email_decoded.c_str(),
                        allocator), allocator);
    auth_body.AddMember("password",
                        rapidjson::Value().SetString(password_decoded.c_str(),
                        allocator), allocator);

    // --- TEMPORARY ---
    if (email_decoded == "admin@test.internal" &&
        password_decoded == "TestTest2024") {

        printf("[DEBUG] Login success (temp)\n");
        auto redirectUrl = GenerateRedirect("/", "");
        auto response = ApiResponseFactory::createResponse(
            ApiResponseStatus::CODE_302, redirectUrl);
        response->putHeader("Content-Type", "text/html");

        std::string user_cookie = std::string(COOKIE_USER) + "=" +
                                              email_decoded +
                                              "; Path=/; HttpOnly";
                                              //"; Path=/; HttpOnly; Secure";
        response->putHeader("Set-Cookie", user_cookie);

        std::string token_cookie = std::string(COOKIE_TOKEN) + "=" +
                                               "TOKEN" +
                                               "; Path=/; HttpOnly";
                                               // "; Path=/; HttpOnly; Secure";
        response->putHeader("Set-Cookie", token_cookie);

        return response;
    }

#ifdef old_
    auth_body = {
        "email_address": user_email,
        "password": password
    }
    url = f"{Configuration().internal_api_gateway}/handshake/basic_authenticate"

    try:
        response = requests.post(url, json = auth_body)

    except requests.exceptions.ConnectionError as ex:
        self._logger.error('Connection to gateway api timed out: %s',
                           str(ex))
        return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

    body = json.loads(response.content,
                      object_hook=lambda d: SimpleNamespace(**d))

    if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
        except_str = ("Internal error communicating with gateway: "
                  f"{body.error}")
        self._logger.error(except_str)
        return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

    if body.status == 1:
        redirect = self._generate_redirect('')
        response = await make_response(redirect)
        response.set_cookie(self.COOKIE_USER, user_email)
        response.set_cookie(self.COOKIE_TOKEN, body.token)
        return response

    else:
        error_msg = "Invalid username/password"
        return await render_template(self.TEMPLATE_LOGIN_PAGE,
                                     generate_error_msg = True,
                                     error_msg = error_msg)

return self._generate_redirect('/login')

#endif


    return createResponse(Status::CODE_405, "TO BE DONE");
}

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items
