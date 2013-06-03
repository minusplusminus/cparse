#include <arg3/net/restclient.h>
#include <cparse/json.h>

using arg3::net::http::Method;
using arg3::net::RESTClient;

namespace cparse
{
    class Client : public RESTClient
    {
    public:
        Client();
        virtual ~Client();
        Client(const Client &c);
        Client(Client &&c);
        Client &operator=(const Client &c);
        Client &operator=(Client &&c);

        JSON getResponseValue() const;
    };
}