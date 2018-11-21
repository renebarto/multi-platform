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

struct in_addr : public ::in_addr
{
    in_addr();
    in_addr(const std::initializer_list<uint8_t> & other);
    in_addr(const in_addr & other);
    in_addr(const bytearray & other);
    in_addr(const ::in_addr & other);

    in_addr & operator = (const in_addr & other);
    in_addr & operator = (const bytearray & other);
    in_addr & operator = (const ::in_addr & other);

    in_addr_t value() const;
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
    virtual std::ostream & PrintTo(std::ostream & stream) const;

private:
    OSAL::bytearray _ipAddress;

    void SetData(const OSAL::bytearray & data, size_t offset);
};

inline void PrintTo(const IPV4Address & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, const IPV4Address & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Network
} // namespace OSAL

