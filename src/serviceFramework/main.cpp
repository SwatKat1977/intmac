#include <exception>
#include <iostream>
#include "spdlog/spdlog.h"
#include "ServiceContext.h"
#include "LoggerSettings.h"
#include "oatpp/web/server/HttpConnectionHandler.hpp"

using namespace items::serviceFramework;

class TestModule : public ServiceModule
{
public:

    TestModule (std::string name) : ServiceModule (name)
    {
    }

    bool Initialise ()
    {
        printf ("Initialise test module...\n");
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
