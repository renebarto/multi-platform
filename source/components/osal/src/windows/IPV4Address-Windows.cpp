#include "osal/IPV4Address.h"

#include <sstream>
#include "osal/Assert.h"
#include "osal/Exception.h"
#include "osal/Strings.h"

using namespace std;
using namespace OSAL;
using namespace Network;

namespace OSAL {
namespace Network {

in_addr::in_addr()
    : ::in_addr()
{
}

in_addr::in_addr(const std::initializer_list<uint8_t> & other)
    : ::in_addr()
{
    ASSERT(other.size() == sizeof(s_addr));
    bytearray address(other);
    memcpy(&s_addr, address.data(), address.size());
}

in_addr::in_addr(const in_addr & other)
    : ::in_addr(other)
{
}

in_addr::in_addr(const bytearray & other)
    : ::in_addr()
{
    ASSERT(other.size() == sizeof(s_addr));
    memcpy(&s_addr, other.data(), other.size());
}

in_addr::in_addr(const ::in_addr & other)
    : ::in_addr(other)
{
}

in_addr & in_addr::operator = (const in_addr & other)
{
    if (&other != this)
    {
        s_addr = other.s_addr;
    }
    return *this;
}

in_addr & in_addr::operator = (const bytearray & other)
{
    ASSERT(other.size() == sizeof(s_addr));
    copy_n(other.data(), other.size(), &s_addr);
    return *this;
}

in_addr & in_addr::operator = (const ::in_addr & other)
{
    s_addr = other.s_addr;
    return *this;
}

uint32_t in_addr::value() const
{
    return s_addr;
}

} // namespace Network
} // namespace OSAL

IPV4Address const IPV4Address::None = IPV4Address({0, 0, 0, 0});
IPV4Address const IPV4Address::Any = IPV4Address({0, 0, 0, 0});
IPV4Address const IPV4Address::Broadcast = IPV4Address({255, 255, 255, 255});
IPV4Address const IPV4Address::LocalHost = IPV4Address({127, 0, 0, 1});
static string LocalHostName = "localhost";

IPV4Address::IPV4Address(const OSAL::Network::in_addr & address)
    : _ipAddress(AddressSize)
{
    SetUInt32(address.value());
}

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
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

bool IPV4Address::TryParse(const string & text, IPV4Address & ipAddress)
{
    in_addr address;
    int errorCode = inet_pton(AF_INET, text.c_str(), &address);
    if (errorCode == 0)
    {
        if (0 == Strings::strcasecmp(text.c_str(), LocalHostName.c_str(), std::min(LocalHostName.length(), text.length())))
        {
            ipAddress = LocalHost;
            return true;
        }
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
    ASSERT(offset < AddressSize);
    return _ipAddress[offset];
}

uint32_t IPV4Address::GetUInt32() const
{
    return _ipAddress.getUInt32(0);
}

void IPV4Address::SetUInt32(uint32_t value)
{
    _ipAddress.setUInt32(0, value);
}

OSAL::bytearray IPV4Address::GetBytes() const
{
    return _ipAddress;
}

void IPV4Address::SetData(const OSAL::bytearray & data, size_t offset)
{
    ASSERT(offset + AddressSize <= data.size());
    _ipAddress.set(0, data.data() + offset, AddressSize);
}
