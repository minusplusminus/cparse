#include "client.h"
#include "protocol.h"
#include <cparse/exception.h>
#include <cparse/user.h>
#include <cparse/parse.h>
#include <arg3/format/format.h>

namespace cparse
{

    extern string cparse_app_id_;
    extern string cparse_api_key_;

    Client::Client()
    {
        setSecure(true);

        setHost(protocol::HOST);

        setVersion(protocol::VERSION);

        addHeader(protocol::HEADER_APP_ID, cparse_app_id_);

        addHeader(protocol::HEADER_API_KEY, cparse_api_key_);

        addHeader("Content-Type", "application/json");

        User *user = User::currentUser();

        if(user != NULL)
        {
            addHeader(protocol::HEADER_SESSION_TOKEN, user->sessionToken());
        }

        addHeader("User-Agent", format("libcparse-{0}", Parse::VERSION));
    }

    Value Client::getResponseValue() const
    {
        Value obj;

        if (!obj.parse(getResponse()))
            throw Exception("reponse is not a valid response");

        if (obj.contains("error"))
        {
            throw Exception(obj.getString("error"));
        }

        return obj;
    }
}