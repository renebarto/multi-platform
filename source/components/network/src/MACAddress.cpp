#include "network/MACAddress.h"
#include <sstream>
#include "core/Core.h"

using namespace std;
using namespace Network;

MACAddress::~MACAddress()
{
}

MACAddress MACAddress::Parse(const string & text)
{
    MACAddress macAddress;
    if (!TryParse(text, macAddress))
    {
        ostringstream stream;
        stream << "MACAddress string representation must be formatted as xx-xx-xx-xx-xx-xx, string is " << text;
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
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
    string delimiter = "-";

    size_t pos = 0;
    string token;
    uint8_t value;
    size_t index = 0;
    while (((pos = str.find(delimiter)) != string::npos) && (index < Size))
    {
        token = str.substr(0, pos);
        if (!TryParse(token, value))
            return false;
        macAddress[index++] = value;
        str.erase(0, pos + delimiter.length());
    }
    if ((index >= Size))
        return false;
    if (!TryParse(str, value))
        return false;
    macAddress[index++] = value;
    return (index == Size);
}

MACAddress & MACAddress::operator = (const MACAddress & other)
{
    macAddress = other.macAddress;
    return *this;
}

bool MACAddress::operator == (const MACAddress & other) const
{
    if (&other == this)
        return true;
    return (other.macAddress == macAddress);
}

bool MACAddress::operator != (const MACAddress & other) const
{
    return ! this->operator ==(other);
}

uint8_t & MACAddress::operator[] (size_t offset)
{
    if (offset < Size)
    {
        return macAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

const uint8_t & MACAddress::operator[] (size_t offset) const
{
    if (offset < Size)
    {
        return macAddress[offset];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
}

Core::ByteArray MACAddress::GetBytes() const
{
    return macAddress;
}

string MACAddress::ToString() const
{
    ostringstream stream;
    stream << hex << setfill('0');
    stream << uppercase << setw(2) << (int)macAddress[0] << "-";
    stream << uppercase << setw(2) << (int)macAddress[1] << "-";
    stream << uppercase << setw(2) << (int)macAddress[2] << "-";
    stream << uppercase << setw(2) << (int)macAddress[3] << "-";
    stream << uppercase << setw(2) << (int)macAddress[4] << "-";
    stream << uppercase << setw(2) << (int)macAddress[5];
    return stream.str();
}

void MACAddress::SetData(const Core::ByteArray & data, size_t offset)
{
    if (offset + Size > data.Size())
    {
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offset", "Invalid index");
    }
    macAddress.Set(0, data.Data() + offset, Size);
}
