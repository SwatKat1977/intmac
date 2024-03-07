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
#ifndef WEBROUTE_H
#define WEBROUTE_H
#include <string>
#include <vector>
#include "ServiceContext.h"

namespace items { namespace webPortalSvc {

    // Base class for route that serve a web page.
    class WebRoute : public serviceFramework::ApiRoute
    {
    public:
        WebRoute (std::string name);

        ~WebRoute () = default;

        bool HasAuthCookies (std::vector<std::string> cookies);

        bool ValidAuthCookies (std::string tokenValue, std::string userValue);

    protected:
        std::string GenerateRedirect (std::string redirectURLRoot,
            std::string redirectURL);

        inline void LeftTrim (std::string& inStr)
        {
            inStr.erase (inStr.begin (), 
                         std::find_if (inStr.begin (),
                         inStr.end (), [](unsigned char ch)
            {
                return !std::isspace (ch);
            }));
        }

        std::vector<std::string> ParseCookieHeader (const std::string& cookieHeader);

        void GetAuthCookies (
            std::vector<std::string> cookies,
            std::string& tokenValue,
            std::string& userValue);

        bool GetCookieValue (
            const std::string& cookieValue,
            std::string& name,
            std::string& value);
    };

} }   // namespace items::webPortalSvc

#endif  // #ifndef 
