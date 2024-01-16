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
#ifndef CONFIGURATIONLAYOUT_H
#define CONFIGURATIONLAYOUT_H
#include "ConfigSetup.h"
#include "LoggerSettings.h"

namespace items
{
    namespace gatewaySvc
    {
        using namespace serviceFramework;

        const int ONE_MINUTE = 60;

        const std::string BOOL_YES = "YES";
        const std::string BOOL_NO = "NO";

        const std::string SECTION_SESSIONS = "sessions";

        const std::string SESSIONS_SESSION_TIMEOUT_SECS = "timeout_secs";

        const int SESSIONS_SESSION_TIMEOUT_SECS_DEFAULT = ONE_MINUTE * 60;

        const std::string SECTION_INTERNAL_APIS = "internal_apis";

        const std::string SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC = "accounts_svc";
        const std::string SESSIONS_INTERNAL_APIS_CMS_SVC = "cms_svc";
        const std::string SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC_DEFAULT = "http://localhost:3000/";
        const std::string SESSIONS_INTERNAL_APIS_CMS_SVC_DEFAULT = "http://localhost:4000/";

        const SectionList SessionsSettings =
        {
            {
                SESSIONS_SESSION_TIMEOUT_SECS,
                ConfigSetupItem (SESSIONS_SESSION_TIMEOUT_SECS, CONFIG_ITEM_TYPE_INTEGER)
                        .DefaultValue (SESSIONS_SESSION_TIMEOUT_SECS_DEFAULT)
            }
        };

        const SectionList InternalApisSettings =
        {
            {
                SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC,
                ConfigSetupItem (SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC_DEFAULT)
            },
            {
                SESSIONS_INTERNAL_APIS_CMS_SVC,
                ConfigSetupItem (SESSIONS_INTERNAL_APIS_CMS_SVC, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (SESSIONS_INTERNAL_APIS_CMS_SVC_DEFAULT)
            }
        };

        SectionsMap CONFIGURATION_LAYOUT_MAP =
        {
            {LOGGING_SECTION, LoggerSettings },
            {SECTION_SESSIONS, SessionsSettings },
            {SECTION_INTERNAL_APIS, InternalApisSettings }
        };

    }   // namespace gatewaySvc
}   // namespace items


#endif
