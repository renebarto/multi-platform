#include "osal/IPV4Address.h"
#include <netdb.h>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "osal/OSAL.h"

using namespace std;
using namespace OSAL;
using namespace Network;

IPV4Address IPV4Address::None = IPV4Address({0, 0, 0, 0});
IPV4Address IPV4Address::Any = IPV4Address({0, 0, 0, 0});
IPV4Address IPV4Address::Broadcast = IPV4Address({255, 255, 255, 255});
IPV4Address IPV4Address::LocalHost = IPV4Address({127, 0, 0, 1});

IPV4Address::~IPV4Address()
{
}

IPV4Address IPV4Address::Parse(const OSAL::String & text)
{
    IPV4Address ipAddress;
    if (!TryParse(text, ipAddress))
    {
        basic_ostringstream<OSAL::Char> stream;
        stream << _("IPV4Address string representation must be formatted as ddd.ddd.ddd.ddd, string is ") << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, _("text"), stream.str());
    }
    return ipAddress;
}

bool IPV4Address::TryParse(const OSAL::String & text, IPV4Address & ipAddress)
{
    in_addr address;
    string narrowText = ToNarrowString(text);
    int errorCode = inet_pton(AF_INET, narrowText.c_str(), &address);
    if (errorCode == 0)
    {
        addrinfo * addressInfo;
        addrinfo hints = { 0, AF_INET, 0, 0, 0, nullptr, nullptr, nullptr };
        errorCode = getaddrinfo(text.c_str(), nullptr, &hints, &addressInfo);
        if (errorCode != 0)
            return false;
        address = ((sockaddr_in *)(addressInfo->ai_addr))->sin_addr;
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

bool IPV4Address::operator == (const Address & other) const
{
    if (&other == this)
        return true;
    if (other.Family() != SocketFamily::InternetV4)
        return false;
    const IPV4Address * otherAsIPV4Address = dynamic_cast<const IPV4Address *>(&other);
    return (otherAsIPV4Address->_ipAddress == _ipAddress);
}

bool IPV4Address::operator != (const Address & other) const
{
    return ! this->operator ==(other);
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
    assert(offset < AddressSize);
    return _ipAddress[offset];
}

const uint8_t & IPV4Address::operator[] (size_t offset) const
{
    assert(offset < AddressSize);
    return _ipAddress[offset];
}

uint32_t IPV4Address::GetUInt32() const
{
    return _ipAddress.GetUInt32(0);
}

void IPV4Address::SetUInt32(uint32_t value)
{
    _ipAddress.SetUInt32(0, value);
}

OSAL::ByteArray IPV4Address::GetBytes() const
{
    return _ipAddress;
}

std::ostream & IPV4Address::PrintTo(std::ostream & stream) const
{
    stream << (int)_ipAddress[size_t{0}] << _(".");
    stream << (int)_ipAddress[size_t{1}] << _(".");
    stream << (int)_ipAddress[size_t{2}] << _(".");
    stream << (int)_ipAddress[size_t{3}];
    return stream;
}

void IPV4Address::SetData(const OSAL::ByteArray & data, size_t offset)
{
    assert(offset + AddressSize <= data.Size());
    _ipAddress.Set(0, data.Data() + offset, AddressSize);
}
