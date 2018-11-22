#pragma once

#include <sys/socket.h>
#include <netatalk/at.h>
#include <linux/irda.h>
#include <memory>
#include "osal/exports.h"
#include "osal/bytearray.h"

namespace OSAL {
namespace Network {

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
    AppleTalk = AF_APPLETALK,
    NetBIOS = AF_UNSPEC,
    IrDA = AF_IRDA,
    Bluetooth = AF_UNSPEC,
};

std::ostream & operator << (std::ostream & stream, SocketFamily socketFamily);

class OSAL_EXPORT EndPoint
{
public:
    EndPoint() {}
    virtual ~EndPoint() {}

    virtual SocketFamily Family() const = 0;
    virtual size_t Size() const = 0;
    virtual bytearray GetBytes() const = 0;
    virtual std::ostream & PrintTo(std::ostream & stream) const = 0;
};
using EndPointPtr = std::shared_ptr<EndPoint>;

EndPointPtr OSAL_EXPORT Create(sockaddr * address);

inline void PrintTo(const EndPoint & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, const EndPoint & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Network
} // namespace OSAL

