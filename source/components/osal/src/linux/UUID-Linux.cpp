#include "osal/PlatformDefines.h"

#if defined(LINUX)

#include "uuid/uuid.h"

#include "osal/Exception.h"
#include "osal/UUID.h"

using namespace std;
using namespace OSAL;

const UUID UUID::Null;

UUID UUID::Generate()
{
    UUID uuid;

    uuid_generate(uuid._uuid.Data());

    return uuid;
}

UUID UUID::GenerateRandom()
{
    UUID uuid;

    uuid_generate_random(uuid._uuid.Data());

    return uuid;
}

UUID UUID::GenerateTime()
{
    UUID uuid;

    uuid_generate_time(uuid._uuid.Data());

    return uuid;
}

UUID UUID::GenerateTimeSafe()
{
    UUID uuid;

    uuid_generate_time_safe(uuid._uuid.Data());

    return uuid;
}

timeval UUID::GetTime() const
{
    timeval result;
    uuid_time(_uuid.Data(), &result);
    return result;
}

UUIDVersion UUID::GetVersion() const
{
    return static_cast<UUIDVersion>(uuid_type(_uuid.Data()));
}

int UUID::GetVariant() const
{
    return uuid_variant(_uuid.Data());
}

extern void uuid_generate(uuid_t out);
extern void uuid_generate_random(uuid_t out);
extern void uuid_generate_time(uuid_t out);
extern int uuid_generate_time_safe(uuid_t out);
extern time_t uuid_time(const uuid_t uu, struct timeval *ret_tv);
extern int uuid_type(const uuid_t uu);
extern int uuid_variant(const uuid_t uu);

UUID UUID::Parse(const std::string & text)
{
    UUID uuid;
    if (!TryParse(text, uuid))
    {
        ostringstream stream;
        stream << "UUID string representation must be formatted as xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return uuid;
}

bool UUID::TryParse(const std::string & text, uint8_t & value)
{
    if (text.length() != 2)
        return false;
    int extractedValue;
    if (sscanf(text.c_str(), "%02x", &extractedValue) != 1)
        return false;
    value = (uint8_t)extractedValue;

    return true;
}

bool UUID::TryParse(const std::string & text, UUID & uuid)
{
    std::string str = text;
    const char delimiter = '-';
    UUID result;

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
        result._uuid[index] = value;
    }
    uuid = result;
    return true;
}

UUID & UUID::operator = (const UUID & other)
{
    if (this != &other)
    {
        this->_uuid = other._uuid;
    }
    return *this;
}

bool UUID::operator == (const UUID & other) const
{
    return this->_uuid == other._uuid;
}

bool UUID::operator != (const UUID & other) const
{
    return this->_uuid != other._uuid;
}

uint8_t & UUID::operator[] (size_t offset)
{
    return this->_uuid[offset];
}

const uint8_t & UUID::operator[] (size_t offset) const
{
    return this->_uuid[offset];
}

OSAL::ByteArray UUID::GetBytes() const
{
    return _uuid;
}

static void Print(std::ostream & stream, uint8_t value)
{
    stream << hex << setw(2) << nouppercase << setfill('0') << int(value);
}

std::ostream & UUID::PrintTo(std::ostream & stream) const
{
    Print(stream, _uuid[0]);
    Print(stream, _uuid[1]);
    Print(stream, _uuid[2]);
    Print(stream, _uuid[3]);
    stream << '-';
    Print(stream, _uuid[4]);
    Print(stream, _uuid[5]);
    stream << '-';
    Print(stream, _uuid[6]);
    Print(stream, _uuid[7]);
    stream << '-';
    Print(stream, _uuid[8]);
    Print(stream, _uuid[9]);
    stream << '-';
    Print(stream, _uuid[10]);
    Print(stream, _uuid[11]);
    Print(stream, _uuid[12]);
    Print(stream, _uuid[13]);
    Print(stream, _uuid[14]);
    Print(stream, _uuid[15]);
    return stream;
}

#endif // defined(LINUX)
