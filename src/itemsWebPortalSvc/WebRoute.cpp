/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2023 Integrated Test Management Suite Development Team

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
#include <sstream>
#include "WebRoute.h"
#include "Logger.h"

namespace items { namespace webPortalSvc {

    // Name of cookies.
    const std::string COOKIE_TOKEN = "items_token";
    const std::string COOKIE_USER = "items_user";

    const std::string REDIRECT_URL =
        "<meta http-equiv=\"Refresh\" content=\"0; url='{0}\"/>";

    WebRoute::WebRoute(std::shared_ptr<GatewaySvcClient> gatewaySvcClient,
                       serviceFramework::ConfigManager configManager)
        : serviceFramework::ApiEndpointController(),
          gatewaySvcClient_(gatewaySvcClient), configManager_(configManager) {
    }

    std::string WebRoute::GenerateRedirect(std::string redirectURLRoot,
        std::string redirectURL) {
        std::stringstream url;
        url << "<meta http-equiv=\"Refresh\" content=\"0; url='"
            << redirectURLRoot << redirectURL << "'\"/>";
        return url.str();
    }

AuthCookies *WebRoute::GetAuthCookies(std::vector<std::string> cookies) {
    bool tokenCookieFound = false;
    bool userCookieFound = false;
    std::string tokenValue;
    std::string userValue;

    // Iterate cook entries looking for correct one.
    for (const auto& cookieEntry : cookies) {
        CookieKeyPair *pair = GetCookieValue(cookieEntry);
        if (!pair) continue;

        if (pair->Key() == COOKIE_TOKEN) {
            tokenCookieFound = true;
            tokenValue = pair->Value();
        }

        if (pair->Key() == COOKIE_USER) {
            userCookieFound = true;
            userValue = pair->Value();
        }
    }

    if (tokenCookieFound && userCookieFound) {
        return new AuthCookies(tokenValue, userValue);
    }

    return nullptr;
}

bool WebRoute::HasAuthCookies(std::vector<std::string> cookies) {
    auto authCookies = GetAuthCookies(cookies);

    if (!authCookies || authCookies->Token().empty() ||
        authCookies->User().empty()) {
        return false;
    }

    return true;
}

bool WebRoute::AuthCookiesValidate(std::string user, std::string token) {
    bool returnStatus = false;

/*
    url = f"{Configuration().internal_api_gateway}/handshake/valid_token"

    request_body = {
        "email_address": username,
        "token": token
    }

    try:
        response = requests.get(url, json = request_body, timeout=1)

    except requests.exceptions.ConnectionError as ex:
        raise ItemsException('Connection to gateway api timed out') from ex

    data = json.loads(response.content,
                      object_hook=lambda d: SimpleNamespace(**d))

    if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
        except_str = ("Internal error communicating with gateway: "
                      f"{data.status}")
        raise ItemsException(except_str)

    if data.status == "VALID":
        return_status = True
*/
    return returnStatus;
}

std::vector<std::string> WebRoute::ParseCookieHeader(
    const std::string& cookieHeader) {
    std::vector<std::string> cookieValues;
    std::istringstream iss(cookieHeader);
    std::string cookieValue;

    while (std::getline(iss, cookieValue, ';')) {
        LeftTrim(cookieValue);
        cookieValues.push_back(cookieValue);
    }

    return cookieValues;
}

CookieKeyPair *WebRoute::GetCookieValue(const std::string& cookieValue) {
    size_t delimiterPos = cookieValue.find('=');
    if (delimiterPos != std::string::npos) {
        return new CookieKeyPair(cookieValue.substr(0, delimiterPos),
                                 cookieValue.substr(delimiterPos + 1));
    }

    return nullptr;
}

}   // namespace webPortalSvc
}   // namespace items
