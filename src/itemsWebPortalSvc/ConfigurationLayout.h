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
#include "ConfigurationLayoutDefinitions.h"

namespace items { namespace webPortalSvc {

const serviceFramework::SectionList HtmlSettings = {
    {
        HTMl_DIRECTORY,
        serviceFramework::ConfigSetupItem(HTMl_DIRECTORY,
                        serviceFramework::CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(HTMl_DIRECTORY_DEFAULT)
    }
};

const serviceFramework::SectionList InternalApisSettings = {
    {
        INTERNAL_APIS_GATEWAY_SVC_HOST,
        serviceFramework::ConfigSetupItem(INTERNAL_APIS_GATEWAY_SVC_HOST,
                        serviceFramework::CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(INTERNAL_APIS_GATEWAY_SVC_HOST_DEFAULT)
    },
    {
        INTERNAL_APIS_GATEWAY_SVC_PORT,
        serviceFramework::ConfigSetupItem(INTERNAL_APIS_GATEWAY_SVC_PORT,
                    serviceFramework::CONFIG_ITEM_TYPE_INTEGER)
                .DefaultValue(INTERNAL_APIS_GATEWAY_SVC_PORT_DEFAULT)
    }
};

const serviceFramework::SectionList AuthenticationSettings = {
    {
        AUTHENTICATION_TOKEN,
        serviceFramework::ConfigSetupItem(AUTHENTICATION_TOKEN,
                    serviceFramework::CONFIG_ITEM_TYPE_STRING)
                .IsRequired(true)
    },
};

serviceFramework::SectionsMap CONFIGURATION_LAYOUT_MAP = {
    {serviceFramework::LOGGING_SECTION, serviceFramework::LoggerSettings},
    {SECTION_HTML, HtmlSettings},
    {SECTION_INTERNAL_APIS, InternalApisSettings},
    {SECTION_AUTHENTICATION, AuthenticationSettings }
};

}   // namespace webPortalSvc
}   // namespace items

#endif  // CONFIGURATIONLAYOUT_H_
