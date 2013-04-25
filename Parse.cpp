#include <cparse/Parse.h>
#include <string>

using namespace std;

namespace cparse
{

    string cparse_app_id_;
    string cparse_api_key_;
    string cparse_facebook_app_id_;
    bool cparse_offline_messages_;
    bool cparse_error_messages_;

    Parse::Parse()
    {

    }

    Parse::Parse(const Parse &other)
    {

    }

    Parse &Parse::operator=(const Parse &other)
    {
        return *this;
    }

    void Parse::set_application_id(const string &appId)
    {
        cparse_app_id_ = appId;
    }

    void Parse::set_api_key(const string &apiKey)
    {
        cparse_api_key_ = apiKey;
    }

    void Parse::set_facebook_application_id(const string &appId)
    {
        cparse_facebook_app_id_ = appId;
    }

    bool Parse::has_facebook_application_id()
    {
        return !cparse_facebook_app_id_.empty();
    }

    void Parse::offline_messages_enabled(bool value)
    {
        cparse_offline_messages_ = value;
    }

    void Parse::error_messages_enabled(bool value)
    {
        cparse_error_messages_ = value;
    }

}
