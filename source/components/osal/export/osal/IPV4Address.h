#pragma once

#include "osal/osal.h"
#if defined(LINUX) || defined(APPLE)
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <inaddr.h>
#endif
#include "osal/bytearray.h"
#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

struct OSAL_EXPORT in_addr : public ::in_addr
{
    in_addr();
    in_addr(const std::initializer_list<uint8_t> & other);
    in_addr(const in_addr & other);
    in_addr(const bytearray & other);
    in_addr(const ::in_addr & other);

    in_addr & operator = (const in_addr & other);
    in_addr & operator = (const bytearray & other);
    in_addr & operator = (const ::in_addr & other);

    uint32_t value() const;
};

class OSAL_EXPORT IPV4Address
{
public:
    static const size_t AddressSize = 4;
    static const IPV4Address Any;
    static const IPV4Address None;
    static const IPV4Address Broadcast;
    static const IPV4Address LocalHost;

    IPV4Address()
        : _ipAddress(None._ipAddress)
    {
    }
    IPV4Address(const IPV4Address & other)
        : _ipAddress(other._ipAddress)
    {
    }
    explicit IPV4Address(const OSAL::bytearray & ipAddress)
        : _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    explicit IPV4Address(const std::initializer_list<uint8_t> & ipAddress)
        : _ipAddress(AddressSize)
    {
        SetData(OSAL::bytearray(ipAddress), 0);
    }
    IPV4Address(const OSAL::bytearray & ipAddress, size_t offset)
        : _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }
    IPV4Address(uint32_t ipAddress)
        : _ipAddress(AddressSize)
    {
        SetUInt32(ipAddress);
    }
    explicit IPV4Address(const in_addr & address);

    virtual ~IPV4Address();

    static IPV4Address Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV4Address & ipAddress);
    IPV4Address & operator = (const IPV4Address & other);
    bool operator == (const IPV4Address & other) const;
    bool operator != (const IPV4Address & other) const;
    uint8_t & operator[] (size_t offset);

    uint32_t GetUInt32() const;
    void SetUInt32(uint32_t value);

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::InternetV4; }
    virtual size_t Size() const { return AddressSize; }
    virtual OSAL::bytearray GetBytes() const;
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const;

private:
    OSAL::bytearray _ipAddress;

    void SetData(const OSAL::bytearray & data, size_t offset);
};

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & IPV4Address::PrintTo(std::basic_ostream<Elem, Traits> & s) const
{
    s << (int)_ipAddress[size_t{0}] << ".";
    s << (int)_ipAddress[size_t{1}] << ".";
    s << (int)_ipAddress[size_t{2}] << ".";
    s << (int)_ipAddress[size_t{3}];
    return s;
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s, const IPV4Address & value)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const IPV4Address & value)
{
    return value.PrintTo(s);
}

} // namespace Network
} // namespace OSAL

