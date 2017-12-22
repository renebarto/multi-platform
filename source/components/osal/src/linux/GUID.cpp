#include "osal/GUID.h"

#include "uuid/uuid.h"

using namespace std;
using namespace OSAL;

const GUID GUID::Null;

GUID GUID::Generate()
{
    GUID guid;

    uuid_generate(guid._guid.Data());

    return guid;
}

GUID GUID::GenerateRandom()
{
    GUID guid;

    uuid_generate_random(guid._guid.Data());

    return guid;
}

GUID GUID::GenerateTime()
{
    GUID guid;

    uuid_generate_time(guid._guid.Data());

    return guid;
}

GUID GUID::GenerateTimeSafe()
{
    GUID guid;

    uuid_generate_time_safe(guid._guid.Data());

    return guid;
}

timeval GUID::GetTime() const
{
    timeval result;
    uuid_time(_guid.Data(), &result);
    return result;
}

GUIDVersion GUID::GetVersion() const
{
    return static_cast<GUIDVersion>(uuid_type(_guid.Data()));
}

int GUID::GetVariant() const
{
    return uuid_variant(_guid.Data());
}

extern void uuid_generate(uuid_t out);
extern void uuid_generate_random(uuid_t out);
extern void uuid_generate_time(uuid_t out);
extern int uuid_generate_time_safe(uuid_t out);
extern time_t uuid_time(const uuid_t uu, struct timeval *ret_tv);
extern int uuid_type(const uuid_t uu);
extern int uuid_variant(const uuid_t uu);

GUID GUID::Parse(const OSAL::String & text)
{
    GUID guid;
    if (!TryParse(text, guid))
    {
        basic_ostringstream<OSAL::Char> stream;
        stream << _("GUID string representation must be formatted as xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx, string is ") << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, _("text"), stream.str());
    }
    return guid;
}

bool GUID::TryParse(const OSAL::String & text, uint8_t & value)
{
    if (text.length() != 2)
        return false;
    int extractedValue;
    if (sscanf(ToNarrowString(text).c_str(), "%02x", &extractedValue) != 1)
        return false;
    value = (uint8_t)extractedValue;

    return true;
}

bool GUID::TryParse(const OSAL::String & text, GUID & guid)
{
    OSAL::String str = text;
    const OSAL::Char delimiter = _('-');
    GUID result;

    if (str.length() != 36)
        return false;
    if (str.find(delimiter) != 8)
        return false;
    str.erase(8, 1);
    if (str.find(delimiter) != 12)
        return false;
    str.erase(12, 1);
    if (str.find(delimiter) != 16)
        return false;
    str.erase(16, 1);
    if (str.find(delimiter) != 20)
        return false;
    str.erase(20, 1);
    if (str.find(delimiter) != OSAL::String::npos)
        return false;

    for (size_t index = 0; index < 16; ++index)
    {
        OSAL::String token = str.substr(index * 2, 2);
        uint8_t value;
        if (!TryParse(token, value))
            return false;
        result._guid[index] = value;
    }
    guid = result;
    return true;
}

GUID & GUID::operator = (const GUID & other)
{
    if (this != &other)
    {
        this->_guid = other._guid;
    }
    return *this;
}

bool GUID::operator == (const GUID & other) const
{
    return this->_guid == other._guid;
}

bool GUID::operator != (const GUID & other) const
{
    return this->_guid != other._guid;
}

uint8_t & GUID::operator[] (size_t offset)
{
    return this->_guid[offset];
}

const uint8_t & GUID::operator[] (size_t offset) const
{
    return this->_guid[offset];
}

OSAL::ByteArray GUID::GetBytes() const
{
    return _guid;
}

static void Print(std::ostream & stream, uint8_t value)
{
    stream << hex << setw(2) << nouppercase << setfill('0') << int(value);
}

std::ostream & GUID::PrintTo(std::ostream & stream) const
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
