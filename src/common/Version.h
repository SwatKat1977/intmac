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

The following is based on Ogre3D code:
* GetEnv from os-int.h
-----------------------------------------------------------------------------
*/
#ifndef VERSION_H
#define VERSION_H
#include <string>

namespace items
{
    namespace accountsSvc
    {
        const std::string COPYRIGHT_TEXT = "Copyright 2014 - 2023 Integrated Test Management Suite";
        const std::string LICENSE_TEXT = "Licensed under the Apache License, Version 2.0";

        const std::string VERSION_MAJOR = "0";
        const std::string VERSION_MINOR = "0";
        const std::string VERSION_PATCH = "0";
        const std::string VERSION_TAG = "Alpha-01";

    }   // namespace accountsSvc
}   // namespace items

#endif