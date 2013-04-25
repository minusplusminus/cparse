#include <cparse/PFObject.h>
#include <cparse/Parse.h>
#include <igloo/igloo.h>
#include <fstream>
#include "PFLog.h"

using namespace cparse;
using namespace igloo;
using namespace std;

void die(const char *message)
{
    printf("ERROR: %s\n", message);
    abort();
}

int main(void)
{
    ifstream file("./cparse.test.json");

    if (!file.is_open())
        die("cparse.test.json not found");

    arg3::json::object config(file);

    if (config.contains("parseAppId"))
        Parse::set_application_id(config.getString("parseAppId"));
    else
        die("No app id");

    if (config.contains("parseApiKey"))
        Parse::set_api_key(config.getString("parseApiKey"));
    else
        die("No api key");

    if(config.contains("logLevel"))
    {
        PFLog::level lev = PFLog::lookupLogLevel(config.getString("logLevel"));
        PFLog::info("Setting log level " + std::to_string(lev));
        PFLog::setMinLogLevel(lev);
    }
    else
        die("no log level");

    return TestRunner::RunAllTests();
}
