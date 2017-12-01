#include "osal/MACAddress.h"
#include <sstream>
#include "osal/OSAL.h"

using namespace std;
using namespace OSAL;
using namespace Network;

MACAddress::~MACAddress()
{
}

MACAddress MACAddress::Parse(const OSAL::String & text)
{
    MACAddress macAddress;
    if (!TryParse(text, macAddress))
    {
        basic_ostringstream<OSAL::Char> stream;
        stream << _("MACAddress string representation must be formatted as xx-xx-xx-xx-xx-xx, string is ") << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, _("text"), stream.str());
    }
    return macAddress;
}

bool MACAddress::TryParse(const OSAL::String & text, uint8_t & value)
{
    if (text.length() != 2)
        return false;
    int extractedValue;
    if (sscanf(ToNarrowString(text).c_str(), "%02x", &extractedValue) != 1)
        return false;
    value = (uint8_t)extractedValue;

    return true;
}

bool MACAddress::TryParse(const OSAL::String & text, MACAddress & macAddress)
{
    OSAL::String str = text;
    const OSAL::Char delimiter = _('-');
    MACAddress result;

    size_t pos = 0;
    OSAL::String token;
    uint8_t value;
    size_t index = 0;
    while (((pos = str.find(delimiter)) != OSAL::String::npos) && (index < AddressSize))
    {
        token = str.substr(0, pos);
        if (!TryParse(token, value))
            return false;
        result._macAddress[index++] = value;
        str.erase(0, pos + 1);
    }
    if ((index >= AddressSize))
        return false;
    if (!TryParse(str, value))
        return false;
    result._macAddress[index++] = value;
    if (index == AddressSize)
    {
        macAddress = result;
        return true;
    }
    return false;
}

MACAddress & MACAddress::operator = (const MACAddress & other)
{
    _macAddress = other._macAddress;
    return *this;
}

bool MACAddress::operator == (const EndPoint & other) const
{
    if (&other == this)
        return true;
    if (other.Family() != SocketFamily::Packet)
        return false;
    const MACAddress * otherAsMACAddress = dynamic_cast<const MACAddress *>(&other);
    return (otherAsMACAddress->_macAddress == _macAddress);
}

bool MACAddress::operator != (const EndPoint & other) const
{
    return ! this->operator ==(other);
}

bool MACAddress::operator == (const MACAddress & other) const
{
    if (&other == this)
        return true;
    return (other._macAddress == _macAddress);
}

bool MACAddress::operator != (const MACAddress & other) const
{
    return ! this->operator ==(other);
}

uint8_t MACAddress::operator[] (size_t offset) const
{
    assert(offset < AddressSize);
    return _macAddress[offset];
}

OSAL::ByteArray MACAddress::GetBytes() const
{
    return _macAddress;
}

std::ostream & MACAddress::PrintTo(std::ostream & stream) const
{
    stream << hex << setfill(_('0'));
    stream << uppercase << setw(2) << (int)_macAddress[size_t{0}] << _("-");
    stream << uppercase << setw(2) << (int)_macAddress[size_t{1}] << _("-");
    stream << uppercase << setw(2) << (int)_macAddress[size_t{2}] << _("-");
    stream << uppercase << setw(2) << (int)_macAddress[size_t{3}] << _("-");
    stream << uppercase << setw(2) << (int)_macAddress[size_t{4}] << _("-");
    stream << uppercase << setw(2) << (int)_macAddress[size_t{5}];
    return stream;
}

void MACAddress::SetData(const OSAL::ByteArray & data, size_t offset)
{
    assert(offset + AddressSize <= data.Size());
    _macAddress.Set(0, data.Data() + offset, AddressSize);
}
