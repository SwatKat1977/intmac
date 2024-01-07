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
    namespace accountsSvc
    {
        using namespace serviceFramework;

        const std::string BOOL_YES = "YES";
        const std::string BOOL_NO = "NO";

        const std::string SECTION_BACKEND = "backend";

        const std::string BACKEND_INTERNAL_DB_FILENAME = "internal_db_filename";
        const std::string BACKEND_CREATE_DB_IF_MISSING = "create_db_if_missing";

        const std::string BACKEND_INTERNAL_DB_FILENAME_DEFAULT = "accounts_svc.db";
        const std::string BACKEND_CREATE_DB_IF_MISSING_DEFAULT = BOOL_NO;

        const SectionList BackendSettings =
        {
            {
                BACKEND_INTERNAL_DB_FILENAME,
                ConfigSetupItem (BACKEND_INTERNAL_DB_FILENAME, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (BACKEND_INTERNAL_DB_FILENAME_DEFAULT)
            },
            {
                BACKEND_CREATE_DB_IF_MISSING,
                ConfigSetupItem (BACKEND_CREATE_DB_IF_MISSING, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (BOOL_NO)
                        .ValidValues (StringList{ BOOL_YES, BOOL_NO })
            }
        };

        SectionsMap CONFIGURATION_LAYOUT_MAP =
        {
            {LOGGING_SECTION, LoggerSettings },
            {SECTION_BACKEND, BackendSettings }
        };

    }   // namespace accountsSvc
}   // namespace items


#endif
