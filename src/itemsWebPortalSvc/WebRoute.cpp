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
#include <string>
#include "WebRoute.h"
#include "Logger.h"
#include "apis/gatewaySvc/GatewaySvcClient.h"
#include "apis/gatewaySvc/GatewaySvcDTOs.h"
#include "apis/CommonDTOs.h"

namespace items { namespace webPortalSvc {

    using common::apis::gatewaySvc::IsValidUserTokenResponseDTO;
    using common::apis::gatewaySvc::GATEWAYSVC_RESPONSE_STATUS_OK;

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

bool WebRoute::IsValidSession(std::string cookieHeader) {
    std::vector<std::string> cookieValues = ParseCookieHeader(
        cookieHeader);

    if (!HasAuthCookies(cookieValues)) return false;

    auto authCookies = GetAuthCookies(cookieValues);

    return CallIsSessionValid(authCookies->User(), authCookies->Token());
}

bool WebRoute::CallIsSessionValid(std::string user, std::string token) {
    std::string tokenValue =  configManager_.GetStringEntry("authentication",
                                                           "token");

    auto requestBody = common::apis::EmptyDTO::createShared();
    oatpp::Object<IsValidUserTokenResponseDTO> callBody;
    try {
        auto response = gatewaySvcClient_->isValidSession(user, token,
            requestBody, tokenValue);

        if (response->getStatusCode() != 200) {
            throw std::runtime_error("Call to check valid session Failed!");
        }

        callBody = response->readBodyToDto<
            oatpp::Object<IsValidUserTokenResponseDTO>>(m_objectMapper.get());
    }
    catch (std::runtime_error& ex) {
        LOGGER->error("Cannot connect to gateway service API, reason: {0}",
            ex.what());
        throw std::runtime_error("Cannot connect to gateway service API");
    }

    return (callBody->status == GATEWAYSVC_RESPONSE_STATUS_OK);
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

std::string WebRoute::DecodeUrl(const std::string &encoded) {
    std::ostringstream decoded;
    for (size_t i = 0; i < encoded.size(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 < encoded.size()) {
                // Get the two characters after '%'
                std::string hex = encoded.substr(i + 1, 2);
                // Convert hex string to integer value
                char decodedChar = static_cast<char>(std::stoi(hex, nullptr, 16));
                decoded << decodedChar;
                i += 2;  // Move the index forward past the hex digits
            } else {
                // Invalid encoding, add % as is
                decoded << '%';
            }
        } else if (encoded[i] == '+') {
            // Handle '+' as space (commonly used in query strings)
            decoded << ' ';
        } else {
            // Normal characters, copy as is
            decoded << encoded[i];
        }
    }
    return decoded.str();
}

}   // namespace webPortalSvc
}   // namespace items
