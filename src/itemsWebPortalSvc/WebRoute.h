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
#ifndef WEBROUTE_H_
#define WEBROUTE_H_
#include <string>
#include <vector>
#include "apis/gatewaySvc/GatewaySvcClient.h"
#include "ServiceContext.h"

namespace items { namespace webPortalSvc {

using common::apis::gatewaySvc::GatewaySvcClient;

class CookieKeyPair {
 public:
    CookieKeyPair(std::string key, std::string value)
        : key_(key), value_(value) {}

    inline std::string Key() { return key_; }
    inline std::string Value() { return value_; }

 private:
    std::string key_;
    std::string value_;
};

struct AuthCookies {
 public:
    AuthCookies(std::string token, std::string user)
        : token_(token), user_(user) {}

    inline std::string Token() { return token_; }
    inline std::string User() { return user_; }

 private:
    std::string token_;
    std::string user_;
};

// Base class for route that serve a web page.
class WebRoute  : public serviceFramework::ApiEndpointController {
 public:
    WebRoute(std::shared_ptr<GatewaySvcClient> gatewaySvcClient);

    ~WebRoute() = default;

    bool HasAuthCookies(std::vector<std::string> cookies);

 protected:
    std::shared_ptr<GatewaySvcClient> gatewaySvcClient_;

    std::string GenerateRedirect(std::string redirectURLRoot,
        std::string redirectURL);

    inline void LeftTrim(std::string& inStr) {  // NOLINT
        inStr.erase(inStr.begin(),
                    std::find_if(inStr.begin(),
                    inStr.end(), [](unsigned char ch) {
            return !std::isspace (ch);
        }));
    }

    std::vector<std::string> ParseCookieHeader(const std::string& cookieHeader);

    AuthCookies *GetAuthCookies(std::vector<std::string> cookies);

    CookieKeyPair *GetCookieValue(const std::string& cookieValue);
};

}   // namespace webPortalSvc
}   // namespace items

#endif  // WEBROUTE_H_
