#include "network/DomainSocketAddress.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sstream>
#include "core/Core.h"

using namespace std;
using namespace Network;

DomainSocketAddress DomainSocketAddress::None = DomainSocketAddress("");
DomainSocketAddress DomainSocketAddress::Any = DomainSocketAddress("");
DomainSocketAddress DomainSocketAddress::Broadcast = DomainSocketAddress("");
DomainSocketAddress DomainSocketAddress::LocalHost = DomainSocketAddress("");

DomainSocketAddress::~DomainSocketAddress()
{
}

DomainSocketAddress DomainSocketAddress::Parse(const string & text)
{
    DomainSocketAddress address;
    if (!TryParse(text, address))
    {
        ostringstream stream;
        stream << "DomainSocketAddress string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return address;
}

bool DomainSocketAddress::TryParse(const string & text, DomainSocketAddress & address)
{
    in_addr inAddress;
    const char * path = "";
    int errorCode = inet_pton(AF_UNIX, text.c_str(), &inAddress);
    if (errorCode == 0)
    {
        addrinfo * addressInfo;
        addrinfo hints = { 0, AF_UNIX, 0, 0, 0, nullptr, nullptr, nullptr };
        errorCode = getaddrinfo(text.c_str(), nullptr, &hints, &addressInfo);
        if (errorCode != 0)
            return false;
        path = ((sockaddr_un *)(addressInfo[0].ai_addr))->sun_path;
        freeaddrinfo(addressInfo);
    }
    address = DomainSocketAddress(path);
    return true;
}

DomainSocketAddress & DomainSocketAddress::operator = (const DomainSocketAddress & other)
{
    _address = other._address;
    return *this;
}

bool DomainSocketAddress::operator == (const DomainSocketAddress & other) const
{
    if (&other == this)
        return true;
    return (other._address == _address);
}

bool DomainSocketAddress::operator != (const DomainSocketAddress & other) const
{
    return ! this->operator ==(other);
}

uint8_t & DomainSocketAddress::operator[] (size_t offset)
{
    assert(offset < AddressSize);
    return _address[offset];
}

const uint8_t & DomainSocketAddress::operator[] (size_t offset) const
{
    assert(offset < AddressSize);
    return _address[offset];
}

OSAL::String DomainSocketAddress::GetData() const
{
    return ToString();
}

void DomainSocketAddress::SetData(const OSAL::String & value)
{
    _address.Size(AddressSize);
    _address.Set(0, reinterpret_cast<const uint8_t *>(value.c_str()), value.length());
}

Core::ByteArray DomainSocketAddress::GetBytes() const
{
    return _address;
}

OSAL::String DomainSocketAddress::ToString() const
{
    ostringstream stream;
    stream << reinterpret_cast<char*>(_address.Data(), _address.Size());
    return stream.str();
}

void DomainSocketAddress::SetData(const Core::ByteArray & data, size_t offset)
{
    assert(offset + AddressSize <= data.Size());
    _address.Set(0, data.Data() + offset, AddressSize);
}

namespace Core
{

namespace Util
{

bool TryParse(const string & text, Network::DomainSocketAddress & address)
{
    return Network::DomainSocketAddress::TryParse(text, address);
}

} // namespace Util
} // namespace Core
