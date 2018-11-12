#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include "osal/osal.h"

#include "osal/Exception.h"
#include "osal/GUID.h"

using namespace std;
using namespace OSAL;

const OSAL::GUID OSAL::GUID::Null;

OSAL::GUID OSAL::GUID::Generate()
{
    OSAL::GUID guid;

    UuidCreate(reinterpret_cast<UUID *>(guid._guid.Data()));

    return guid;
}

OSAL::GUID OSAL::GUID::GenerateRandom()
{
    OSAL::GUID guid;

    UuidCreate(reinterpret_cast<UUID *>(guid._guid.Data()));

    return guid;
}

OSAL::GUID OSAL::GUID::GenerateTime()
{
    OSAL::GUID guid;

    UuidCreateSequential(reinterpret_cast<UUID *>(guid._guid.Data()));

    return guid;
}

OSAL::GUID OSAL::GUID::GenerateTimeSafe()
{
    OSAL::GUID guid;

    UuidCreateSequential(reinterpret_cast<UUID *>(guid._guid.Data()));

    return guid;
}

timeval OSAL::GUID::GetTime() const
{
    timeval result {};
    return result;
}

GUIDVersion OSAL::GUID::GetVersion() const
{
    return static_cast<GUIDVersion>(static_cast<GUIDVersion>(_guid.Data()[0] >> 4));
}

int OSAL::GUID::GetVariant() const
{
    return (((_guid.Data()[0] >> 2) & 0x03) == 0x01) ? 1 : 2;
}


OSAL::GUID OSAL::GUID::Parse(const std::string & text)
{
    OSAL::GUID guid;
    if (!TryParse(text, guid))
    {
        ostringstream stream;
        stream << "GUID string representation must be formatted as xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return guid;
}

bool OSAL::GUID::TryParse(const std::string & text, uint8_t & value)
{
    if (text.length() != 2)
        return false;
    int extractedValue;
    if (sscanf(text.c_str(), "%02x", &extractedValue) != 1)
        return false;
    value = (uint8_t)extractedValue;

    return true;
}

bool OSAL::GUID::TryParse(const std::string & text, OSAL::GUID & guid)
{
    std::string str = text;
    const char delimiter = '-';
    OSAL::GUID result;

    if (str.length() != 36)
        return false;
    if (str[8] != delimiter)
        return false;
    str.erase(8, 1);
    if (str[12] != delimiter)
        return false;
    str.erase(12, 1);
    if (str[16] != delimiter)
        return false;
    str.erase(16, 1);
    if (str[20] != delimiter)
        return false;
    str.erase(20, 1);
    if (str.find(delimiter) != std::string::npos)
        return false;

    for (size_t index = 0; index < 16; ++index)
    {
        std::string token = str.substr(index * 2, 2);
        uint8_t value;
        if (!TryParse(token, value))
            return false;
        result._guid[index] = value;
    }
    guid = result;
    return true;
}

OSAL::GUID & OSAL::GUID::operator = (const OSAL::GUID & other)
{
    if (this != &other)
    {
        this->_guid = other._guid;
    }
    return *this;
}

bool OSAL::GUID::operator == (const OSAL::GUID & other) const
{
    return this->_guid == other._guid;
}

bool OSAL::GUID::operator != (const OSAL::GUID & other) const
{
    return this->_guid != other._guid;
}

uint8_t & OSAL::GUID::operator[] (size_t offset)
{
    return this->_guid[offset];
}

const uint8_t & OSAL::GUID::operator[] (size_t offset) const
{
    return this->_guid[offset];
}

OSAL::ByteArray OSAL::GUID::GetBytes() const
{
    return _guid;
}

static void Print(std::ostream & stream, uint8_t value)
{
    stream << hex << setw(2) << nouppercase << setfill('0') << int(value);
}

std::ostream & OSAL::GUID::PrintTo(std::ostream & stream) const
{
    Print(stream, _guid[0]);
    Print(stream, _guid[1]);
    Print(stream, _guid[2]);
    Print(stream, _guid[3]);
    stream << '-';
    Print(stream, _guid[4]);
    Print(stream, _guid[5]);
    stream << '-';
    Print(stream, _guid[6]);
    Print(stream, _guid[7]);
    stream << '-';
    Print(stream, _guid[8]);
    Print(stream, _guid[9]);
    stream << '-';
    Print(stream, _guid[10]);
    Print(stream, _guid[11]);
    Print(stream, _guid[12]);
    Print(stream, _guid[13]);
    Print(stream, _guid[14]);
    Print(stream, _guid[15]);
    return stream;
}

#endif // defined(WIN_MSVC)
