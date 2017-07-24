#include "network/IPV4EndPoint.h"
#include <sstream>
#include "core/Core.h"

using namespace std;
using namespace Network;

IPV4EndPoint::~IPV4EndPoint()
{
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
    IPV4Address address = IPV4Address::None;
    long port = AnyPort;
    if (pos == string::npos)
    {
        if (!IPV4Address::TryParse(text, address))
            return false;
    }
    else
    {
        if (!IPV4Address::TryParse(text.substr(0, pos), address))
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

string IPV4EndPoint::ToString() const
{
    ostringstream stream;
    stream << _ipAddress.ToString() << ":";
    stream << _port;
    return stream.str();
}
