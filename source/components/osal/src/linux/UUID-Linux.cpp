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

    uuid_generate(uuid._uuid.data());

    return uuid;
}

UUID UUID::GenerateRandom()
{
    UUID uuid;

    uuid_generate_random(uuid._uuid.data());

    return uuid;
}

UUID UUID::GenerateTime()
{
    UUID uuid;

    uuid_generate_time(uuid._uuid.data());

    return uuid;
}

UUID UUID::GenerateTimeSafe()
{
    UUID uuid;

    uuid_generate_time_safe(uuid._uuid.data());

    return uuid;
}

timeval UUID::GetTime() const
{
    timeval result;
    uuid_time(_uuid.data(), &result);
    return result;
}

UUIDVersion UUID::GetVersion() const
{
    return static_cast<UUIDVersion>(uuid_type(_uuid.data()));
}

UUIDVariant UUID::GetVariant() const
{
    return static_cast<UUIDVariant>(uuid_variant(_uuid.data()));
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

OSAL::bytearray UUID::GetBytes() const
{
    return _uuid;
}

#endif // defined(LINUX)
