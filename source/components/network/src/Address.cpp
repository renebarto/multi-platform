#include "network/Address.h"

using namespace std;
using namespace Network;

ostream & Network::operator << (ostream & stream, Network::SocketFamily socketFamily)
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

inline std::ostream & operator << (std::ostream & stream, SocketFamily value)
{
    switch (value)
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

