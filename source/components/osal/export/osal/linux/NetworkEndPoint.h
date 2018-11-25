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
    virtual std::basic_ostream<char, std::char_traits<char>> & PrintTo(std::basic_ostream<char, std::char_traits<char>> & s) const = 0;
    virtual std::basic_ostream<wchar_t, std::char_traits<wchar_t>> & PrintTo(std::basic_ostream<wchar_t, std::char_traits<wchar_t>> & s) const = 0;
};
using EndPointPtr = std::shared_ptr<EndPoint>;

class IPV4Address;
class IPV6Address;

EndPointPtr OSAL_EXPORT Create(const sockaddr * address);
EndPointPtr OSAL_EXPORT Create(const IPV4Address & address, uint16_t port);
EndPointPtr OSAL_EXPORT Create(const IPV6Address & address, uint16_t port);

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const EndPoint & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const EndPoint & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Network
} // namespace OSAL

