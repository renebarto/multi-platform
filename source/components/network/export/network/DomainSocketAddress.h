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
        _address(None._address)
    {
    }
    DomainSocketAddress(const DomainSocketAddress & other) :
        _address(other._address)
    {
    }
    DomainSocketAddress(const Core::ByteArray & address) :
        _address(AddressSize)
    {
        SetData(address, 0);
    }
    DomainSocketAddress(const OSAL::String & address) :
        _address(AddressSize)
    {
        SetData(address);
    }
    DomainSocketAddress(const Core::ByteArray & address, size_t offset) :
        _address(AddressSize)
    {
        SetData(address, offset);
    }

    virtual ~DomainSocketAddress();
    static DomainSocketAddress Parse(const std::string & text);
    static bool TryParse(const std::string & text, DomainSocketAddress & address);
    DomainSocketAddress & operator = (const DomainSocketAddress & other);
    bool operator == (const DomainSocketAddress & other) const;
    bool operator != (const DomainSocketAddress & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    OSAL::String GetData() const;
    void SetData(const OSAL::String & value);

    virtual SocketFamily Family() const { return SocketFamily::Unix; }
    virtual size_t Size() const override { return AddressSize; }
    virtual Core::ByteArray GetBytes() const override;
    virtual OSAL::String ToString() const override;

private:
    Core::ByteArray _address;
    static const size_t AddressSize = 108;

    void SetData(const Core::ByteArray & data, size_t offset);
};

inline void PrintTo(const DomainSocketAddress & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Network

namespace Core
{

namespace Util
{

bool TryParse(const OSAL::String & text, Network::DomainSocketAddress & address);

}

} // namespace Core

