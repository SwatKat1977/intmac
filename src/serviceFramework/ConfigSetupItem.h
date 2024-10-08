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
#ifndef CONFIGSETUPITEM_H
#define CONFIGSETUPITEM_H
#include <list>
#include <string>

namespace items
{
    namespace serviceFramework
    {
        // Enumeration for configuration item data type
        enum ConfigItemDataType
        {
            CONFIG_ITEM_TYPE_INTEGER = 0,
            CONFIG_ITEM_TYPE_STRING = 1
        };

        using StringList = std::list<std::string>;
        using IntList = std::list<int>;

        // Configuration layout class
        class ConfigSetupItem
        {
        public:
            ConfigSetupItem(std::string itemName, ConfigItemDataType dataType);

            std::string ItemName ();

            ConfigItemDataType Type();

            bool IsUnset ();

            bool IsRequired ();

            int DefaultIntValue ();

            std::string DefaultStringValue ();

            IntList ValidIntValues ();

            StringList ValidStringValues ();

            ConfigSetupItem &IsRequired(bool state);

            ConfigSetupItem &DefaultValue(int defaultValue);
            ConfigSetupItem &DefaultValue(std::string defaultValue);

            ConfigSetupItem &ValidValues(IntList validValue);
            ConfigSetupItem &ValidValues(StringList validValue);

        private:
            std::string m_itemName;
            bool m_isRequired;
            ConfigItemDataType m_itemType;
            bool m_isUnset;

            IntList m_validValuesInt;
            StringList m_validValuesStr;

            int m_defaultValueInt;
            std::string m_defaultValueStr;
        };

    }   // namespace serviceFramework
}   // namespace items

#endif
