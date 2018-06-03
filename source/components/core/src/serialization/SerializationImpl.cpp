#include <core/serialization/SerializationImpl.h>

#include <bitset>
#include <climits>
#include <iomanip>
#include <sstream>
#include <string>
#include <osal/Unused.h>

using namespace std;
using namespace Core;
using namespace Util;

string Core::Serialize(bool value)
{
    return value ? BooleanTrueLiteral : BooleanFalseLiteral;
}

string Core::Serialize(int8_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<8> x(static_cast<uint8_t>(value));
            stream << std::setfill('0') << std::setw(8) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(3) << (int)value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << (int)value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint8_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<8> x(value);
            stream << std::setfill('0') << std::setw(8) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(3) << (int)value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << (int)value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(int16_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<16> x(static_cast<uint16_t>(value));
            stream << std::setfill('0') << std::setw(16) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(6) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint16_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<16> x(value);
            stream << std::setfill('0') << std::setw(16) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(6) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(int32_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<32> x(static_cast<uint32_t>(value));
            stream << std::setfill('0') << std::setw(32) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(11) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint32_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<32> x(value);
            stream << std::setfill('0') << std::setw(32) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(11) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(int64_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<64> x(static_cast<uint64_t>(value));
            stream << std::setfill('0') << std::setw(64) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(22) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(uint64_t value, int base)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
        {
            std::bitset<64> x(value);
            stream << std::setfill('0') << std::setw(64) << x;
        }
            break;
        case 8:
        {
            stream << std::oct << std::setfill('0') << std::setw(22) << value;
        }
            break;
        case 10:
        {
            stream << std::dec << std::setfill('0') << value;
        }
            break;
        case 16:
        {
            stream << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << value;
        }
            break;
        default:
            assert(false);
    }
    return stream.str();
}

string Core::Serialize(float value, int precision)
{
    std::ostringstream stream;

    stream << std::setprecision(precision) << value;

    return stream.str();
}

string Core::Serialize(double value, int precision)
{
    std::ostringstream stream;

    stream << std::setprecision(precision) << value;

    return stream.str();
}

string Core::Serialize(long double value, int precision)
{
    std::ostringstream stream;

    stream << std::setprecision(precision) << value;

    return stream.str();
}

void Core::Serialize(OSAL::ByteArray & result, bool value, Endianness UNUSED(endianness))
{
    result.Append({static_cast<uint8_t>(value ? 0x01 : 0x00)});
}

void Core::Serialize(OSAL::ByteArray & result, int8_t value, Endianness UNUSED(endianness))
{
    result.Append({static_cast<uint8_t>(value)});
}

void Core::Serialize(OSAL::ByteArray & result, uint8_t value, Endianness UNUSED(endianness))
{
    result.Append({value});
}

void Core::Serialize(OSAL::ByteArray & result, int16_t value, Endianness endianness)
{
    if (endianness == Endianness::LittleEndian)
    {
        result.Append({static_cast<uint8_t>((value >> 0) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF)});
    }
    else
    {
        result.Append({static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 0) & 0xFF)});
    }
}

void Core::Serialize(OSAL::ByteArray & result, uint16_t value, Endianness endianness)
{
    if (endianness == Endianness::LittleEndian)
    {
        result.Append({static_cast<uint8_t>((value >> 0) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF)});
    }
    else
    {
        result.Append({static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 0) & 0xFF)});
    }
}

void Core::Serialize(OSAL::ByteArray & result, int32_t value, Endianness endianness)
{
    if (endianness == Endianness::LittleEndian)
    {
        result.Append({static_cast<uint8_t>((value >> 0) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 24) & 0xFF)});
    }
    else
    {
        result.Append({static_cast<uint8_t>((value >> 24) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 0) & 0xFF)});
    }
}

void Core::Serialize(OSAL::ByteArray & result, uint32_t value, Endianness endianness)
{
    if (endianness == Endianness::LittleEndian)
    {
        result.Append({static_cast<uint8_t>((value >> 0) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 24) & 0xFF)});
    }
    else
    {
        result.Append({static_cast<uint8_t>((value >> 24) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 0) & 0xFF)});
    }
}

