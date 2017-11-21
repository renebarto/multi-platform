#pragma once

#include "osal/NetworkAddress.h"

namespace OSAL {
namespace Network {

class IPV6Address : public OSAL::Network::Address
{
public:
    static IPV6Address Any;
    static IPV6Address None;
    static IPV6Address Broadcast;
    static IPV6Address LocalHost;

    IPV6Address() :
        _ipAddress(None._ipAddress)
    {
    }
    IPV6Address(const IPV6Address & other) :
        _ipAddress(other._ipAddress)
    {
    }
    IPV6Address(const OSAL::ByteArray & ipAddress) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    IPV6Address(uint8_t ipAddress[16]) :
        _ipAddress(AddressSize)
    {
        SetData(OSAL::ByteArray(ipAddress, 16), 0);
    }
    IPV6Address(const OSAL::ByteArray & ipAddress, size_t offset) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }

    virtual ~IPV6Address();
    static IPV6Address Parse(const OSAL::String & text);
    static bool TryParse(const OSAL::String & text, IPV6Address & ipAddress);
    IPV6Address & operator = (const IPV6Address & other);
    bool operator == (const Address & other) const;
    bool operator != (const Address & other) const;
    bool operator == (const IPV6Address & other) const;
    bool operator != (const IPV6Address & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    OSAL::ByteArray GetData() const;
    void SetData(const OSAL::ByteArray & value);

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::InternetV6; }
    virtual size_t Size() const override { return AddressSize; }
    virtual OSAL::ByteArray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    OSAL::ByteArray _ipAddress;
    static const size_t AddressSize = 16;

    void SetData(const OSAL::ByteArray & data, size_t offset);
};

} // namespace Network
} // namespace OSAL
