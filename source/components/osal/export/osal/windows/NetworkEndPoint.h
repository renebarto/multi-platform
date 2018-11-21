#pragma once

#include "osal/OSAL.h"

#include <memory>
#include "osal/exports.h"
#include "osal/bytearray.h"

namespace OSAL {
namespace Network {

enum class SocketFamily
{
    Any = AF_UNSPEC,
    Unix = AF_UNSPEC,
    Internet = AF_INET,
    InternetV4 = AF_INET,
    InternetV6 = AF_INET6,
    IPX = AF_UNSPEC,
    Netlink = AF_UNSPEC,
    Packet = AF_UNSPEC,
};

OSAL_EXPORT std::ostream & operator << (std::ostream & stream, SocketFamily socketFamily);

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

