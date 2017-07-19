#include "network/IPV4Address.h"
#include <netdb.h>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "core/Core.h"

using namespace std;
using namespace Network;

IPV4Address IPV4Address::None = IPV4Address({0, 0, 0, 0});
IPV4Address IPV4Address::Any = IPV4Address({0, 0, 0, 0});
IPV4Address IPV4Address::Broadcast = IPV4Address({255, 255, 255, 255});
IPV4Address IPV4Address::LocalHost = IPV4Address({127, 0, 0, 1});

IPV4Address::~IPV4Address()
{
}

IPV4Address IPV4Address::Parse(const string & text)
{
    IPV4Address ipAddress;
    if (!TryParse(text, ipAddress))
    {
        ostringstream stream;
        stream << "IPV4Address string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

bool IPV4Address::TryParse(const string & text, IPV4Address & ipAddress)
{
    in_addr address;
    int errorCode = inet_aton(text.c_str(), &address);
    if (errorCode == 0)
    {
        addrinfo * addressInfo;
        addrinfo hints = { 0, AF_INET, 0, 0, 0, nullptr, nullptr, nullptr };
        errorCode = getaddrinfo(text.c_str(), 0, &hints, &addressInfo);
        if (errorCode != 0)
            return false;
        address = ((sockaddr_in *)(addressInfo[0].ai_addr))->sin_addr;
        freeaddrinfo(addressInfo);
    }
    ipAddress = IPV4Address((uint32_t)address.s_addr);
    return true;
}

IPV4Address & IPV4Address::operator = (const IPV4Address & other)
{
    _ipAddress = other._ipAddress;
    return *this;
}

bool IPV4Address::operator == (const IPV4Address & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress);
}

bool IPV4Address::operator != (const IPV4Address & other) const
{
    return ! this->operator ==(other);
}

uint8_t & IPV4Address::operator[] (size_t offset)
{
    if (offset < AddressSize)
    {
        return _ipAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

const uint8_t & IPV4Address::operator[] (size_t offset) const
{
    if (offset < AddressSize)
    {
        return _ipAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

uint32_t IPV4Address::GetUInt32() const
{
    return _ipAddress.GetUInt32(0);
}

void IPV4Address::SetUInt32(uint32_t value)
{
    _ipAddress.SetUInt32(0, value);
}

Core::ByteArray IPV4Address::GetBytes() const
{
    return _ipAddress;
}

string IPV4Address::ToString() const
{
    ostringstream stream;
    stream << (int)_ipAddress[0] << ".";
    stream << (int)_ipAddress[1] << ".";
    stream << (int)_ipAddress[2] << ".";
    stream << (int)_ipAddress[3];
    return stream.str();
}

void IPV4Address::SetData(const Core::ByteArray & data, size_t offset)
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

bool TryParse(const string & text, Network::IPV4Address & ipAddress)
{
    return Network::IPV4Address::TryParse(text, ipAddress);
}

} // namespace Util
} // namespace Core
