#include <arg3/net/restclient.h>
#include <cparse/value.h>

using arg3::net::http::Method;
using arg3::net::RESTClient;

namespace cparse
{
    class Client : public RESTClient
    {
    public:
        Client();

        Value getResponseValue() const;
    };
}