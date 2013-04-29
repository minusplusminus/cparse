#include "PFClient.h"
#include "Protocol.h"
#include <cparse/PFException.h>

namespace cparse
{

    extern string cparse_app_id_;
    extern string cparse_api_key_;

    PFClient::PFClient()
    {
        setSecure(true);

        setHost(protocol::HOST);

        setVersion(protocol::VERSION);

        addHeader(protocol::HEADER_APP_ID, cparse_app_id_);

        addHeader(protocol::HEADER_API_KEY, cparse_api_key_);

        addHeader("Content-Type", "application/json");
    }

    PFValue PFClient::getResponseValue() const
    {
        PFValue obj;

        if (!obj.parse(getResponse()))
            throw PFException("reponse is not a valid response");

        if (obj.contains("error"))
        {
            throw PFException(obj.getString("error"));
        }

        return obj;
    }
}