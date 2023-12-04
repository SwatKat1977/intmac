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
#ifndef LOGGERSETTINGS_H
#define LOGGERSETTINGS_H
#include <string>
#include "ConfigSetup.h"
#include "ConfigSetupItem.h"

namespace items
{
    namespace serviceFramework
    {
        const std::string LOGGING_SECTION = "logging";

        const std::string LOGGING_LOG_LEVEL = "log_level";
        const std::string LOGGING_LOG_LEVEL_DEBUG = "DEBUG";
        const std::string LOGGING_LOG_LEVEL_INfO = "INFO";

        const std::string LOGGING_LOG_TO_CONSOLE = "log_to_console";
        const std::string LOGGING_LOG_TO_CONSOLE_YES = "YES";
        const std::string LOGGING_LOG_TO_CONSOLE_NO = "NO";

        const std::string LOGGING_LOG_FILENAME = "log_filename";

        const std::string LOGGING_MAX_FILE_SIZE = "max_file_size";

        const std::string LOGGING_MAX_FILE_COUNT = "max_file_count";

        const std::string LOGGING_LOG_FORMAT = "log_format";
        const std::string LOGGING_LOG_FORMAT_DEFAULT = "%Y-%m-%d %H:%M:%S %^%l%$ [%n] %v";

        const SectionList LoggerSettings =
        {
            {
                LOGGING_LOG_LEVEL,
                ConfigSetupItem (LOGGING_LOG_LEVEL, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (LOGGING_LOG_LEVEL_INfO)
                        .ValidValues (StringList{ LOGGING_LOG_LEVEL_DEBUG,
                                                    LOGGING_LOG_LEVEL_INfO })
            },
            {
                LOGGING_LOG_TO_CONSOLE,
                ConfigSetupItem (LOGGING_LOG_TO_CONSOLE, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (LOGGING_LOG_TO_CONSOLE_NO)
                        .ValidValues (StringList{ LOGGING_LOG_TO_CONSOLE_YES,
                                                    LOGGING_LOG_TO_CONSOLE_NO })
            },
            {
                LOGGING_LOG_FILENAME,
                ConfigSetupItem (LOGGING_LOG_FILENAME, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue ("")
            },
            {
                LOGGING_MAX_FILE_SIZE,
                ConfigSetupItem (LOGGING_MAX_FILE_SIZE, CONFIG_ITEM_TYPE_INTEGER)
                        .DefaultValue (0)
            },
            {
                LOGGING_MAX_FILE_COUNT,
                ConfigSetupItem (LOGGING_MAX_FILE_COUNT, CONFIG_ITEM_TYPE_INTEGER)
                        .DefaultValue (1)
            },
            {
                LOGGING_LOG_FORMAT,
                ConfigSetupItem (LOGGING_LOG_FORMAT, CONFIG_ITEM_TYPE_STRING)
                        .DefaultValue (LOGGING_LOG_FORMAT_DEFAULT)
            }
        };

#define GET_LOGGING_LOG_TO_CONSOLE m_configManager.GetStringEntry(\
            LOGGING_SECTION, LOGGING_LOG_TO_CONSOLE)

#define GET_LOGGING_LOG_FILENAME m_configManager.GetStringEntry(\
            LOGGING_SECTION, LOGGING_LOG_FILENAME)

#define GET_LOGGING_MAX_FILE_SIZE m_configManager.GetIntEntry(\
            LOGGING_SECTION, LOGGING_MAX_FILE_SIZE)

#define GET_LOGGING_MAX_FILE_COUNT m_configManager.GetIntEntry(\
            LOGGING_SECTION, LOGGING_MAX_FILE_COUNT)

#define GET_LOGGING_LOG_FORMAT m_configManager.GetStringEntry(\
            LOGGING_SECTION, LOGGING_LOG_FORMAT)

        const int ONE_MEGABYTE = 1048576;

    }   // namespace serviceFramework
}   // namespace items


#endif