void Core::Serialize(OSAL::ByteArray & result, int64_t value, Endianness endianness)
{
    if (endianness == Endianness::LittleEndian)
    {
        result.Append({static_cast<uint8_t>((value >> 0) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 24) & 0xFF),
                       static_cast<uint8_t>((value >> 32) & 0xFF),
                       static_cast<uint8_t>((value >> 40) & 0xFF),
                       static_cast<uint8_t>((value >> 48) & 0xFF),
                       static_cast<uint8_t>((value >> 56) & 0xFF)});
    }
    else
    {
        result.Append({static_cast<uint8_t>((value >> 56) & 0xFF),
                       static_cast<uint8_t>((value >> 48) & 0xFF),
                       static_cast<uint8_t>((value >> 40) & 0xFF),
                       static_cast<uint8_t>((value >> 32) & 0xFF),
                       static_cast<uint8_t>((value >> 24) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 0) & 0xFF)});
    }
}

void Core::Serialize(OSAL::ByteArray & result, uint64_t value, Endianness endianness)
{
    if (endianness == Endianness::LittleEndian)
    {
        result.Append({static_cast<uint8_t>((value >> 0) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 24) & 0xFF),
                       static_cast<uint8_t>((value >> 32) & 0xFF),
                       static_cast<uint8_t>((value >> 40) & 0xFF),
                       static_cast<uint8_t>((value >> 48) & 0xFF),
                       static_cast<uint8_t>((value >> 56) & 0xFF)});
    }
    else
    {
        result.Append({static_cast<uint8_t>((value >> 56) & 0xFF),
                       static_cast<uint8_t>((value >> 48) & 0xFF),
                       static_cast<uint8_t>((value >> 40) & 0xFF),
                       static_cast<uint8_t>((value >> 32) & 0xFF),
                       static_cast<uint8_t>((value >> 24) & 0xFF),
                       static_cast<uint8_t>((value >> 16) & 0xFF),
                       static_cast<uint8_t>((value >> 8) & 0xFF),
                       static_cast<uint8_t>((value >> 0) & 0xFF)});
    }
}

void Core::Serialize(OSAL::ByteArray & result, float value, Endianness endianness)
{
    uint8_t bytes[sizeof(value)];
    *reinterpret_cast<float *>(bytes) = value;
    if (endianness == Endianness::LittleEndian)
    {
        size_t i = 0;
        while (i < sizeof(bytes))
        {
            result.Append({bytes[i++]});
        }
    }
    else
    {
        size_t i = sizeof(bytes);
        while (i > 0)
        {
            result.Append({bytes[--i]});
        }
    }
}

void Core::Serialize(OSAL::ByteArray & result, double value, Endianness endianness)
{
    uint8_t bytes[sizeof(value)];
    *reinterpret_cast<double *>(bytes) = value;
    if (endianness == Endianness::LittleEndian)
    {
        size_t i = 0;
        while (i < sizeof(bytes))
        {
            result.Append({bytes[i++]});
        }
    }
    else
    {
        size_t i = sizeof(bytes);
        while (i > 0)
        {
            result.Append({bytes[--i]});
        }
    }
}

void Core::Serialize(OSAL::ByteArray & result, long double value, Endianness endianness)
{
    static constexpr size_t LongDoubleSize = 10;
    uint8_t bytes[sizeof(value)];
    *reinterpret_cast<long double *>(bytes) = value;
    if (endianness == Endianness::LittleEndian)
    {
        size_t i = 0;
        while (i < LongDoubleSize)
        {
            result.Append({bytes[i++]});
        }
    }
    else
    {
        size_t i = LongDoubleSize;
        while (i > 0)
        {
            result.Append({bytes[--i]});
        }
    }
}

void Core::Serialize(OSAL::ByteArray & result, std::string value, Endianness UNUSED(endianness))
{
    size_t i = 0;
    while (i < value.length())
    {
        result.Append({static_cast<uint8_t>(value[i++])});
    }
    result.Append({static_cast<uint8_t>(0x00)});
}
