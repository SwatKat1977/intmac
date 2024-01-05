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
#include "spdlog/spdlog.h"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "ConfigurationLayout.h"
#include "Logger.h"
#include "Platform.h"
#include "ServiceContext.h"
#include "StartupModule.h"

using namespace items::accountsSvc;
using namespace items::serviceFramework;

const std::string SERVICE_CONTEXT_NAME = "Items Accounts Svc";

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class MessageDto : public oatpp::DTO {

    DTO_INIT (MessageDto, DTO /* Extends */)

    DTO_FIELD (Int32, statusCode);   // Status code field
    DTO_FIELD (String, message);     // Message field

};
/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)


class TestRoute1 : public ApiRoute
{
public:
    TestRoute1 (std::string name) : ApiRoute (name) {}

    virtual ApiOutResponsePtr Route (const ApiIncomingReqPtr& request)
    {
        auto inMsg = request->readBodyToDto<oatpp::Object<MessageDto>> (m_objectMapper.get ());

        try
        {
            if ((!inMsg.get ()->message) || (!inMsg.get ()->statusCode))
            {
                return ApiResponseFactory::createResponse (ApiResponseStatus::CODE_200, "Invalid msg");
            }
        }
        catch (...)
        {
            printf ("Invalid messge\n");
        }

        std::string msg = inMsg.get ()->message;
        int status = inMsg.get ()->statusCode;

        auto message = MessageDto::createShared ();
        message->statusCode = status;
        message->message = msg;
        return ApiResponseFactory::createResponse (ApiResponseStatus::CODE_200, message, m_objectMapper);
    }
};

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

    StartupModule startupModule = StartupModule ("Startup Module");

    auto context = new ServiceContext (SERVICE_CONTEXT_NAME);

    context->AddServiceModule (&startupModule);

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

    try
    {
        context->AddRoute ("entry1", HTTPRequestMethod_POST, "/test", new TestRoute1 ("A Route"));
        //context->AddRoute ("Invad", HTTPRequestMethod_GET, "/test", nullptr);
    }
    catch (std::runtime_error e)
    {
        std::cout << "Exception : " << e.what () << std::endl;
        return EXIT_FAILURE;
    }

    context->Execute ();

    return EXIT_SUCCESS;
}
