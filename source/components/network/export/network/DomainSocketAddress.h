#pragma once

#include "network/Address.h"

namespace Network
{

class DomainSocketAddress : public Address
{
public:
    static DomainSocketAddress Any;
    static DomainSocketAddress None;
    static DomainSocketAddress Broadcast;
    static DomainSocketAddress LocalHost;

    DomainSocketAddress() :
        _ipAddress(None._ipAddress)
    {
    }
    DomainSocketAddress(const DomainSocketAddress & other) :
        _ipAddress(other._ipAddress)
    {
    }
    DomainSocketAddress(const Core::ByteArray & ipAddress) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    DomainSocketAddress(uint32_t ipAddress) :
        _ipAddress(AddressSize)
    {
        SetUInt32(ipAddress);
    }
    DomainSocketAddress(const Core::ByteArray & ipAddress, size_t offset) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }

    virtual ~DomainSocketAddress();
    static DomainSocketAddress Parse(const std::string & text);
    static bool TryParse(const std::string & text, DomainSocketAddress & ipAddress);
    DomainSocketAddress & operator = (const DomainSocketAddress & other);
    bool operator == (const DomainSocketAddress & other) const;
    bool operator != (const DomainSocketAddress & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    uint32_t GetUInt32() const;
    void SetUInt32(uint32_t value);

    virtual SocketFamily Family() const { return SocketFamily::Unix; }
    virtual size_t Size() const override { return AddressSize; }
    virtual Core::ByteArray GetBytes() const override;
    virtual std::string ToString() const override;

private:
    Core::ByteArray _ipAddress;
    static const size_t AddressSize = 4;

    void SetData(const Core::ByteArray & data, size_t offset);
};

} // namespace Network

namespace Core
{

namespace Util
{

bool TryParse(const std::string & text, Network::DomainSocketAddress & ipAddress);

}

} // namespace Core

