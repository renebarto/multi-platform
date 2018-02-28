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
    IPV4EndPoint(const OSAL::Network::IPV4Address & ipAddress)
        : _ipAddress(ipAddress)
        , _port(AnyPort)
    {
    }
    IPV4EndPoint(const OSAL::Network::IPV4Address & ipAddress, PortType port)
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
    virtual OSAL::ByteArray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    OSAL::Network::IPV4Address _ipAddress;
    PortType _port;
};

} // namespace Network
} // namespace OSAL
