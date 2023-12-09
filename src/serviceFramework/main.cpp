#include <exception>
#include <iostream>
#include "spdlog/spdlog.h"
#include "ServiceContext.h"
#include "LoggerSettings.h"

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
        context->AddServiceProvider ("localhost", 8008, SERVICENETWORKTYPE_IPV4);
        context->AddServiceProvider ("localhost", 8099, SERVICENETWORKTYPE_IPV4);
    }
    catch (std::invalid_argument e)
    {
        std::cout << "Exception : " << e.what () << std::endl;
    }

    context->Execute ();

    return EXIT_SUCCESS;
}
