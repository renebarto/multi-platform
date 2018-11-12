#include "osal/PlatformDefines.h"

#if defined(WIN_MINGW)

#include <winsock2.h>
#include <Rpc.h>

#include "osal/Exception.h"
#include "osal/GUID.h"

using namespace std;
using namespace OSAL;

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

// UUID Variant definitions
#define UUID_VARIANT_NCS	0
#define UUID_VARIANT_DCE	1
#define UUID_VARIANT_MICROSOFT	2
#define UUID_VARIANT_OTHER	3

// UUID Type definitions
#define UUID_TYPE_DCE_TIME   1
#define UUID_TYPE_DCE_RANDOM 4

#define TIME_OFFSET_HIGH 0x01B21DD2
#define TIME_OFFSET_LOW  0x13814000

struct uuid {
    uint32_t	time_low;
    uint16_t	time_mid;
    uint16_t	time_hi_and_version;
    uint16_t	clock_seq;
    uint8_t	node[6];
};

//static void uuid_pack(const struct uuid * uu, unsigned char * ptr)
//{
//    uint32_t	tmp;
//    unsigned char	*out = ptr;
//
//    tmp = uu->time_low;
//    out[3] = (unsigned char) tmp;
//    tmp >>= 8;
//    out[2] = (unsigned char) tmp;
//    tmp >>= 8;
//    out[1] = (unsigned char) tmp;
//    tmp >>= 8;
//    out[0] = (unsigned char) tmp;
//
//    tmp = uu->time_mid;
//    out[5] = (unsigned char) tmp;
//    tmp >>= 8;
//    out[4] = (unsigned char) tmp;
//
//    tmp = uu->time_hi_and_version;
//    out[7] = (unsigned char) tmp;
//    tmp >>= 8;
//    out[6] = (unsigned char) tmp;
//
//    tmp = uu->clock_seq;
//    out[9] = (unsigned char) tmp;
//    tmp >>= 8;
//    out[8] = (unsigned char) tmp;
//
//    memcpy(out+10, uu->node, 6);
//}

static void uuid_unpack(const unsigned char * in, struct uuid *uu)
{
    const uint8_t	*ptr = in;
    uint32_t		tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_low = tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_mid = tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_hi_and_version = tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->clock_seq = tmp;

    memcpy(uu->node, ptr, 6);
}

static time_t uuid_time(const unsigned char * uu, struct timeval *ret_tv)
{
    struct timeval	tv;
    struct uuid		uuid;
    uint32_t		high;
    uint64_t		clock_reg;

    uuid_unpack(uu, &uuid);

    high = uuid.time_mid | ((uuid.time_hi_and_version & 0xFFF) << 16);
    clock_reg = uuid.time_low | ((uint64_t) high << 32);

    clock_reg -= (((uint64_t) 0x01B21DD2) << 32) + 0x13814000;
    tv.tv_sec = clock_reg / 10000000;
    tv.tv_usec = (clock_reg % 10000000) / 10;

    if (ret_tv)
        *ret_tv = tv;

    return tv.tv_sec;
}

int uuid_type(const unsigned char * uu)
{
    struct uuid		uuid;

    uuid_unpack(uu, &uuid);
    return ((uuid.time_hi_and_version >> 12) & 0xF);
}

int uuid_variant(const unsigned char * uu)
{
    struct uuid		uuid;
    int			var;

    uuid_unpack(uu, &uuid);
    var = uuid.clock_seq;

    if ((var & 0x8000) == 0)
        return UUID_VARIANT_NCS;
    if ((var & 0x4000) == 0)
        return UUID_VARIANT_DCE;
    if ((var & 0x2000) == 0)
        return UUID_VARIANT_MICROSOFT;
    return UUID_VARIANT_OTHER;
}

const OSAL::GUID OSAL::GUID::Null;

OSAL::GUID OSAL::GUID::Generate()
{
    struct _GUID guid;

    auto result = UuidCreate(&guid);
    if (result != RPC_S_OK)
        return ByteArray(sizeof(struct _GUID));
    return ByteArray(reinterpret_cast<const uint8_t *>(&guid), sizeof(struct _GUID));
}

OSAL::GUID OSAL::GUID::GenerateRandom()
{
    return OSAL::GUID::Generate();
}

OSAL::GUID OSAL::GUID::GenerateTime()
{
    struct _GUID guid;

    auto result = UuidCreateSequential(&guid);
    if (result != RPC_S_OK)
        return ByteArray(sizeof(struct _GUID));
    return ByteArray(reinterpret_cast<const uint8_t *>(&guid), sizeof(struct _GUID));
}

OSAL::GUID OSAL::GUID::GenerateTimeSafe()
{
    return GUID::GenerateTime();
}

timeval OSAL::GUID::GetTime() const
{
    timeval result;
    uuid_time(_guid.Data(), &result);
    return result;
}

GUIDVersion OSAL::GUID::GetVersion() const
{
    return static_cast<GUIDVersion>(uuid_type(_guid.Data()));
}

int OSAL::GUID::GetVariant() const
{
    return uuid_variant(_guid.Data());
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

bool OSAL::GUID::TryParse(const std::string & text, GUID & guid)
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

#endif // defined(WIN_MINGW)
