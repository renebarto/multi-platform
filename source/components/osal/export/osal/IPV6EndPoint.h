#pragma once

#include <stdint.h>
#include <streambuf>
#include "osal/NetworkEndPoint.h"
#include <osal/IPV6Address.h>

namespace OSAL {
namespace Network {

class OSAL_EXPORT IPV6EndPoint : public OSAL::Network::EndPoint
{
public:
    static constexpr uint16_t AnyPort = 0;

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
    IPV6EndPoint(const OSAL::Network::IPV6Address & ipAddress, uint16_t port,
                 uint32_t flowInformation = 0, uint32_t scopeIdentifier = 0)
        : _ipAddress(ipAddress)
        , _port(port)
        , _flowInformation(flowInformation)
        , _scopeIdentifier(scopeIdentifier)
    {
    }
    IPV6EndPoint(uint8_t ipAddress[16], uint16_t port,
                 uint32_t flowInformation = 0, uint32_t scopeIdentifier = 0)
        : _ipAddress(ipAddress)
        , _port(port)
        , _flowInformation(flowInformation)
        , _scopeIdentifier(scopeIdentifier)
    {
    }
    IPV6EndPoint(uint16_t port)
        : _ipAddress(OSAL::Network::IPV6Address::None)
        , _port(port)
        , _flowInformation()
        , _scopeIdentifier()
    {
    }

    virtual ~IPV6EndPoint() override;
    static IPV6EndPoint Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV6EndPoint & ipEndPoint);
    IPV6EndPoint & operator = (const IPV6EndPoint & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const IPV6EndPoint & other) const;
    bool operator != (const IPV6EndPoint & other) const;

    const OSAL::Network::IPV6Address & GetIPAddress() const { return _ipAddress; }
    uint16_t GetPort() const { return _port; }
    uint32_t GetFlowInfo() const { return _flowInformation; }
    uint32_t GetScopeID() const { return _scopeIdentifier; }

    virtual OSAL::Network::SocketFamily Family() const override { return OSAL::Network::SocketFamily::InternetV6; }
    virtual size_t Size() const override;
    virtual OSAL::ByteArray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    OSAL::Network::IPV6Address _ipAddress;
    uint16_t _port;
    uint32_t _flowInformation;
    uint32_t _scopeIdentifier;
};

} // namespace Network
} // namespace OSAL
