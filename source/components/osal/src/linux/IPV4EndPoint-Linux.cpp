#include "osal/IPV4EndPoint.h"

#include <sstream>
#include <netinet/in.h>
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;
using namespace Network;

const IPV4EndPoint::PortType IPV4EndPoint::AnyPort = 0;

IPV4EndPoint::~IPV4EndPoint()
{
}

EndPointPtr IPV4EndPoint::Create()
{
    auto result = std::make_shared<IPV4EndPoint>();
    return result;
}

EndPointPtr IPV4EndPoint::Create(const EndPoint &other)
{
    if ((other.Family() != SocketFamily::InternetV4))
        return nullptr;
    const IPV4EndPoint * pOther = dynamic_cast<const IPV4EndPoint *>(&other);
    auto result = std::make_shared<IPV4EndPoint>(*pOther);
    return result;
}

EndPointPtr IPV4EndPoint::Create(const std::string &text)
{
    auto result = std::make_shared<IPV4EndPoint>();
    if (!TryParse(text, *result))
    {
        return nullptr;
    }
    return result;
}

IPV4EndPoint IPV4EndPoint::Parse(const string & text)
{
    IPV4EndPoint ipAddress;
    if (!TryParse(text, ipAddress))
    {
        ostringstream stream;
        stream << "IPV4EndPoint string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

bool IPV4EndPoint::TryParse(const string & text, IPV4EndPoint & ipEndPoint)
{
    size_t pos = text.find(':');
    OSAL::Network::IPV4Address address = OSAL::Network::IPV4Address::None;
    long port = AnyPort;
    if (pos == string::npos)
    {
        if (!OSAL::Network::IPV4Address::TryParse(text, address))
            return false;
    }
    else
    {
        if (!OSAL::Network::IPV4Address::TryParse(text.substr(0, pos), address))
            return false;
        port = strtol(text.substr(pos + 1).c_str(), 0, 10);
        if ((port < 0) || (port > 65535))
            return false;
    }
    ipEndPoint = IPV4EndPoint(address, static_cast<uint16_t>(port));
    return true;
}

IPV4EndPoint & IPV4EndPoint::operator = (const IPV4EndPoint & other)
{
    _ipAddress = other._ipAddress;
    _port = other._port;
    return *this;
}

bool IPV4EndPoint::operator == (const EndPoint & other) const
{
    if (&other == this)
        return true;
    if (other.Family() != SocketFamily::InternetV4)
        return false;
    const IPV4EndPoint * otherAsMACAddress = dynamic_cast<const IPV4EndPoint *>(&other);
    return (otherAsMACAddress->_ipAddress == _ipAddress);
}

bool IPV4EndPoint::operator != (const EndPoint & other) const
{
    return ! this->operator ==(other);
}

bool IPV4EndPoint::operator == (const IPV4EndPoint & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress) && (other._port == _port);
}

bool IPV4EndPoint::operator != (const IPV4EndPoint & other) const
{
    return ! this->operator ==(other);
}

size_t IPV4EndPoint::Size() const
{
    return sizeof(sockaddr_in);
}

OSAL::ByteArray IPV4EndPoint::GetBytes() const
{
    OSAL::ByteArray result;
    sockaddr_in address {};
    address.sin_family = AF_INET;
    uint32_t ipAddress = _ipAddress.GetUInt32();
    memcpy(&(address.sin_addr.s_addr), &ipAddress, sizeof(ipAddress));
    address.sin_port = _port;
    result.Set(0, reinterpret_cast<const uint8_t *>(&address), sizeof(address));
    return result;
}

std::ostream & IPV4EndPoint::PrintTo(std::ostream & stream) const
{
    stream << _ipAddress << ":";
    stream << _port;
    return stream;
}
