#pragma once

#include <memory>
#include "osal/OSAL.h"
#include "osal/exports.h"
#include "osal/Strings.h"
#include "osal/ByteArray.h"

namespace OSAL {
namespace Network {

#define AF_PACKET   17

enum class SocketFamily
{
    Any = AF_UNSPEC,
    Unix = AF_UNIX,
    Internet = AF_INET,
    InternetV4 = AF_INET,
    InternetV6 = AF_INET6,
    IPX = AF_IPX,
    Packet = AF_PACKET,
};

std::ostream & operator << (std::ostream & stream, SocketFamily socketFamily);

class Address
{
public:
    Address() {}
    virtual ~Address() {}

    virtual SocketFamily Family() const = 0;
    virtual size_t Size() const = 0;
    virtual OSAL::ByteArray GetBytes() const = 0;
    virtual std::ostream & PrintTo(std::ostream & stream) const = 0;
};
using AddressPtr = std::shared_ptr<Address>;

inline void PrintTo(const Address & value, std::basic_ostream<OSAL::Char> & stream)
{
    value.PrintTo(stream);
}

} // namespace Network
} // namespace OSAL

