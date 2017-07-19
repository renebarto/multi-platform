#pragma once

#include <stdint.h>
#include <streambuf>
#include "network/IPV4Address.h"

namespace Network
{

class IPV4EndPoint
{
public:
    static const uint16_t AnyPort = 0;

    IPV4EndPoint() :
        _ipAddress(),
        _port(AnyPort)
    {
    }
    IPV4EndPoint(const IPV4EndPoint & other) :
        _ipAddress(other._ipAddress),
        _port(other._port)
    {
    }
    IPV4EndPoint(const IPV4Address & ipAddress) :
        _ipAddress(ipAddress),
        _port(AnyPort)
    {
    }
    IPV4EndPoint(const IPV4Address & ipAddress, uint16_t port) :
        _ipAddress(ipAddress),
        _port(port)
    {
    }
    IPV4EndPoint(uint32_t ipAddress, uint16_t port) :
        _ipAddress(ipAddress),
        _port(port)
    {
    }
    IPV4EndPoint(uint16_t port) :
        _ipAddress(IPV4Address::None),
        _port(port)
    {
    }

    virtual ~IPV4EndPoint();
    static IPV4EndPoint Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV4EndPoint & ipEndPoint);
    IPV4EndPoint & operator = (const IPV4EndPoint & other);
    bool operator == (const IPV4EndPoint & other) const;
    bool operator != (const IPV4EndPoint & other) const;

    const IPV4Address & GetIPAddress() const { return _ipAddress; }
    uint16_t GetPort() const { return _port; }
    std::string ToString() const;

private:
    IPV4Address _ipAddress;
    uint16_t _port;
};

} // namespace Network
