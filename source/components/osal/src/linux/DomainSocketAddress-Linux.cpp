#include "osal/linux/DomainSocketAddress.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sstream>
#include "osal/OSAL.h"

using namespace std;
using namespace OSAL;
using namespace Network;

DomainSocketAddress DomainSocketAddress::None = DomainSocketAddress(_(""));
DomainSocketAddress DomainSocketAddress::Any = DomainSocketAddress(_(""));
DomainSocketAddress DomainSocketAddress::Broadcast = DomainSocketAddress(_(""));
DomainSocketAddress DomainSocketAddress::LocalHost = DomainSocketAddress(_(""));

DomainSocketAddress::~DomainSocketAddress()
{
}

DomainSocketAddress DomainSocketAddress::Parse(const OSAL::String & text)
{
    DomainSocketAddress address;
    if (!TryParse(text, address))
    {
        basic_ostringstream<OSAL::Char> stream;
        stream << _("DomainSocketAddress string representation must be a UNIX path of no more than ")
               << AddressSize << _("bytes, string is ") << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, _("text"), stream.str());
    }
    address._address.Set(0, reinterpret_cast<const uint8_t *>(text.c_str()), text.size());
    return address;
}

bool DomainSocketAddress::TryParse(const OSAL::String & text, DomainSocketAddress & address)
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

bool DomainSocketAddress::operator == (const Address & other) const
{
    if (&other == this)
        return true;
    if (other.Family() != SocketFamily::Unix)
        return false;
    const DomainSocketAddress * otherAsDomainSocketAddress = dynamic_cast<const DomainSocketAddress *>(&other);
    return (otherAsDomainSocketAddress->_address == _address);
}

bool DomainSocketAddress::operator != (const Address & other) const
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

OSAL::ByteArray DomainSocketAddress::GetBytes() const
{
    return _address;
}

OSAL::String DomainSocketAddress::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;
    stream << reinterpret_cast<const char *>(_address.Data());
    return stream.str();
}

void DomainSocketAddress::SetData(const OSAL::ByteArray & data, size_t offset)
{
    assert(data.Size() - offset <= AddressSize);
    _address.Set(0, data.Data() + offset, data.Size() - offset);
}
