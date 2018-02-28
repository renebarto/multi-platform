#pragma once

#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

class OSAL_EXPORT IPV6Address
{
public:
    static const size_t AddressSize = 16;
    static const IPV6Address Any;
    static const IPV6Address None;
    static const IPV6Address Broadcast;
    static const IPV6Address LocalHost;

    IPV6Address()
        : _ipAddress(None._ipAddress)
    {
    }
    IPV6Address(const IPV6Address & other)
        : _ipAddress(other._ipAddress)
    {
    }
    IPV6Address(const OSAL::ByteArray & ipAddress)
        : _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    IPV6Address(const OSAL::ByteArray & ipAddress, size_t offset)
        : _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }
    IPV6Address(uint8_t ipAddress[16])
        : _ipAddress(AddressSize)
    {
        SetData(OSAL::ByteArray(ipAddress, 16), 0);
    }

    virtual ~IPV6Address();

    static IPV6Address Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV6Address & ipAddress);
    IPV6Address & operator = (const IPV6Address & other);
    bool operator == (const IPV6Address & other) const;
    bool operator != (const IPV6Address & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    OSAL::ByteArray GetData() const;
    void SetData(const OSAL::ByteArray & value);

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::InternetV6; }
    virtual size_t Size() const { return AddressSize; }
    virtual OSAL::ByteArray GetBytes() const;
    virtual std::ostream & PrintTo(std::ostream & stream) const;

private:
    OSAL::ByteArray _ipAddress;

    void SetData(const OSAL::ByteArray & data, size_t offset);
};

inline void PrintTo(const IPV6Address & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, const IPV6Address & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Network
} // namespace OSAL
