#include "osal/linux/DomainSocketAddress.h"

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sstream>
#include "osal/Assert.h"
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;
using namespace Network;

const size_t DomainSocketAddress::AddressSize = 108;

const DomainSocketAddress DomainSocketAddress::None = DomainSocketAddress("");
const DomainSocketAddress DomainSocketAddress::Any = DomainSocketAddress("");
const DomainSocketAddress DomainSocketAddress::Broadcast = DomainSocketAddress("");
const DomainSocketAddress DomainSocketAddress::LocalHost = DomainSocketAddress("");

DomainSocketAddress::~DomainSocketAddress()
{
}

EndPointPtr DomainSocketAddress::Create()
{
    auto result = std::make_shared<DomainSocketAddress>();
    return result;
}

EndPointPtr DomainSocketAddress::Create(const EndPoint &other)
{
    if ((other.Family() != SocketFamily::Unix))
        return nullptr;
    const DomainSocketAddress * pOther = dynamic_cast<const DomainSocketAddress *>(&other);
    auto result = std::make_shared<DomainSocketAddress>(*pOther);
    return result;
}

EndPointPtr DomainSocketAddress::Create(const std::string &text)
{
    auto result = std::make_shared<DomainSocketAddress>();
    if (!TryParse(text, *result))
    {
        return nullptr;
    }
    return result;
}

DomainSocketAddress DomainSocketAddress::Parse(const string & text)
{
    DomainSocketAddress address;
    if (!TryParse(text, address))
    {
        basic_ostringstream<char> stream;
        stream << "DomainSocketAddress string representation must be a UNIX path of no more than "
               << AddressSize << "bytes, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    address._address.Set(0, reinterpret_cast<const uint8_t *>(text.c_str()), text.size());
    return address;
}

bool DomainSocketAddress::TryParse(const string & text, DomainSocketAddress & address)
{
    if (text.length() > AddressSize)
    {
        address = DomainSocketAddress();
        return false;
    }
    address = DomainSocketAddress(text);
    return true;
}

DomainSocketAddress & DomainSocketAddress::operator = (const DomainSocketAddress & other)
{
    _address = other._address;
    return *this;
}

bool DomainSocketAddress::operator == (const EndPoint & other) const
{
    if (&other == this)
        return true;
    if (other.Family() != SocketFamily::Unix)
        return false;
    const DomainSocketAddress * otherAsDomainSocketAddress = dynamic_cast<const DomainSocketAddress *>(&other);
    return (otherAsDomainSocketAddress->_address == _address);
}

bool DomainSocketAddress::operator != (const EndPoint & other) const
{
    return ! this->operator ==(other);
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
    ASSERT(offset < AddressSize);
    if (offset >= AddressSize)
        return _address[AddressSize - 1];
    return _address[offset];
}

const uint8_t & DomainSocketAddress::operator[] (size_t offset) const
{
    ASSERT(offset < AddressSize);
    if (offset >= AddressSize)
        return _address[AddressSize - 1];
    return _address[offset];
}

string DomainSocketAddress::GetData() const
{
    basic_ostringstream<char> stream;
    PrintTo(stream);
    return stream.str();
}

void DomainSocketAddress::SetData(const string & value)
{
    _address.Size(AddressSize);
    _address.Set(0, reinterpret_cast<const uint8_t *>(value.c_str()), value.length());
}

OSAL::ByteArray DomainSocketAddress::GetBytes() const
{
    return _address;
}

std::ostream & DomainSocketAddress::PrintTo(std::ostream & stream) const
{
    stream << reinterpret_cast<const char *>(_address.Data());
    return stream;
}

void DomainSocketAddress::SetData(const OSAL::ByteArray & data, size_t offset)
{
    ASSERT(data.Size() - offset <= AddressSize);
    size_t safeOffset = std::min(AddressSize, offset);
    size_t safeLength = std::min(AddressSize, data.Size() - offset);
    _address.Set(0, data.Data() + safeOffset, safeLength);
}
