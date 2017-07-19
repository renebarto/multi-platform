#include "network/IPV6Address.h"
#include <netdb.h>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "core/Core.h"

using namespace std;
using namespace Network;

IPV6Address IPV6Address::None = IPV6Address({0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, });
IPV6Address IPV6Address::Any = IPV6Address({0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, });
IPV6Address IPV6Address::Broadcast = IPV6Address({255, 255, 255, 255, 255, 255, 255, 255,
                                                  255, 255, 255, 255, 255, 255, 255, 255, });
IPV6Address IPV6Address::LocalHost = IPV6Address({0, 0, 0, 0, 0, 0, 0, 0,
                                                  0, 0, 0, 0, 0, 0, 0, 1});

IPV6Address::~IPV6Address()
{
}

IPV6Address IPV6Address::Parse(const string & text)
{
    IPV6Address ipAddress;
    if (!TryParse(text, ipAddress))
    {
        ostringstream stream;
        stream << "IPV6Address string representation must be formatted as xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx " << endl
               << "(or a shortened format), string is " << text;
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

bool IPV6Address::TryParse(const string & text, IPV6Address & ipAddress)
{
    in6_addr address;
    int errorCode = inet_pton(AF_INET6, text.c_str(), &address);
    if (errorCode == 0)
    {
        addrinfo * addressInfo;
        addrinfo hints = { 0, AF_INET6, 0, 0, 0, nullptr, nullptr, nullptr };
        errorCode = getaddrinfo(text.c_str(), 0, &hints, &addressInfo);
        if (errorCode != 0)
            return false;
        address = ((sockaddr_in6 *)(addressInfo[0].ai_addr))->sin6_addr;
        freeaddrinfo(addressInfo);
    }
    ipAddress = IPV6Address(address.__in6_u.__u6_addr8);
    return true;
}

IPV6Address & IPV6Address::operator = (const IPV6Address & other)
{
    _ipAddress = other._ipAddress;
    return *this;
}

bool IPV6Address::operator == (const IPV6Address & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress);
}

bool IPV6Address::operator != (const IPV6Address & other) const
{
    return ! this->operator ==(other);
}

uint8_t & IPV6Address::operator[] (size_t offset)
{
    if (offset < AddressSize)
    {
        return _ipAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

const uint8_t & IPV6Address::operator[] (size_t offset) const
{
    if (offset < AddressSize)
    {
        return _ipAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

Core::ByteArray IPV6Address::GetData() const
{
    return _ipAddress;
}

void IPV6Address::SetData(const Core::ByteArray & value)
{
    _ipAddress.Set(0, value);
}

Core::ByteArray IPV6Address::GetBytes() const
{
    return _ipAddress;
}

string IPV6Address::ToString() const
{
    ostringstream stream;
    stream << hex
           << (int)_ipAddress[0] << setw(2) << setfill('0') << (int)_ipAddress[1] << ":";
    stream << (int)_ipAddress[2] << setw(2) << setfill('0') << (int)_ipAddress[3] << ":";
    stream << (int)_ipAddress[4] << setw(2) << setfill('0') << (int)_ipAddress[5] << ":";
    stream << (int)_ipAddress[6] << setw(2) << setfill('0') << (int)_ipAddress[7] << ":";
    stream << (int)_ipAddress[8] << setw(2) << setfill('0') << (int)_ipAddress[9] << ":";
    stream << (int)_ipAddress[10] << setw(2) << setfill('0') << (int)_ipAddress[11] << ":";
    stream << (int)_ipAddress[12] << setw(2) << setfill('0') << (int)_ipAddress[13] << ":";
    stream << (int)_ipAddress[14] << setw(2) << setfill('0') << (int)_ipAddress[15] << dec;
    return stream.str();
}

void IPV6Address::SetData(const Core::ByteArray & data, size_t offset)
{
    if (offset + AddressSize > data.Size())
    {
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
    }
    _ipAddress.Set(0, data.Data() + offset, AddressSize);
}

namespace Core
{

namespace Util
{

bool TryParse(const string & text, Network::IPV6Address & ipAddress)
{
    return Network::IPV6Address::TryParse(text, ipAddress);
}

} // namespace Util
} // namespace Core
