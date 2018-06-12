#include <core/serialization/DeserializationImpl.h>

#include <climits>
#include <cfloat>
#include <ctgmath>
#include <osal/Strings.h>
#include <osal/Unused.h>

using namespace std;
using namespace Core;
using namespace Util;

static bool HasValidCharactersForBase(const string & text, int base, bool maybeSigned)
{
    if (text.empty())
        return false;
    switch (base)
    {
        case 2:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(BinaryDigits, static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 8:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(OctalDigits, static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        case 10:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (maybeSigned)
                {
                    if (OSAL::Strings::strchr(DecimalDigitsPlusMinus,
                                              static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                        return false;
                }
                else
                {
                    if (OSAL::Strings::strchr(DecimalDigitsPlus,
                                              static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                        return false;
                }
            }
            break;
        case 16:
            for (size_t i = 0; i < text.length(); i++)
            {
                if (OSAL::Strings::strchr(HexaDecimalDigits, static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
                    return false;
            }
            break;
        default:
            return false;
    }
    return true;
}

static bool HasValidCharactersFloatingPoint(const string & text)
{
    if (text.empty())
        return false;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (OSAL::Strings::strchr(FloatingPointChars, static_cast<char>(OSAL::Strings::toupper(text[i]))) == nullptr)
            return false;
    }
    return true;
}


bool Core::Deserialize(const string & text, bool & value)
{
    if (OSAL::Strings::IsEqualIgnoreCase(BooleanTrueLiteral, text))
    {
        value = true;
        return true;
    }
    if (OSAL::Strings::IsEqualIgnoreCase(BooleanFalseLiteral, text))
    {
        value = false;
        return true;
    }

    return false;
}

bool Core::Deserialize(const string & text, int8_t & value, int base)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < CHAR_MIN) || (result > CHAR_MAX)))
        return false;
    value = static_cast<int8_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, uint8_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > UCHAR_MAX)))
        return false;
    value = static_cast<uint8_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, int16_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < SHRT_MIN) || (result > SHRT_MAX)))
        return false;
    value = static_cast<int16_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, uint16_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > USHRT_MAX)))
        return false;
    value = static_cast<uint16_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, int32_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    long result = OSAL::Strings::strtol(text.c_str(), nullptr, base);
    unsigned long result_ul = OSAL::Strings::strtoul(text.c_str(), nullptr, base);
    if (result < INT_MIN)
        return false;
    if ((base == 10) && (result >= 0) && (result_ul > INT_MAX))
        return false;
    if ((base != 10) && (result_ul > INT_MAX))
        value = static_cast<int32_t>(result_ul);
    else
        value = static_cast<int32_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, uint32_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    unsigned long result = OSAL::Strings::strtoul(text.c_str(), nullptr, base);
    if ((base == 10) && (result > UINT_MAX))
        return false;
    value = static_cast<uint32_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, int64_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, true))
        return false;

    // For some strange reason stroull behaves incorrectly in some cases.
    long long result = OSAL::Strings::strtoll(text.c_str(), nullptr, base);
    unsigned long long result_ull = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    if (result < LLONG_MIN)
        return false;
    if ((base == 10) && (result >= 0) && (result_ull > LLONG_MAX))
        return false;
    if ((base != 10) && (result_ull > LLONG_MAX))
        value = static_cast<int64_t>(result_ull);
    else
        value = result;

    return true;
}

bool Core::Deserialize(const string & text, uint64_t & value, int base /*= 10*/)
{
    value = {};
    if (!HasValidCharactersForBase(text, base, false))
        return false;

    unsigned long long result = OSAL::Strings::strtoull(text.c_str(), nullptr, base);
    value = static_cast<uint64_t>(result);

    return true;
}

bool Core::Deserialize(const string & text, float & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    long double tmp;
    std::istringstream stream(text);
    stream >> tmp;
    if ((std::abs(tmp) < FLT_MIN) || (std::abs(tmp) > FLT_MAX))
        return false;

    value = static_cast<float>(tmp);
    return true;
}

bool Core::Deserialize(const string & text, double & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    long double tmp;
    std::istringstream stream(text);
    stream >> tmp;
    if ((std::abs(tmp) < DBL_MIN) || (std::abs(tmp) > DBL_MAX))
        return false;

    value = static_cast<double>(tmp);
    return true;
}

