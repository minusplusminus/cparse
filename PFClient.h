#include <arg3/net/restclient.h>
#include <cparse/PFValue.h>

using arg3::net::http::Method;
using arg3::net::RESTClient;

namespace cparse
{
    class PFClient : public RESTClient
    {
    public:
        PFClient();

        PFValue getResponseValue() const;
    };
}