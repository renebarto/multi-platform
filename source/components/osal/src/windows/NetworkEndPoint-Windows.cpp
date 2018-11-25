#include "osal/NetworkEndPoint.h"
#include "osal/IPV4EndPoint.h"
#include "osal/IPV6EndPoint.h"

using namespace std;

namespace OSAL {
namespace Network {

ostream & operator<<(ostream & stream, SocketFamily socketFamily)
{
    switch (socketFamily)
    {
        case SocketFamily::Unix:
            stream << "UNIX";
            break;
        case SocketFamily::InternetV4:
            stream << "IPV4";
            break;
        case SocketFamily::InternetV6:
            stream << "IPV6";
            break;
        default:
            stream << "Unknown";
            break;
    }
    return stream;
}

EndPointPtr Create(const sockaddr * address)
{
    if (address == nullptr)
        return nullptr;
    switch (static_cast<SocketFamily>(address->sa_family))
    {
        case SocketFamily::InternetV4:
            return make_shared<IPV4EndPoint>(reinterpret_cast<const sockaddr_in *>(address));
        case SocketFamily::InternetV6:
            return make_shared<IPV6EndPoint>(reinterpret_cast<const sockaddr_in6 *>(address));
        default:
            break;
    }
    return nullptr;
}

EndPointPtr Create(const IPV4Address & address, uint16_t port)
{
    return make_shared<IPV4EndPoint>(address, port);
}

EndPointPtr Create(const IPV6Address & address, uint16_t port)
{
    return make_shared<IPV6EndPoint>(address, port);
}

} // namespace Network
} // namespace OSAL