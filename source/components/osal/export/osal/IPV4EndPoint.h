#pragma once

#include <stdint.h>
#include <streambuf>
#include "osal/NetworkEndPoint.h"
#include "osal/IPV4Address.h"

namespace OSAL {
namespace Network {

class OSAL_EXPORT IPV4EndPoint : public OSAL::Network::EndPoint
{
public:
    static constexpr uint16_t AnyPort = 0;

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
    IPV4EndPoint(const OSAL::Network::IPV4Address & ipAddress)
        : _ipAddress(ipAddress)
        , _port(AnyPort)
    {
    }
    IPV4EndPoint(const OSAL::Network::IPV4Address & ipAddress, uint16_t port)
        : _ipAddress(ipAddress)
        , _port(port)
    {
    }
    IPV4EndPoint(uint32_t ipAddress, uint16_t port)
        : _ipAddress(ipAddress)
        , _port(port)
    {
    }
    IPV4EndPoint(uint16_t port)
        : _ipAddress(OSAL::Network::IPV4Address::None)
        , _port(port)
    {
    }

    virtual ~IPV4EndPoint();

    static EndPointPtr Create();
    static EndPointPtr Create(EndPointPtr other);
    static EndPointPtr Create(const std::string & text);

    static IPV4EndPoint Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV4EndPoint & ipEndPoint);
    IPV4EndPoint & operator = (const IPV4EndPoint & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const IPV4EndPoint & other) const;
    bool operator != (const IPV4EndPoint & other) const;

    const OSAL::Network::IPV4Address & GetIPAddress() const { return _ipAddress; }
    uint16_t GetPort() const { return _port; }

    virtual OSAL::Network::SocketFamily Family() const override { return OSAL::Network::SocketFamily::InternetV4; }
    virtual size_t Size() const override;
    virtual OSAL::ByteArray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    OSAL::Network::IPV4Address _ipAddress;
    uint16_t _port;
};

} // namespace Network
} // namespace OSAL
