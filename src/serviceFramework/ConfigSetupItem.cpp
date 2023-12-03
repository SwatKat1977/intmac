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
#include "ConfigSetupItem.h"

namespace items
{
    namespace serviceFramework
    {
    ConfigSetupItem::ConfigSetupItem (
        std::string itemName,
        ConfigItemDataType dataType) : m_itemName(itemName),
                                       m_isRequired(false),
                                       m_itemType(dataType),
                                       m_isUnset(true),
                                       m_defaultValueInt(0),
                                       m_defaultValueStr("")
    {
    }

    std::string ConfigSetupItem::ItemName ()
    {
        return m_itemName;
    }

    ConfigItemDataType ConfigSetupItem::Type()
    {
        return m_itemType;
    }

    bool ConfigSetupItem::IsUnset ()
    {
        return m_isUnset;
    }

    bool ConfigSetupItem::IsRequired ()
    {
        return m_isRequired;
    }

    int ConfigSetupItem::DefaultIntValue ()
    {
        return m_defaultValueInt;
    }

    std::string ConfigSetupItem::DefaultStringValue ()
    {
        return m_defaultValueStr;
    }

    IntList ConfigSetupItem::ValidIntValues ()
    {
        return m_validValuesInt;
    }

    StringList ConfigSetupItem::ValidStringValues ()
    {
        return m_validValuesStr;
    }

    ConfigSetupItem &ConfigSetupItem::IsRequired (bool state)
    {
        m_isRequired = state;
        return *this;
    }

    ConfigSetupItem &ConfigSetupItem::DefaultValue (int defaultValue)
    {
        m_defaultValueInt = defaultValue;
        m_isUnset = false;
        return *this;
    }

    ConfigSetupItem &ConfigSetupItem::DefaultValue(std::string defaultValue)
    {
        m_defaultValueStr = defaultValue;
        m_isUnset = false;
        return *this;
    }

    ConfigSetupItem &ConfigSetupItem::ValidValues(IntList validValue)
    {
        m_validValuesInt = validValue;
        return *this;
    }

    ConfigSetupItem &ConfigSetupItem::ValidValues(StringList validValue)
    {
        m_validValuesStr = validValue;
        return *this;
    }

    }   // namespace serviceFramework
}   // namespace items


int main(int argc, char **argv)
{
    return 0;
}