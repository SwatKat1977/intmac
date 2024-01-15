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
#include <iostream>
#include "Platform.h"
#include "ConfigurationLayout.h"
#include "ServiceContext.h"
#include "serviceModules/StartupModule.h"

using namespace items::gatewaySvc;
using namespace items::serviceFramework;

const std::string SERVICE_CONTEXT_NAME = "Items Gateway Svc";

int main ()
{
    auto configFile = GetEnv ("ITEMS_GATEWAY_SVC_CONFIG_FILE");
    auto configFileRequired = GetEnv ("ITEMS_GATEWAY_SVC_CONFIG_FILE_REQUIRED");

    bool fileIsRequired = true ? (!configFileRequired.empty () &&
        configFileRequired == "YES")
        : false;

    if (configFile.empty () && fileIsRequired)
    {
        std::cout << "[FATAL ERROR] Configuration file missing!" << std::endl;
        return EXIT_FAILURE;
    }

    StartupModule startupModule = StartupModule ("Startup Module");

    auto context = new ServiceContext (SERVICE_CONTEXT_NAME);

    context->AddServiceModule (&startupModule);

    if (!context->Initialise (&CONFIGURATION_LAYOUT_MAP, configFile))
    {
        return EXIT_FAILURE;
    }

    context->Execute ();

    return EXIT_SUCCESS;
}
