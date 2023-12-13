#include <exception>
#include <iostream>
#include "spdlog/spdlog.h"
#include "ServiceContext.h"
#include "LoggerSettings.h"
#include "oatpp/web/server/HttpConnectionHandler.hpp"

using namespace items::serviceFramework;

#define LOGGER spdlog::get ("loggername")

class TestModule : public ServiceModule
{
public:

    TestModule (std::string name) : ServiceModule (name)
    {
    }

    bool Initialise ()
    {
        printf ("Initialise test module...\n");

        LOGGER->info ("[LOGGING]");
        LOGGER->info ("-> Log Level      : {0}",
                      m_context->GetConfigManager ().GetStringEntry ("logging", "log_level").c_str ());
        LOGGER->info ("-> Log To Console : {0}",
            m_context->GetConfigManager ().GetStringEntry ("logging", "log_to_console").c_str ());
        LOGGER->info ("-> Log Filename   : {0}",
            m_context->GetConfigManager ().GetStringEntry ("logging", "log_filename").c_str ());
        LOGGER->info ("-> Max File Size  : {0:d}",
            m_context->GetConfigManager ().GetIntEntry ("logging", "max_file_size"));
        LOGGER->info ("-> Max File Count : {0:d}",
            m_context->GetConfigManager ().GetIntEntry ("logging", "max_file_count"));
        LOGGER->info ("-> Log Format     : {0}",
            m_context->GetConfigManager ().GetStringEntry ("logging", "log_format").c_str ());

        return true;
    }
};

class TestRoute1 : public ApiRoute
{
public:
    TestRoute1 (std::string name) : ApiRoute (name) {}

    virtual ApiOutResponsePtr Route (const ApiIncomingReqPtr& request)
    {
        return ApiResponseFactory::createResponse (ApiResponseStatus::CODE_200, "This is my endpoint");
    }

};

int main ()
{
    SectionsMap CONFIGURATION_LAYOUT_MAP =
    {
        {LOGGING_SECTION, LoggerSettings }
    };

    TestModule module = TestModule ("Test Init");

    auto context = new ServiceContext ("Test Application");

    context->AddServiceModule(&module);

    if (!context->Initialise (&CONFIGURATION_LAYOUT_MAP, "test.ini"))
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
        context->AddRoute ("entry1", HTTPRequestMethod_GET, "/test", new TestRoute1("A Route"));
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
