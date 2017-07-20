#pragma once

#include "network/Address.h"

namespace Network
{

class IPV4Address : public Address
{
public:
    static IPV4Address Any;
    static IPV4Address None;
    static IPV4Address Broadcast;
    static IPV4Address LocalHost;

    IPV4Address() :
        _ipAddress(None._ipAddress)
    {
    }
    IPV4Address(const IPV4Address & other) :
        _ipAddress(other._ipAddress)
    {
    }
    IPV4Address(const Core::ByteArray & ipAddress) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    IPV4Address(uint32_t ipAddress) :
        _ipAddress(AddressSize)
    {
        SetUInt32(ipAddress);
    }
    IPV4Address(const Core::ByteArray & ipAddress, size_t offset) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }

    virtual ~IPV4Address();
    static IPV4Address Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV4Address & ipAddress);
    IPV4Address & operator = (const IPV4Address & other);
    bool operator == (const IPV4Address & other) const;
    bool operator != (const IPV4Address & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    uint32_t GetUInt32() const;
    void SetUInt32(uint32_t value);

    virtual SocketFamily Family() const { return SocketFamily::InternetV4; }
    virtual size_t Size() const override { return AddressSize; }
    virtual Core::ByteArray GetBytes() const override;
    virtual OSAL::String ToString() const override;

private:
    Core::ByteArray _ipAddress;
    static const size_t AddressSize = 4;

    void SetData(const Core::ByteArray & data, size_t offset);
};

inline void PrintTo(const IPV4Address & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Network

namespace Core
{

namespace Util
{

bool TryParse(const OSAL::String & text, Network::IPV4Address & ipAddress);

} // namespace Util

} // namespace Core

