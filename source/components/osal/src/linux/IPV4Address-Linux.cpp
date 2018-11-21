#include "osal/IPV4Address.h"

#include <netdb.h>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "osal/Assert.h"
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;
using namespace Network;

IPV4Address const IPV4Address::None = IPV4Address({0, 0, 0, 0});
IPV4Address const IPV4Address::Any = IPV4Address({0, 0, 0, 0});
IPV4Address const IPV4Address::Broadcast = IPV4Address({255, 255, 255, 255});
IPV4Address const IPV4Address::LocalHost = IPV4Address({127, 0, 0, 1});

IPV4Address::IPV4Address(const in_addr & address)
    : _ipAddress(AddressSize)
{
    SetUInt32(address.s_addr);
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

std::ostream & IPV4Address::PrintTo(std::ostream & stream) const
{
    stream << (int)_ipAddress[size_t{0}] << ".";
    stream << (int)_ipAddress[size_t{1}] << ".";
    stream << (int)_ipAddress[size_t{2}] << ".";
    stream << (int)_ipAddress[size_t{3}];
    return stream;
}

void IPV4Address::SetData(const OSAL::bytearray & data, size_t offset)
{
    ASSERT(offset + AddressSize <= data.size());
    _ipAddress.set(0, data.data() + offset, AddressSize);
}
