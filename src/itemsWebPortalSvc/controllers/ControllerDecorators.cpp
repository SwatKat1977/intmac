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
#include <string>
#include "ControllerDecorators.h"

namespace items { namespace webPortalSvc { namespace controllers {

bool AuthKeyInHeaderValid(const std::shared_ptr<OatppRequest>& request,
                          std::string auth_key_key,
                          std::string auth_key_value) {
    // Get the value of the auth key header value.
    auto headerValue = request->getHeader(auth_key_key.c_str());

    // Check if the header is missing or empty
    if (!headerValue || headerValue->empty()) {
        return false;
    }

    // Check if it equal to the auth key value;
    return (headerValue == auth_key_value);
}

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items
