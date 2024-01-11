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
#include "ServiceContext.h"
#include "SqliteInterface.h"

namespace items
{
    namespace accountsSvc
    {
        using namespace serviceFramework;

        class BasicAuthAuthenticate : public ApiRoute
        {
        public:
            BasicAuthAuthenticate (std::string name, SqliteInterface *sqlite);

            ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);

        private:
            SqliteInterface* m_sqlite;
        };

    }   // namespace accountsSvc
}   // namespace items
