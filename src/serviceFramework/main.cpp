#include <exception>
#include <iostream>
#include "spdlog/spdlog.h"
#include "ServiceContext.h"
#include "LoggerSettings.h"

using namespace items::serviceFramework;

class TestInit : public ServiceInitialiser
{
public:

    TestInit (std::string name) : ServiceInitialiser (name)
    {
    }

    bool CallInitialise ()
    {
        printf ("Hello my little kitty\n");
        return true;
    }
};

int main ()
{
    SectionsMap CONFIGURATION_LAYOUT_MAP =
    {
        {LOGGING_SECTION, LoggerSettings }
    };

    TestInit testInit = TestInit ("Test Init");

    auto context = new ServiceContext ("Test Application");

    context->AddInitialiser (&testInit);

    context->Initialise (&CONFIGURATION_LAYOUT_MAP, "test.ini");
    spdlog::get ("loggername")->info ("Test something here....");

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

    //return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
