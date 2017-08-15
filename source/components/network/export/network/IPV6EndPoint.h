#pragma once

#include <stdint.h>
#include <streambuf>
#include <osal/IPV6Address.h>

namespace Network
{

class IPV6EndPoint
{
public:
    static const uint16_t AnyPort = 0;

    IPV6EndPoint() :
        _ipAddress(),
        _port(AnyPort)
    {
    }
    IPV6EndPoint(const IPV6EndPoint & other) :
        _ipAddress(other._ipAddress),
        _port(other._port)
    {
    }
    IPV6EndPoint(const OSAL::Network::IPV6Address & ipAddress) :
        _ipAddress(ipAddress),
        _port(AnyPort)
    {
    }
    IPV6EndPoint(const OSAL::Network::IPV6Address & ipAddress, uint16_t port) :
        _ipAddress(ipAddress),
        _port(port)
    {
    }
    IPV6EndPoint(uint8_t ipAddress[16], uint16_t port) :
        _ipAddress(ipAddress),
        _port(port)
    {
    }
    IPV6EndPoint(uint16_t port) :
        _ipAddress(OSAL::Network::IPV6Address::None),
        _port(port)
    {
    }

    virtual ~IPV6EndPoint();
    static IPV6EndPoint Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV6EndPoint & ipEndPoint);
    IPV6EndPoint & operator = (const IPV6EndPoint & other);
    bool operator == (const IPV6EndPoint & other) const;
    bool operator != (const IPV6EndPoint & other) const;

    const OSAL::Network::IPV6Address & GetIPAddress() const { return _ipAddress; }
    uint16_t GetPort() const { return _port; }
    OSAL::String ToString() const;

private:
    OSAL::Network::IPV6Address _ipAddress;
    uint16_t _port;
};

} // namespace Network
