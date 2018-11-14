#include "osal/IPV6EndPoint.h"

#include <sstream>
#include "osal/Exception.h"
#include "osal/Strings.h"

using namespace std;
using namespace OSAL;
using namespace Network;

const IPV6EndPoint::PortType IPV6EndPoint::AnyPort = 0;

IPV6EndPoint::~IPV6EndPoint()
{
}

EndPointPtr IPV6EndPoint::Create()
{
    auto result = std::make_shared<IPV6EndPoint>();
    return result;
}

EndPointPtr IPV6EndPoint::Create(const EndPoint &other)
{
    if ((other.Family() != SocketFamily::InternetV6))
        return nullptr;
    const IPV6EndPoint * pOther = dynamic_cast<const IPV6EndPoint *>(&other);
    auto result = std::make_shared<IPV6EndPoint>(*pOther);
    return result;
}

EndPointPtr IPV6EndPoint::Create(const std::string &text)
{
    auto result = std::make_shared<IPV6EndPoint>();
    if (!TryParse(text, *result))
    {
        return nullptr;
    }
    return result;
}

IPV6EndPoint IPV6EndPoint::Parse(const string & text)
{
    IPV6EndPoint ipAddress;
    if (!TryParse(text, ipAddress))
    {
        ostringstream stream;
        stream << "IPV6EndPoint string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

static long ParseScopeID(const string & text)
{
    long value = 0;
    for (auto ch : text)
    {
        if (!Strings::isdigit(ch))
            break;
        value = value * 10 + (ch - '0');
    }
    return value;
}

bool IPV6EndPoint::TryParse(const string & text, IPV6EndPoint & ipEndPoint)
{
    OSAL::Network::IPV6Address address = OSAL::Network::IPV6Address::None;
    long port = AnyPort;
    long scopeID = 0;
    size_t bracketPos = text.find('[');
    if (bracketPos == 0)
    {
        // We have a URI with possibly a port number
        bracketPos = text.find(']');
        if (bracketPos == string::npos)
            return false;
        size_t scopeDelimiterPos = text.find('%', 1);
        if (scopeDelimiterPos == string::npos)
        {
            if (!OSAL::Network::IPV6Address::TryParse(text.substr(1, bracketPos - 1), address))
                return false;
        }
        else
        {
            if (!OSAL::Network::IPV6Address::TryParse(text.substr(1, scopeDelimiterPos - 1), address))
                return false;
            scopeID = ParseScopeID(text.substr(scopeDelimiterPos + 1, bracketPos));
        }
        size_t pos = text.find(':', bracketPos + 1);
        if (pos != string::npos)
        {
            port = strtol(text.substr(pos + 1).c_str(), 0, 10);
            if ((port < 0) || (port > 65535))
                return false;
        }
    }
    else
    {
        // We have a plain IPV6 address with possibly a scope identifier
        size_t scopeDelimiterPos = text.find('%');
        if (scopeDelimiterPos == string::npos)
        {
            // We have a plain IPV6 address
            if (!OSAL::Network::IPV6Address::TryParse(text, address))
                return false;
        }
        else
        {
            // We have a plain IPV6 address with a scope identifier
            if (!OSAL::Network::IPV6Address::TryParse(text.substr(0, scopeDelimiterPos), address))
                return false;
            scopeID = ParseScopeID(text.substr(scopeDelimiterPos + 1));
        }
    }
    ipEndPoint = IPV6EndPoint(address, static_cast<uint16_t>(port), 0, static_cast<uint32_t>(scopeID));
    return true;
}

IPV6EndPoint & IPV6EndPoint::operator = (const IPV6EndPoint & other)
{
    _ipAddress = other._ipAddress;
    _port = other._port;
    return *this;
}

bool IPV6EndPoint::operator == (const EndPoint & other) const
{
    if (&other == this)
        return true;
    if (other.Family() != SocketFamily::InternetV6)
        return false;
    const IPV6EndPoint * otherAsMACAddress = dynamic_cast<const IPV6EndPoint *>(&other);
    return (otherAsMACAddress->_ipAddress == _ipAddress);
}

bool IPV6EndPoint::operator != (const EndPoint & other) const
{
    return ! this->operator ==(other);
}

bool IPV6EndPoint::operator == (const IPV6EndPoint & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress) && (other._port == _port);
}

bool IPV6EndPoint::operator != (const IPV6EndPoint & other) const
{
    return ! this->operator ==(other);
}

size_t IPV6EndPoint::Size() const
{
    return sizeof(sockaddr_in6);
}

OSAL::ByteArray IPV6EndPoint::GetBytes() const
{
    OSAL::ByteArray result;
    sockaddr_in6 address {};
    address.sin6_family = AF_INET6;
    address.sin6_flowinfo = 0;
    address.sin6_scope_id = 0;
    memcpy(&(address.sin6_addr), _ipAddress.GetData().Data(), _ipAddress.GetData().Size());
    address.sin6_port = _port;
    result.Set(0, reinterpret_cast<const uint8_t *>(&address), sizeof(address));
    return result;
}

std::ostream & IPV6EndPoint::PrintTo(std::ostream & stream) const
{
    if (_port != 0)
    {
        stream << "[" << _ipAddress << "]:";
        stream << _port;
    }
    else
    {
        stream << _ipAddress;
    }
    if (_scopeIdentifier != 0)
    {
        stream << '%' << _scopeIdentifier;
    }
    return stream;
}