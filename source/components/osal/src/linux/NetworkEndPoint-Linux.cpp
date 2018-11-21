#include "osal/NetworkEndPoint.h"

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
        case SocketFamily::IPX:
            stream << "IPX";
            break;
        case SocketFamily::Netlink:
            stream << "Netlink";
            break;
        case SocketFamily::Packet:
            stream << "Packet";
            break;
        default:
            stream << "Unknown";
            break;
    }
    return stream;
}

EndPointPtr Create(sockaddr * address)
{
    if (address == nullptr)
        return nullptr;
    switch (static_cast<SocketFamily>(address->sa_family))
    {
        // TODO: Add other network address types
        case SocketFamily ::InternetV4:
            return make_shared<IPV4EndPoint>(reinterpret_cast<sockaddr_in *>(address));
        case SocketFamily ::InternetV6:
            return make_shared<IPV6EndPoint>(reinterpret_cast<sockaddr_in6 *>(address));
        default:
            break;
    }
    return nullptr;
}

} // namespace Network
} // namespace OSAL