#pragma once

#include "network/Address.h"

namespace Network
{

class IPV6Address : public Address
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
    IPV6Address(const Core::ByteArray & ipAddress) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    IPV6Address(uint8_t ipAddress[16]) :
        _ipAddress(AddressSize)
    {
        SetData(Core::ByteArray(ipAddress, 16), 0);
    }
    IPV6Address(const Core::ByteArray & ipAddress, size_t offset) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }

    virtual ~IPV6Address();
    static IPV6Address Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV6Address & ipAddress);
    IPV6Address & operator = (const IPV6Address & other);
    bool operator == (const IPV6Address & other) const;
    bool operator != (const IPV6Address & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    Core::ByteArray GetData() const;
    void SetData(const Core::ByteArray & value);

    virtual SocketFamily Family() const { return SocketFamily::InternetV6; }
    virtual size_t Size() const override { return AddressSize; }
    virtual Core::ByteArray GetBytes() const override;
    virtual std::string ToString() const override;

private:
    Core::ByteArray _ipAddress;
    static const size_t AddressSize = 16;

    void SetData(const Core::ByteArray & data, size_t offset);
};

} // namespace Network

namespace Core
{

namespace Util
{

bool TryParse(const std::string & text, Network::IPV6Address & ipAddress);

}

} // namespace Core