bool Core::Deserialize(const string & text, long double & value)
{
    value = {};
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    std::istringstream stream(text);
    stream >> value;

    return true;
}

//#if !defined(WIN_MSVC)
//bool Core::Deserialize(const string & text, OSAL::Network::DomainSocketAddress & value)
//{
//    return OSAL::Network::DomainSocketAddress::TryParse(text, value);
//}
//#endif
//
//bool Core::Deserialize(const string & text, OSAL::Network::IPV4Address & value)
//{
//    return OSAL::Network::IPV4Address::TryParse(text, value);
//}
//
//bool Core::Deserialize(const string & text, OSAL::Network::IPV6Address & value)
//{
//    return OSAL::Network::IPV6Address::TryParse(text, value);
//}
//
//bool Core::Deserialize(const string & text, OSAL::Network::MACAddress & value)
//{
//    return OSAL::Network::MACAddress::TryParse(text, value);
//}

bool Core::Deserialize(const OSAL::ByteArray & result, bool & value, OSAL::Endianness UNUSED(endianness))
{
    if (result.Size() < sizeof(uint8_t))
        return false;
    value = result[0] ? true : false;
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, int8_t & value, OSAL::Endianness UNUSED(endianness))
{
    if (result.Size() < sizeof(int8_t))
        return false;
    value = static_cast<int8_t>(result[0]);
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, uint8_t & value, OSAL::Endianness UNUSED(endianness))
{
    if (result.Size() < sizeof(uint8_t))
        return false;
    value = static_cast<uint8_t>(result[0]);
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, int16_t & value, OSAL::Endianness endianness)
{
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        if (result.Size() < sizeof(int16_t))
            return false;
        value = static_cast<int16_t>(
            (result[0] << 0) |
            (result[1] << 8));
    }
    else
    {
        if (result.Size() < sizeof(int16_t))
            return false;
        value = static_cast<int16_t>(
            (result[0] << 8) |
            (result[1] << 0));
    }
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, uint16_t & value, OSAL::Endianness endianness)
{
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        if (result.Size() < sizeof(uint16_t))
            return false;
        value = static_cast<uint16_t>(
            (result[0] << 0) |
            (result[1] << 8));
    }
    else
    {
        if (result.Size() < sizeof(uint16_t))
            return false;
        value = static_cast<uint16_t>(
            (result[0] << 8) |
            (result[1] << 0));
    }
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, int32_t & value, OSAL::Endianness endianness)
{
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        if (result.Size() < sizeof(int32_t))
            return false;
        value = static_cast<int32_t>(
            (result[0] << 0) |
            (result[1] << 8) |
            (result[2] << 16) |
            (result[3] << 24));
    }
    else
    {
        if (result.Size() < sizeof(int32_t))
            return false;
        value = static_cast<int32_t>(
            (result[0] << 24) |
            (result[1] << 16) |
            (result[2] << 8) |
            (result[3] << 0));
    }
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, uint32_t & value, OSAL::Endianness endianness)
{
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        if (result.Size() < sizeof(uint32_t))
            return false;
        value = static_cast<uint32_t>(
            (result[0] << 0) |
            (result[1] << 8) |
            (result[2] << 16) |
            (result[3] << 24));
    }
    else
    {
        if (result.Size() < sizeof(uint32_t))
            return false;
        value = static_cast<uint32_t>(
            (result[0] << 24) |
            (result[1] << 16) |
            (result[2] << 8) |
            (result[3] << 0));
    }
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, int64_t & value, OSAL::Endianness endianness)
{
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        if (result.Size() < sizeof(int64_t))
            return false;
        value = static_cast<int64_t>(
            (static_cast<int64_t>(result[0]) << 0) |
            (static_cast<int64_t>(result[1]) << 8) |
            (static_cast<int64_t>(result[2]) << 16) |
            (static_cast<int64_t>(result[3]) << 24) |
            (static_cast<int64_t>(result[4]) << 32) |
            (static_cast<int64_t>(result[5]) << 40) |
            (static_cast<int64_t>(result[6]) << 48) |
            (static_cast<int64_t>(result[7]) << 56));
    }
    else
    {
        if (result.Size() < sizeof(int64_t))
            return false;
        value = static_cast<int64_t>(
            (static_cast<int64_t>(result[0]) << 56) |
            (static_cast<int64_t>(result[1]) << 48) |
            (static_cast<int64_t>(result[2]) << 40) |
            (static_cast<int64_t>(result[3]) << 32) |
            (static_cast<int64_t>(result[4]) << 24) |
            (static_cast<int64_t>(result[5]) << 16) |
            (static_cast<int64_t>(result[6]) << 8) |
            (static_cast<int64_t>(result[7]) << 0));
    }
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, uint64_t & value, OSAL::Endianness endianness)
{
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        if (result.Size() < sizeof(uint64_t))
            return false;
        value = static_cast<uint64_t>(
            (static_cast<uint64_t>(result[0]) << 0) |
            (static_cast<uint64_t>(result[1]) << 8) |
            (static_cast<uint64_t>(result[2]) << 16) |
            (static_cast<uint64_t>(result[3]) << 24) |
            (static_cast<uint64_t>(result[4]) << 32) |
            (static_cast<uint64_t>(result[5]) << 40) |
            (static_cast<uint64_t>(result[6]) << 48) |
            (static_cast<uint64_t>(result[7]) << 56));
    }
    else
    {
        if (result.Size() < sizeof(uint64_t))
            return false;
        value = static_cast<uint64_t>(
            (static_cast<uint64_t>(result[0]) << 56) |
            (static_cast<uint64_t>(result[1]) << 48) |
            (static_cast<uint64_t>(result[2]) << 40) |
            (static_cast<uint64_t>(result[3]) << 32) |
            (static_cast<uint64_t>(result[4]) << 24) |
            (static_cast<uint64_t>(result[5]) << 16) |
            (static_cast<uint64_t>(result[6]) << 8) |
            (static_cast<uint64_t>(result[7]) << 0));
    }
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, float & value, OSAL::Endianness endianness)
{
    if (result.Size() < sizeof(float))
        return false;
    uint8_t bytes[sizeof(value)];
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        size_t i = 0;
        while (i < sizeof(bytes))
        {
            bytes[i] = result[i];
            ++i;
        }
    }
    else
    {
        size_t i = sizeof(bytes);
        while (i > 0)
        {
            --i;
            bytes[sizeof(value) - i - 1] = result[i];
        }
    }
    value = *reinterpret_cast<float *>(bytes);
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, double & value, OSAL::Endianness endianness)
{
    if (result.Size() < sizeof(double))
        return false;
    uint8_t bytes[sizeof(value)];
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        size_t i = 0;
        while (i < sizeof(bytes))
        {
            bytes[i] = result[i];
            ++i;
        }
    }
    else
    {
        size_t i = sizeof(bytes);
        while (i > 0)
        {
            --i;
            bytes[sizeof(value) - i - 1] = result[i];
        }
    }
    value = *reinterpret_cast<double *>(bytes);
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, long double & value, OSAL::Endianness endianness)
{
    static constexpr size_t LongDoubleSize = 10;
    if (result.Size() < LongDoubleSize)
        return false;
    uint8_t bytes[sizeof(value)];
    if (endianness == OSAL::Endianness::LittleEndian)
    {
        size_t i = 0;
        while (i < LongDoubleSize)
        {
            bytes[i] = result[i];
            ++i;
        }
    }
    else
    {
        size_t i = LongDoubleSize;
        while (i > 0)
        {
            --i;
            bytes[LongDoubleSize - i - 1] = result[i];
        }
    }
    memset(bytes + LongDoubleSize, 0, sizeof(value) - LongDoubleSize);
    value = *reinterpret_cast<long double *>(bytes);
    return true;
}

bool Core::Deserialize(const OSAL::ByteArray & result, std::string & value, OSAL::Endianness UNUSED(endianness))
{
    size_t i = 0;
    while (i < result.Size())
    {
        uint8_t byte = result[i];
        if (byte == 0)
            break;
        value += static_cast<char>(result[i++]);
    }
    return true;
}
