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
#ifndef PLATFORM_H
#define PLATFORM_H
#include <assert.h>
#include <string>

#define ITEMS_PLATFORM_WINDOWS      1
#define ITEMS_PLATFORM_WINDOWS_MSVC 2
#define ITEMS_PLATFORM_LINUX        3

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(__ANDROID__)
#  if defined(_MSC_VER)
#    define ITEMS_PLATFORM ITEMS_PLATFORM_WINDOWS_MSVC 
#  else
#    define ITEMS_PLATFORM ITEMS_PLATFORM_WINDOWS_CORE
#  endif
#else
#    define ITEMS_PLATFORM ITEMS_PLATFORM_LINUX
#endif

#define ItemsAssert( expr, mesg ) assert( (expr) && (mesg) )

std::string GetEnv (const char* field);

#endif
