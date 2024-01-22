/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2024 Integrated Test Management Suite Development Team

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
#ifndef PROJECTSROUTES_H
#define PROJECTSROUTES_H
#include "ServiceContext.h"

namespace items { namespace gatewaySvc { namespace routes { namespace projects {

    using namespace serviceFramework;

    class GetProjects : public ApiRoute
    {
    public:
        GetProjects (std::string name);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);
    };

    class GetProject : public ApiRoute
    {
    public:
        GetProject (std::string name);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);
    };

} } } }   // namespace items::gatewaySvc::routes::projects

#endif // #ifndef PROJECTSROUTES_H
