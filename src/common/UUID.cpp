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
#include "UUID.h"
#include <iostream>
#include <random>

namespace items
{
    namespace common
    {
        UUID UUID::New ()
        {
            UUID uuid;
            std::random_device rd;
            std::mt19937 engine{ rd () };
            std::uniform_int_distribution<int> dist{ 0, 256 }; //Limits of the interval

            for (int index = 0; index < 16; ++index)
            {
                uuid.m_data[index] = (unsigned char)dist (engine);
            }

            uuid.m_data[6] = ((uuid.m_data[6] & 0x0f) | 0x40); // Version 4
            uuid.m_data[8] = ((uuid.m_data[8] & 0x3f) | 0x80); // Variant is 10

            return uuid;
        }

        std::string UUID::ToString ()
        {
            // Formats to "0065e7d7-418c-4da4-b4d6-b54b6cf7466a"
            char buffer[256] = { 0 };
            std::snprintf (buffer, 255,
                "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                m_data[0], m_data[1], m_data[2], m_data[3],
                m_data[4], m_data[5],
                m_data[6], m_data[7],
                m_data[8], m_data[9],
                m_data[10], m_data[11], m_data[12], m_data[13], m_data[14], m_data[15]);

            std::string uuid = buffer;

            return uuid;
        }

    }   // namespace accountsSvc
}   // namespace items
