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
#include <iostream>
#include "spdlog/spdlog.h"
#include "ServiceContext.h"
#include "ConfigurationLayout.h"
#include "Platform.h"

using namespace items::accountsSvc;
using namespace items::serviceFramework;

const std::string SERVICE_CONTEXT_NAME = "Items Accounts Svc";

int main ()
{
    auto configFile = GetEnv ("ITEMS_ACCOUNTS_SVC_CONFIG_FILE");
    auto configFileRequired = GetEnv ("ITEMS_ACCOUNTS_SVC_CONFIG_FILE_REQUIRED");

    bool fileIsRequired = true ? (!configFileRequired.empty () &&
                                  configFileRequired == "YES")
                               : false;

    if (configFile.empty () && fileIsRequired)
    {
        std::cout << "[FATAL ERROR] Configuration file missing!" << std::endl;
        return EXIT_FAILURE;
    }

    // TestModule module = TestModule ("Test Init");

    auto context = new ServiceContext (SERVICE_CONTEXT_NAME);

    // context->AddServiceModule (&module);

    if (!context->Initialise (&CONFIGURATION_LAYOUT_MAP, configFile))
    {
        return EXIT_FAILURE;
    }

    try
    {
        context->AddServiceProvider ("entry1", "localhost", 8008, SERVICENETWORKTYPE_IPV4);
        context->AddServiceProvider ("entry2", "localhost", 8099, SERVICENETWORKTYPE_IPV4);
    }
    catch (std::invalid_argument e)
    {
        std::cout << "Exception : " << e.what () << std::endl;
    }

#ifdef __DISABLED__
    try
    {
        context->AddRoute ("entry1", HTTPRequestMethod_GET, "/test", new TestRoute1 ("A Route"));
        //context->AddRoute ("Invad", HTTPRequestMethod_GET, "/test", nullptr);
    }
    catch (std::runtime_error e)
    {
        std::cout << "Exception : " << e.what () << std::endl;
        return EXIT_FAILURE;
    }

    context->Execute ();
#endif

    return EXIT_SUCCESS;
}