/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite )
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
#ifndef SESSIONSMANAGER_H_
#define SESSIONSMANAGER_H_
#include <map>
#include <string>
#include "SessionEntry.h"

namespace items { namespace gatewaySvc {

class SessionsManager {
 public:
    SessionsManager();

    bool AddSession(
        std::string emailAddress,
        std::string token,
        AuthenticationType authType);

    bool DeleteSession(std::string emailAddress);

    bool IsValidSession(std::string emailAddress, std::string token);

    bool HasSession(std::string email_address);

 private:
    std::map<std::string, SessionEntry *> m_sessions;
    std::mutex m_mutex;
};

}   // namespace gatewaySvc
}   // namespace items

#endif  // SESSIONSMANAGER_H_
