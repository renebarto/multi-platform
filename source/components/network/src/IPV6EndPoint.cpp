#include "network/IPV6EndPoint.h"
#include <sstream>
#include "core/Core.h"

using namespace std;
using namespace Network;

IPV6EndPoint::~IPV6EndPoint()
{
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

bool IPV6EndPoint::TryParse(const string & text, IPV6EndPoint & ipEndPoint)
{
    OSAL::Network::IPV6Address address = OSAL::Network::IPV6Address::None;
    long port = AnyPort;
    size_t bracketPos = text.find('[');
    if (bracketPos == 0)
    {
        // We have a URI with possibly a port number
        bracketPos = text.find(']');
        if (bracketPos == string::npos)
            return false;
        if (!OSAL::Network::IPV6Address::TryParse(text.substr(1, bracketPos - 1), address))
            return false;
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
        // We have a plain IPV6 address
        if (!OSAL::Network::IPV6Address::TryParse(text, address))
            return false;
    }
    ipEndPoint = IPV6EndPoint(address, static_cast<uint16_t>(port));
    return true;
}

IPV6EndPoint & IPV6EndPoint::operator = (const IPV6EndPoint & other)
{
    _ipAddress = other._ipAddress;
    _port = other._port;
    return *this;
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
    return stream;
}
