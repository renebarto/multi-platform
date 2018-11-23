#pragma once

#include <stdint.h>
#include <streambuf>
#include "osal/NetworkEndPoint.h"
#include "osal/IPV4Address.h"

namespace OSAL {
namespace Network {

struct OSAL_EXPORT sockaddr_in : public ::sockaddr_in
{
    using PortType = uint16_t;

    sockaddr_in();
    sockaddr_in(const sockaddr_in & other);
    sockaddr_in(const ::sockaddr_in & other);
    sockaddr_in(const in_addr & address);
    sockaddr_in(const in_addr & address, PortType port);

    sockaddr_in & operator = (const sockaddr_in & other);
    sockaddr_in & operator = (const ::sockaddr_in & other);
    sockaddr_in & operator = (const in_addr & address);

    SocketFamily family() const;
    in_addr address() const;
    PortType port() const;
};

class OSAL_EXPORT IPV4EndPoint : public OSAL::Network::EndPoint
{
public:
    using PortType = uint16_t;
    static const size_t AddressSize = IPV4Address::AddressSize + sizeof(PortType);
    static const PortType AnyPort;

    IPV4EndPoint()
        : _ipAddress()
        , _port(AnyPort)
    {
    }
    IPV4EndPoint(const IPV4EndPoint & other)
        : _ipAddress(other._ipAddress)
        , _port(other._port)
    {
    }
    explicit IPV4EndPoint(const OSAL::Network::IPV4Address & ipAddress)
        : _ipAddress(ipAddress)
        , _port(AnyPort)
    {
    }
    IPV4EndPoint(const OSAL::Network::IPV4Address & ipAddress, PortType port)
        : _ipAddress(ipAddress)
        , _port(port)
    {
    }
    explicit IPV4EndPoint(const OSAL::bytearray & ipAddress)
        : _ipAddress(ipAddress)
        , _port(AnyPort)
    {
    }
    IPV4EndPoint(const OSAL::bytearray & ipAddress, PortType port)
        : _ipAddress(ipAddress)
        , _port(port)
    {
    }
    explicit IPV4EndPoint(const in_addr & ipAddress)
        : _ipAddress(ipAddress)
        , _port(AnyPort)
    {
    }
    IPV4EndPoint(const in_addr & ipAddress, PortType port)
        : _ipAddress(ipAddress)
        , _port(port)
    {
    }

    IPV4EndPoint(uint32_t ipAddress, PortType port)
        : _ipAddress(ipAddress)
        , _port(port)
    {
    }
    IPV4EndPoint(PortType port)
        : _ipAddress(OSAL::Network::IPV4Address::None)
        , _port(port)
    {
    }
    IPV4EndPoint(sockaddr_in * address)
        : IPV4EndPoint(address->address(), address->port())
    {
    }

    virtual ~IPV4EndPoint();

    static EndPointPtr Create();
    static EndPointPtr Create(const EndPoint & other);
    static EndPointPtr Create(const std::string & text);

    static IPV4EndPoint Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV4EndPoint & ipEndPoint);
    IPV4EndPoint & operator = (const IPV4EndPoint & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const IPV4EndPoint & other) const;
    bool operator != (const IPV4EndPoint & other) const;

    const OSAL::Network::IPV4Address & GetIPAddress() const { return _ipAddress; }
    PortType GetPort() const { return _port; }

    virtual OSAL::Network::SocketFamily Family() const override { return OSAL::Network::SocketFamily::InternetV4; }
    virtual size_t Size() const override;
    virtual OSAL::bytearray GetBytes() const override;
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & Print(std::basic_ostream<Elem, Traits> & s) const;
    virtual std::basic_ostream<char, std::char_traits<char>> & PrintTo(std::basic_ostream<char, std::char_traits<char>> & s) const override { return Print(s); }
    virtual std::basic_ostream<wchar_t, std::char_traits<wchar_t>> & PrintTo(std::basic_ostream<wchar_t, std::char_traits<wchar_t>> & s) const override { return Print(s); }

private:
    OSAL::Network::IPV4Address _ipAddress;
    PortType _port;
};

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & IPV4EndPoint::Print(std::basic_ostream<Elem, Traits> & s) const
{
    s << _ipAddress << ":";
    s << _port;
    return s;
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s, const IPV4EndPoint & value)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const IPV4EndPoint & value)
{
    return value.PrintTo(s);
}

} // namespace Network
} // namespace OSAL
