#pragma once

#include "osal/Strings.h"
#include "osal/NetworkAddress.h"

namespace OSAL {
namespace Network {

class DomainSocketAddress : public OSAL::Network::Address
{
public:
    static const size_t AddressSize = 108;
    static DomainSocketAddress Any;
    static DomainSocketAddress None;
    static DomainSocketAddress Broadcast;
    static DomainSocketAddress LocalHost;

    DomainSocketAddress() :
        _address(None._address)
    {
    }
    DomainSocketAddress(const DomainSocketAddress & other) :
        _address(other._address)
    {
    }
    DomainSocketAddress(const OSAL::ByteArray & address) :
        _address(AddressSize)
    {
        SetData(address, 0);
    }
    DomainSocketAddress(const OSAL::String & address) :
        _address(AddressSize)
    {
        SetData(address);
    }
    DomainSocketAddress(const OSAL::ByteArray & address, size_t offset) :
        _address(AddressSize)
    {
        SetData(address, offset);
    }

    virtual ~DomainSocketAddress();
    static DomainSocketAddress Parse(const OSAL::String & text);
    static bool TryParse(const OSAL::String & text, DomainSocketAddress & address);
    DomainSocketAddress & operator = (const DomainSocketAddress & other);
    bool operator == (const Address & other) const;
    bool operator != (const Address & other) const;
    bool operator == (const DomainSocketAddress & other) const;
    bool operator != (const DomainSocketAddress & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    OSAL::String GetData() const;
    void SetData(const OSAL::String & value);

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::Unix; }
    virtual size_t Size() const override { return AddressSize; }
    virtual OSAL::ByteArray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    OSAL::ByteArray _address;

    void SetData(const OSAL::ByteArray & data, size_t offset);
};

} // namespace Network
} // namespace OSAL
