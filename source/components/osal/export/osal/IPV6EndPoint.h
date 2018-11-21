#pragma once

#include <stdint.h>
#include <streambuf>
#include "osal/NetworkEndPoint.h"
#include <osal/IPV6Address.h>

namespace OSAL {
namespace Network {

struct OSAL_EXPORT sockaddr_in6 : public ::sockaddr_in6
{
    using PortType = uint16_t;

    sockaddr_in6();
    sockaddr_in6(const sockaddr_in6 & other);
    sockaddr_in6(const ::sockaddr_in6 & other);
    sockaddr_in6(const in6_addr & address);
    sockaddr_in6(const in6_addr & address, PortType port);
    sockaddr_in6(const in6_addr & address, PortType port, uint32_t flowinfo, uint32_t scopeid);

    sockaddr_in6 & operator = (const sockaddr_in6 & other);
    sockaddr_in6 & operator = (const ::sockaddr_in6 & other);
    sockaddr_in6 & operator = (const in6_addr & address);

    SocketFamily family() const;
    in6_addr address() const;
    PortType port() const;
    uint32_t flowinfo() const;
    uint32_t scopeid() const;
};

class OSAL_EXPORT IPV6EndPoint : public OSAL::Network::EndPoint
{
public:
    using PortType = uint16_t;
    static const size_t AddressSize = IPV6Address::AddressSize + sizeof(PortType);
    static const PortType AnyPort;

    IPV6EndPoint()
        : _ipAddress()
        , _port(AnyPort)
        , _flowInformation()
        , _scopeIdentifier()

    {
    }
    IPV6EndPoint(const IPV6EndPoint & other)
        : _ipAddress(other._ipAddress)
        , _port(other._port)
        , _flowInformation(other._flowInformation)
        , _scopeIdentifier(other._scopeIdentifier)
    {
    }
    IPV6EndPoint(const OSAL::Network::IPV6Address & ipAddress)
        : _ipAddress(ipAddress)
        , _port(AnyPort)
        , _flowInformation()
        , _scopeIdentifier()
    {
    }
    IPV6EndPoint(const OSAL::Network::IPV6Address & ipAddress, PortType port,
                 uint32_t flowInformation = 0, uint32_t scopeIdentifier = 0)
        : _ipAddress(ipAddress)
        , _port(port)
        , _flowInformation(flowInformation)
        , _scopeIdentifier(scopeIdentifier)
    {
    }
    IPV6EndPoint(const in6_addr & ipAddress)
            : _ipAddress(ipAddress)
            , _port(AnyPort)
            , _flowInformation()
            , _scopeIdentifier()
    {
    }
    IPV6EndPoint(const in6_addr & ipAddress, PortType port,
                 uint32_t flowInformation = 0, uint32_t scopeIdentifier = 0)
            : _ipAddress(ipAddress)
            , _port(port)
            , _flowInformation(flowInformation)
            , _scopeIdentifier(scopeIdentifier)
    {
    }
    IPV6EndPoint(uint8_t ipAddress[16], PortType port,
                 uint32_t flowInformation = 0, uint32_t scopeIdentifier = 0)
        : _ipAddress(ipAddress)
        , _port(port)
        , _flowInformation(flowInformation)
        , _scopeIdentifier(scopeIdentifier)
    {
    }
    IPV6EndPoint(PortType port)
        : _ipAddress(OSAL::Network::IPV6Address::None)
        , _port(port)
        , _flowInformation()
        , _scopeIdentifier()
    {
    }
    IPV6EndPoint(sockaddr_in6 * address)
        : _ipAddress(address->address())
        , _port(address->port())
        , _flowInformation(address->flowinfo())
        , _scopeIdentifier(address->scopeid())
    {
    }

    virtual ~IPV6EndPoint() override;

    static EndPointPtr Create();
    static EndPointPtr Create(const EndPoint & other);
    static EndPointPtr Create(const std::string & text);

    static IPV6EndPoint Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV6EndPoint & ipEndPoint);
    IPV6EndPoint & operator = (const IPV6EndPoint & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const IPV6EndPoint & other) const;
    bool operator != (const IPV6EndPoint & other) const;

    const OSAL::Network::IPV6Address & GetIPAddress() const { return _ipAddress; }
    PortType GetPort() const { return _port; }
    uint32_t GetFlowInfo() const { return _flowInformation; }
    uint32_t GetScopeID() const { return _scopeIdentifier; }

    virtual OSAL::Network::SocketFamily Family() const override { return OSAL::Network::SocketFamily::InternetV6; }
    virtual size_t Size() const override;
    virtual OSAL::bytearray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    OSAL::Network::IPV6Address _ipAddress;
    PortType _port;
    uint32_t _flowInformation;
    uint32_t _scopeIdentifier;
};

} // namespace Network
} // namespace OSAL
