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
#include "Logger.h"
#include "SessionsManager.h"
#include "SessionEntry.h"

namespace items { namespace gatewaySvc {

    SessionsManager::SessionsManager ()
    {
        //LOGGER->info ("Initialised sessions manager...");
    }

    /*
        Add an authentication session to the REDIS database. It will attempt to
        lock the record before adding it to ensure concurrency consistency.

        parameters :
            emailAddress - Email address of the user
            token - Unique token specific for the session
            authType - Type of authentication that occurred

        returns :
            bool - Success status of the add.
    */
    bool SessionsManager::AddSession (
        std::string emailAddress,
        std::string token,
        AuthenticationType authType)
    {
        bool status = false;

        // Session timeouts haven't been implemented yet, so the expiry will
        // always be set to a value of 0 (no expiry).
        auto entry = new SessionEntry (emailAddress, authType, token);

        m_mutex.lock ();

        // If you logon a second time it will invalid any previous session.
        std::map<std::string, SessionEntry*>::iterator it = m_sessions.find (emailAddress);
        if (m_sessions.end () != it)
        {
            delete &* it->second;
        }
        m_sessions[emailAddress] = entry;

        m_mutex.unlock ();

        status = true;

        return status;
    }

    /*
        Remove an authentication session.It will attempt to lock the record
        before deleting it to ensure concurrency consistency.

        parameters :
            emailAddress - Email address of the user

        returns :
            bool - Success status of the session delete.
    */
    bool SessionsManager::DeleteSession (std::string emailAddress)
    {
        bool status = false;

        m_mutex.lock ();

        std::map<std::string, SessionEntry *>::iterator it = m_sessions.find (emailAddress);
        if (m_sessions.end () != it)
        {
            m_sessions.erase (it);
            status = true;
        }

        m_mutex.unlock ();

        return status;
    }

    /*
        Verify if a session token for a given email address is valid.

        parameters :
            emailAddress - Email address of the user\n
            token - Token value

        returns :
            bool - Validity boolean.
    */
    bool SessionsManager::IsValidSession (std::string emailAddress, std::string token)
    {
        bool status = false;

        std::map<std::string, SessionEntry *>::iterator it = m_sessions.find (emailAddress);
        if (m_sessions.end () != it)
        {
            SessionEntry entry = *it->second;
            if (entry.GetToken () == token)
            {
                status = true;
            }
        }

        return status;
    }

    /*
        Verify if a session exits for a given email address.

        parameters :
            email_address - Email address of the user\n

        returns :
            bool - Validity boolean.
    */
    bool SessionsManager::HasSession (std::string emailAddress)
    {
        bool status = false;

        std::map<std::string, SessionEntry *>::iterator it = m_sessions.find (emailAddress);
        if (m_sessions.end () != it)
        {
            status = true;
        }

        return status;
    }

} }   // namespace items::gatewaySvc
