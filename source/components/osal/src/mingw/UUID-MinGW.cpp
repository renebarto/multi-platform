#include "osal/PlatformDefines.h"

#if defined(WIN_MINGW)

#include "osal/osal.h"

#include "osal/Exception.h"
#include "osal/UUID.h"

using namespace std;
using namespace OSAL;

#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

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
    const uint8_t *ptr = in;
    uint32_t tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_low = tmp;

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_mid = static_cast<uint16_t>(tmp);

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_hi_and_version = static_cast<uint16_t>(tmp);

    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->clock_seq = static_cast<uint16_t>(tmp);

    memcpy(uu->node, ptr, 6);
}

static time_t uuid_time(const unsigned char * uu, struct timeval *ret_tv)
{
    struct timeval tv;
    struct uuid uuid;
    uint32_t high;
    uint64_t clock_reg;

    uuid_unpack(uu, &uuid);

    high = static_cast<uint32_t>(uuid.time_mid | ((uuid.time_hi_and_version & 0xFFF) << 16));
    clock_reg = uuid.time_low | ((uint64_t) high << 32);

    clock_reg -= (((uint64_t) 0x01B21DD2) << 32) + 0x13814000;
    tv.tv_sec = static_cast<long>(clock_reg / 10000000);
    tv.tv_usec = static_cast<long>((clock_reg % 10000000) / 10);

    if (ret_tv)
        *ret_tv = tv;

    return tv.tv_sec;
}

static int uuid_type(const unsigned char * uu)
{
    struct uuid uuid;

    uuid_unpack(uu, &uuid);
    return ((uuid.time_hi_and_version >> 12) & 0xF);
}

static UUIDVariant uuid_variant(const unsigned char * uu)
{
    struct uuid uuid;
    int var;

    uuid_unpack(uu, &uuid);
    var = uuid.clock_seq;

    if ((var & 0x8000) == 0)
        return UUIDVariant::NCS;
    if ((var & 0x4000) == 0)
        return UUIDVariant::RFC4122;
    if ((var & 0x2000) == 0)
        return UUIDVariant::Microsoft;
    return UUIDVariant::Reserved;
}

const OSAL::UUID OSAL::UUID::Null;

OSAL::UUID OSAL::UUID::Generate()
{
    struct _GUID uuid;

    auto result = CoCreateGuid(&uuid);
    if (result != RPC_S_OK)
        return ByteArray(sizeof(struct _GUID));

    std::array<uint8_t, 16> bytes =
    {
        static_cast<uint8_t>((uuid.Data1 >> 24) & 0xFF),
        static_cast<uint8_t>((uuid.Data1 >> 16) & 0xFF),
        static_cast<uint8_t>((uuid.Data1 >> 8) & 0xFF),
        static_cast<uint8_t>((uuid.Data1) & 0xFF),

        static_cast<uint8_t>((uuid.Data2 >> 8) & 0xFF),
        static_cast<uint8_t>((uuid.Data2) & 0xFF),

        static_cast<uint8_t>((uuid.Data3 >> 8) & 0xFF),
        static_cast<uint8_t>((uuid.Data3) & 0xFF),

        uuid.Data4[0],
        uuid.Data4[1],
        uuid.Data4[2],
        uuid.Data4[3],
        uuid.Data4[4],
        uuid.Data4[5],
        uuid.Data4[6],
        uuid.Data4[7]

    };

    // variant must be 0b110xxxxxx (Microsoft)
    bytes[8] &= 0xDF;
    bytes[8] |= 0xC0;

    // version must be 0b0100xxxx (Random)
    bytes[6] &= 0x4F;
    bytes[6] |= 0x40;

    return ByteArray(bytes.data(), bytes.size());
}

OSAL::UUID OSAL::UUID::GenerateRandom()
{
    return OSAL::UUID::Generate();
}

OSAL::UUID OSAL::UUID::GenerateTime()
{
    struct _GUID uuid;

    auto result = CoCreateGuid(&uuid);
    if (result != RPC_S_OK)
        return ByteArray(sizeof(struct _GUID));

    std::array<uint8_t, 16> bytes =
    {
            static_cast<uint8_t>((uuid.Data1 >> 24) & 0xFF),
            static_cast<uint8_t>((uuid.Data1 >> 16) & 0xFF),
            static_cast<uint8_t>((uuid.Data1 >> 8) & 0xFF),
            static_cast<uint8_t>((uuid.Data1) & 0xFF),

            static_cast<uint8_t>((uuid.Data2 >> 8) & 0xFF),
            static_cast<uint8_t>((uuid.Data2) & 0xFF),

            static_cast<uint8_t>((uuid.Data3 >> 8) & 0xFF),
            static_cast<uint8_t>((uuid.Data3) & 0xFF),

            uuid.Data4[0],
            uuid.Data4[1],
            uuid.Data4[2],
            uuid.Data4[3],
            uuid.Data4[4],
            uuid.Data4[5],
            uuid.Data4[6],
            uuid.Data4[7]

    };

    // variant must be 0b110xxxxxx (Microsoft)
    bytes[8] &= 0xDF;
    bytes[8] |= 0xC0;

    // version must be 0b0001xxxx (DateTimeMac)
    bytes[6] &= 0x1F;
    bytes[6] |= 0x10;

    return ByteArray(bytes.data(), bytes.size());
}

OSAL::UUID OSAL::UUID::GenerateTimeSafe()
{
    return UUID::GenerateTime();
}

timeval OSAL::UUID::GetTime() const
{
    timeval result;
    uuid_time(_uuid.Data(), &result);
    return result;
}

UUIDVersion OSAL::UUID::GetVersion() const
{
    return static_cast<UUIDVersion>(uuid_type(_uuid.Data()));
}

UUIDVariant OSAL::UUID::GetVariant() const
{
    return uuid_variant(_uuid.Data());
}

OSAL::UUID OSAL::UUID::Parse(const std::string & text)
{
    OSAL::UUID uuid;
    if (!TryParse(text, uuid))
    {
        ostringstream stream;
        stream << "UUID string representation must be formatted as xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return uuid;
}

bool OSAL::UUID::TryParse(const std::string & text, uint8_t & value)
{
    if (text.length() != 2)
        return false;
    int extractedValue;
    if (sscanf(text.c_str(), "%02x", &extractedValue) != 1)
        return false;
    value = (uint8_t)extractedValue;

    return true;
}

bool OSAL::UUID::TryParse(const std::string & text, UUID & uuid)
{
    std::string str = text;
    const char delimiter = '-';
    OSAL::UUID result;

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

OSAL::UUID & OSAL::UUID::operator = (const OSAL::UUID & other)
{
    if (this != &other)
    {
        this->_uuid = other._uuid;
    }
    return *this;
}

bool OSAL::UUID::operator == (const OSAL::UUID & other) const
{
    return this->_uuid == other._uuid;
}

bool OSAL::UUID::operator != (const OSAL::UUID & other) const
{
    return this->_uuid != other._uuid;
}

uint8_t & OSAL::UUID::operator[] (size_t offset)
{
    return this->_uuid[offset];
}

const uint8_t & OSAL::UUID::operator[] (size_t offset) const
{
    return this->_uuid[offset];
}

OSAL::ByteArray OSAL::UUID::GetBytes() const
{
    return _uuid;
}

static void Print(std::ostream & stream, uint8_t value)
{
    stream << hex << setw(2) << nouppercase << setfill('0') << int(value);
}

std::ostream & OSAL::UUID::PrintTo(std::ostream & stream) const
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

#endif // defined(WIN_MINGW)
