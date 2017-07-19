#pragma once

#include <sys/socket.h>
#include "core/ByteArray.h"

namespace Network
{

enum class SocketFamily
{
    Any = AF_UNSPEC,
    Unix = AF_UNIX,
    Internet = AF_INET,
    InternetV4 = AF_INET,
    InternetV6 = AF_INET6,
    IPX = AF_IPX,
    Netlink = AF_NETLINK,
    Packet = AF_PACKET,
};

class Address
{
public:
    Address() {}
    virtual ~Address() {}

    virtual SocketFamily Family() const = 0;
    virtual size_t Size() const = 0;
    virtual Core::ByteArray GetBytes() const = 0;
    virtual std::string ToString() const = 0;
};

inline void PrintTo(const Address & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Network

