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
#ifndef CONFIGURATIONLAYOUT_H_
#define CONFIGURATIONLAYOUT_H_
#include <string>
#include "ConfigSetup.h"
#include "LoggerSettings.h"

namespace items { namespace gatewaySvc {

using serviceFramework::CONFIG_ITEM_TYPE_INTEGER;
using serviceFramework::CONFIG_ITEM_TYPE_STRING;
using serviceFramework::ConfigSetupItem;

const int ONE_MINUTE = 60;

const char BOOL_YES[] = "YES";
const char BOOL_NO[] = "NO";

const char SECTION_SESSIONS[] = "sessions";

const char SESSIONS_SESSION_TIMEOUT_SECS[] = "timeout_secs";

const int SESSIONS_SESSION_TIMEOUT_SECS_DEFAULT = ONE_MINUTE * 60;

const char SECTION_INTERNAL_APIS[] = "internal_apis";

const char INTERNAL_APIS_ACCOUNTS_SVC_HOST[] = "accounts_svc_host";
const char INTERNAL_APIS_ACCOUNTS_SVC_PORT[] = "accounts_svc_port";
const char INTERNAL_APIS_CMS_SVC_HOST[] = "cms_svc_host";
const char INTERNAL_APIS_CMS_SVC_PORT[] = "cms_svc_port";
const char INTERNAL_APIS_ACCOUNTS_SVC_HOST_DEFAULT[] = "localhost";

const int INTERNAL_APIS_ACCOUNTS_SVC_PORT_DEFAULT = 3000;
const char INTERNAL_APIS_CMS_SVC_HOST_DEFAULT[] = "localhost";
const int INTERNAL_APIS_CMS_SVC_PORT_DEFAULT = 4000;

const char SECTION_AUTHENTICATION[] = "authentication";

const char AUTHENTICATION_TOKEN[] = "token";

const serviceFramework::SectionList SessionsSettings = {
    {
        SESSIONS_SESSION_TIMEOUT_SECS,
        ConfigSetupItem(SESSIONS_SESSION_TIMEOUT_SECS, CONFIG_ITEM_TYPE_INTEGER)
                .DefaultValue(SESSIONS_SESSION_TIMEOUT_SECS_DEFAULT)
    }
};

const serviceFramework::SectionList InternalApisSettings = {
    {
        INTERNAL_APIS_ACCOUNTS_SVC_HOST,
        ConfigSetupItem(INTERNAL_APIS_ACCOUNTS_SVC_HOST,
                        CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(INTERNAL_APIS_ACCOUNTS_SVC_HOST_DEFAULT)
    },
    {
        INTERNAL_APIS_ACCOUNTS_SVC_PORT,
        ConfigSetupItem(INTERNAL_APIS_ACCOUNTS_SVC_PORT,
                        CONFIG_ITEM_TYPE_INTEGER)
                .DefaultValue(INTERNAL_APIS_ACCOUNTS_SVC_PORT_DEFAULT)
    },
    {
        INTERNAL_APIS_CMS_SVC_HOST,
        ConfigSetupItem(INTERNAL_APIS_CMS_SVC_HOST, CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(INTERNAL_APIS_CMS_SVC_HOST_DEFAULT)
    },
    {
        INTERNAL_APIS_CMS_SVC_PORT,
        ConfigSetupItem(INTERNAL_APIS_CMS_SVC_PORT, CONFIG_ITEM_TYPE_INTEGER)
                .DefaultValue(INTERNAL_APIS_CMS_SVC_PORT_DEFAULT)
    }
};

const serviceFramework::SectionList AuthenticationSettings = {
    {
        AUTHENTICATION_TOKEN,
        ConfigSetupItem(AUTHENTICATION_TOKEN, CONFIG_ITEM_TYPE_STRING)
                .IsRequired(true)
    },
};

const serviceFramework::SectionsMap CONFIGURATION_LAYOUT_MAP = {
    {serviceFramework::LOGGING_SECTION, serviceFramework::LoggerSettings },
    {SECTION_SESSIONS, SessionsSettings },
    {SECTION_INTERNAL_APIS, InternalApisSettings },
    {SECTION_AUTHENTICATION, AuthenticationSettings }
};

}   // namespace gatewaySvc
}   // namespace items


#endif  // CONFIGURATIONLAYOUT_H_
