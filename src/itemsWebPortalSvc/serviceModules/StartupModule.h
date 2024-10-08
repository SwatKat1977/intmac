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
#ifndef SERVICEMODULES_STARTUPMODULE_H_
#define SERVICEMODULES_STARTUPMODULE_H_
#include <string>
#include "apis/gatewaySvc/GatewaySvcClient.h"
#include "ServiceContext.h"
#include "ItemTypes.h"

namespace items { namespace webPortalSvc {

class StartupModule : public serviceFramework::ServiceModule {
 public:
    explicit StartupModule(std::string name);

 private:
    serviceFramework::GatewaySvcClientptr gateway_service_client_;

    bool AddServiceProviders();

    bool AddRootController();

    void CreateGatewaySvcClient();

    bool Initialise();
};

}  // namespace webPortalSvc
}  // namespace items

#endif  // SERVICEMODULES_STARTUPMODULE_H_
