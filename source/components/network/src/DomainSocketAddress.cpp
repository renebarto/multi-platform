#include "network/DomainSocketAddress.h"
#include <netdb.h>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "core/Core.h"

using namespace std;
using namespace Network;

DomainSocketAddress DomainSocketAddress::None = DomainSocketAddress({0, 0, 0, 0});
DomainSocketAddress DomainSocketAddress::Any = DomainSocketAddress({0, 0, 0, 0});
DomainSocketAddress DomainSocketAddress::Broadcast = DomainSocketAddress({255, 255, 255, 255});
DomainSocketAddress DomainSocketAddress::LocalHost = DomainSocketAddress({127, 0, 0, 1});

DomainSocketAddress::~DomainSocketAddress()
{
}

DomainSocketAddress DomainSocketAddress::Parse(const string & text)
{
    DomainSocketAddress ipAddress;
    if (!TryParse(text, ipAddress))
    {
        ostringstream stream;
        stream << "DomainSocketAddress string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

bool DomainSocketAddress::TryParse(const string & text, DomainSocketAddress & ipAddress)
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
        ipAddress = DomainSocketAddress((uint32_t)address.s_addr);
        freeaddrinfo(addressInfo);
    }
    ipAddress = DomainSocketAddress((uint32_t)address.s_addr);
    return true;
}

DomainSocketAddress & DomainSocketAddress::operator = (const DomainSocketAddress & other)
{
    _ipAddress = other._ipAddress;
    return *this;
}

bool DomainSocketAddress::operator == (const DomainSocketAddress & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress);
}

bool DomainSocketAddress::operator != (const DomainSocketAddress & other) const
{
    return ! this->operator ==(other);
}

uint8_t & DomainSocketAddress::operator[] (size_t offset)
{
    if (offset < AddressSize)
    {
        return _ipAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

const uint8_t & DomainSocketAddress::operator[] (size_t offset) const
{
    if (offset < AddressSize)
    {
        return _ipAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

uint32_t DomainSocketAddress::GetUInt32() const
{
    return _ipAddress.GetUInt32(0);
}

void DomainSocketAddress::SetUInt32(uint32_t value)
{
    _ipAddress.SetUInt32(0, value);
}

Core::ByteArray DomainSocketAddress::GetBytes() const
{
    return _ipAddress;
}

string DomainSocketAddress::ToString() const
{
    ostringstream stream;
    stream << (int)_ipAddress[0] << ".";
    stream << (int)_ipAddress[1] << ".";
    stream << (int)_ipAddress[2] << ".";
    stream << (int)_ipAddress[3];
    return stream.str();
}

void DomainSocketAddress::SetData(const Core::ByteArray & data, size_t offset)
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

bool TryParse(const string & text, Network::DomainSocketAddress & ipAddress)
{
    return Network::DomainSocketAddress::TryParse(text, ipAddress);
}

} // namespace Util
} // namespace Core
