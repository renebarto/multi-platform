#include "osal/NetworkAddress.h"

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

} // namespace Network
} // namespace OSAL