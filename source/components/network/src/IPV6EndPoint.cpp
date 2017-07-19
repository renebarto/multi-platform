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
    IPV6EndPoint _ipAddress;
    if (!TryParse(text, _ipAddress))
    {
        ostringstream stream;
        stream << "IPV6EndPoint string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return _ipAddress;
}

bool IPV6EndPoint::TryParse(const string & text, IPV6EndPoint & ipEndPoint)
{
    size_t pos = text.find(':');
    IPV6Address address = IPV6Address::None;
    int _port = AnyPort;
    if (pos == string::npos)
    {
        if (!IPV6Address::TryParse(text, address))
            return false;
    }
    else
    {
        if (!IPV6Address::TryParse(text.substr(0, pos), address))
            return false;
        _port = strtol(text.substr(pos + 1).c_str(), 0, 10);
        if ((_port < 0) || (_port > 65535))
            return false;
    }
    ipEndPoint = IPV6EndPoint(address, (uint16_t)_port);
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

string IPV6EndPoint::ToString() const
{
    ostringstream stream;
    stream << _ipAddress.ToString() << ":";
    stream << _port;
    return stream.str();
}
