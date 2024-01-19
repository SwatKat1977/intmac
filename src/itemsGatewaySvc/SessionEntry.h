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
#ifndef SESSION_ENTRY_H
#define SESSION_ENTRY_H
#include <string>

namespace items { namespace gatewaySvc {

    // Enumeration for account authentication type
    enum AuthenticationType
    {
        AuthenticationType_GOOGLE_API = 0,
        AuthenticationType_FACEBOOK = 1,
        AuthenticationType_MANUAL = 2
    };

    class SessionEntry
    {
    public:
        SessionEntry (std::string emailAddress,
            AuthenticationType authenticationType,
            std::string token)
            : m_emailAddress(emailAddress),
              m_authenticationType(authenticationType),
              m_sessionExpiry(0),
              m_token(token)
        {
        }

        std::string GetEmailAddress () { return m_emailAddress; }
        AuthenticationType GetAuthenticationType () { return m_authenticationType; }
        int GetSessionExpiry () { return m_sessionExpiry; }
        std::string GetToken () { return m_token; }

    private:
        std::string m_emailAddress;
        AuthenticationType m_authenticationType;
        int m_sessionExpiry;
        std::string m_token;
    };

} }   // namespace items::gatewaySvc

#endif
