#ifndef CPARSE_PARSE_H
#define CPARSE_PARSE_H

#include <string>

namespace cparse
{
    class Parse
    {
    public:
        static void set_application_id(const std::string &appId);

        static void set_api_key(const std::string &apiKey);

        static void set_facebook_application_id(const std::string &appId);

        static bool has_facebook_application_id();

        static void offline_messages_enabled(bool value);

        static void error_messages_enabled(bool value);
    private:
        Parse();
        Parse(const Parse &other);
        Parse &operator=(const Parse &other);
    };

}

#endif
