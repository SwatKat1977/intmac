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

        const std::string BOOL_YES = "YES";
        const std::string BOOL_NO = "NO";

        const std::string SECTION_SESSIONS_REDIS = "sessions_redis";

        const std::string SESSIONS_REDIS_HOSTNAME = "hostname";
        const std::string SESSIONS_REDIS_USERNAME = "username";
        const std::string SESSIONS_REDIS_PASSWORD = "password";
        const std::string SESSIONS_REDIS_PORT = "port";
        const std::string SESSIONS_REDIS_RETRIES = "retries";

        const std::string SESSIONS_REDIS_HOSTNAME_DEFAULT = "localhost";
        const int SESSIONS_REDIS_PORT_DEFAULT = 10000;
        const int SESSIONS_REDIS_RETRIES_DEFAULT = 3;

        const std::string SECTION_INTERNAL_APIS = "internal_apis";

        const std::string SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC = "accounts_svc";
        const std::string SESSIONS_INTERNAL_APIS_CMS_SVC = "cms_svc";
        const std::string SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC_DEFAULT = "http://localhost:3000/";
        const std::string SESSIONS_INTERNAL_APIS_CMS_SVC_DEFAULT = "http://localhost:4000/";

        const SectionList SessionsRedisSettings =
        {
            {
                SESSIONS_REDIS_HOSTNAME,
                ConfigSetupItem (SESSIONS_REDIS_HOSTNAME, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (SESSIONS_REDIS_HOSTNAME_DEFAULT)
            },
            {
                SESSIONS_REDIS_USERNAME,
                ConfigSetupItem (SESSIONS_REDIS_USERNAME, CONFIG_ITEM_TYPE_STRING)
                        .IsRequired(true)
            },
            {
                SESSIONS_REDIS_PASSWORD,
                ConfigSetupItem (SESSIONS_REDIS_PASSWORD, CONFIG_ITEM_TYPE_STRING)
                        .IsRequired (true)
            },
            {
                SESSIONS_REDIS_PORT,
                ConfigSetupItem (SESSIONS_REDIS_PORT, CONFIG_ITEM_TYPE_INTEGER)
                        .DefaultValue (SESSIONS_REDIS_PORT_DEFAULT)
            },
            {
                SESSIONS_REDIS_RETRIES,
                ConfigSetupItem (SESSIONS_REDIS_RETRIES, CONFIG_ITEM_TYPE_INTEGER)
                        .DefaultValue (SESSIONS_REDIS_RETRIES_DEFAULT)
            }
        };

        const SectionList InternalApisSettings =
        {
            {
                SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC,
                ConfigSetupItem (SESSIONS_INTERNAL_APIS_ACCOUNTS_SVC, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (SESSIONS_REDIS_HOSTNAME_DEFAULT)
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
            {SECTION_SESSIONS_REDIS, SessionsRedisSettings },
            {SECTION_INTERNAL_APIS, InternalApisSettings }
        };

    }   // namespace accountsSvc
}   // namespace items


#endif
