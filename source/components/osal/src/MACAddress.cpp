#include "osal/MACAddress.h"
#include <sstream>
#include "osal/Assert.h"
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;
using namespace Network;

const size_t MACAddress::AddressSize = 6;

MACAddress::~MACAddress()
{
}

EndPointPtr MACAddress::Create()
{
    auto result = std::make_shared<MACAddress>();
    return result;
}

EndPointPtr MACAddress::Create(const EndPoint &other)
{
    if ((other.Family() != SocketFamily::Packet))
        return nullptr;
    const MACAddress * pOther = dynamic_cast<const MACAddress *>(&other);
    auto result = std::make_shared<MACAddress>(*pOther);
    return result;
}

EndPointPtr MACAddress::Create(const std::string &text)
{
    auto result = std::make_shared<MACAddress>();
    if (!TryParse(text, *result))
    {
        return nullptr;
    }
    return result;
}

MACAddress MACAddress::Parse(const string & text)
{
    MACAddress macAddress;
    if (!TryParse(text, macAddress))
    {
        basic_ostringstream<char> stream;
        stream << "MACAddress string representation must be formatted as xx-xx-xx-xx-xx-xx, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return macAddress;
}

bool MACAddress::TryParse(const string & text, uint8_t & value)
{
    if (text.length() != 2)
        return false;
    int extractedValue;
    if (sscanf(text.c_str(), "%02x", &extractedValue) != 1)
        return false;
    value = (uint8_t)extractedValue;

    return true;
}

bool MACAddress::TryParse(const string & text, MACAddress & macAddress)
{
    string str = text;
    const char delimiter = '-';
    MACAddress result;

    size_t pos = 0;
    string token;
    uint8_t value;
    size_t index = 0;
    while (((pos = str.find(delimiter)) != string::npos) && (index < AddressSize))
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
    ASSERT(offset < AddressSize);
    if (offset >= AddressSize)
        return _macAddress[AddressSize - 1];
    return _macAddress[offset];
}

OSAL::ByteArray MACAddress::GetBytes() const
{
    return _macAddress;
}

std::ostream & MACAddress::PrintTo(std::ostream & stream) const
{
    stream << hex << setfill('0');
    stream << uppercase << setw(2) << (int)_macAddress[size_t{0}] << "-";
    stream << uppercase << setw(2) << (int)_macAddress[size_t{1}] << "-";
    stream << uppercase << setw(2) << (int)_macAddress[size_t{2}] << "-";
    stream << uppercase << setw(2) << (int)_macAddress[size_t{3}] << "-";
    stream << uppercase << setw(2) << (int)_macAddress[size_t{4}] << "-";
    stream << uppercase << setw(2) << (int)_macAddress[size_t{5}];
    return stream;
}

void MACAddress::SetData(const OSAL::ByteArray & data, size_t offset)
{
    ASSERT(data.Size() - offset >= AddressSize);
    size_t safeOffset = std::min(AddressSize, offset);
    size_t safeCount = std::min(AddressSize, data.Size() - safeOffset);
    _macAddress.Set(0, data.Data() + safeOffset, safeCount);
}
