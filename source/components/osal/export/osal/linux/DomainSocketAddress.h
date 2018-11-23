#pragma once

#include "osal/Strings.h"
#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

class OSAL_EXPORT DomainSocketAddress : public OSAL::Network::EndPoint
{
public:
    static const size_t AddressSize;
    static const DomainSocketAddress Any;
    static const DomainSocketAddress None;
    static const DomainSocketAddress Broadcast;
    static const DomainSocketAddress LocalHost;

    DomainSocketAddress()
        : _address(None._address)
    {
    }
    DomainSocketAddress(const DomainSocketAddress & other)
        : _address(other._address)
    {
    }
    DomainSocketAddress(const OSAL::bytearray & address)
        : _address(AddressSize)
    {
        SetData(address, 0);
    }
    DomainSocketAddress(const OSAL::bytearray & address, size_t offset)
        : _address(AddressSize)
    {
        SetData(address, offset);
    }
    DomainSocketAddress(const std::string & address)
        : _address(AddressSize)
    {
        SetData(address);
    }

    virtual ~DomainSocketAddress();

    static EndPointPtr Create();
    static EndPointPtr Create(const EndPoint & other);
    static EndPointPtr Create(const std::string & text);

    static DomainSocketAddress Parse(const std::string & text);
    static bool TryParse(const std::string & text, DomainSocketAddress & address);
    DomainSocketAddress & operator = (const DomainSocketAddress & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const DomainSocketAddress & other) const;
    bool operator != (const DomainSocketAddress & other) const;
    uint8_t operator[] (size_t offset) const;

    std::string GetData() const;
    void SetData(const std::string & value);

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::Unix; }
    virtual size_t Size() const override { return AddressSize; }
    virtual OSAL::bytearray GetBytes() const override;
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const;

private:
    OSAL::bytearray _address;

    void SetData(const OSAL::bytearray & data, size_t offset);
};

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & DomainSocketAddress::PrintTo(std::basic_ostream<Elem, Traits> & s) const
{
    s << reinterpret_cast<const char *>(_address.data());
    return s;
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s, const DomainSocketAddress & value)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const DomainSocketAddress & value)
{
    return value.PrintTo(s);
}

} // namespace Network
} // namespace OSAL
