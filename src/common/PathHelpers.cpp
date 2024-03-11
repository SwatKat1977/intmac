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
#include "PathHelpers.h"
#include "Platform.h"

namespace items {

std::string PathAppend(const std::string &left, const std::string &right) {
#if ITEMS_PLATFORM == ITEMS_PLATFORM_WINDOWS_CORE || \
    ITEMS_PLATFORM == ITEMS_PLATFORM_WINDOWS_MSVC
    char seperator = '\\';
#else
    char seperator = '/';
#endif

    std::string joinedPath = left;
    bool leftHasSeperator = (left.back() == seperator);

    if (right.front() == seperator) {
        if (leftHasSeperator) joinedPath.pop_back();
    } else {
        if (!leftHasSeperator) joinedPath += seperator;
    }
    joinedPath += right;

    return joinedPath;
}

}   // namespace items
