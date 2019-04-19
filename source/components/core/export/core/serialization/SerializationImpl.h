#pragma once

#include <bitset>
#include <ios>
#include <sstream>
#include <string>
#include <osal/exports.h>
#if defined(LINUX) || defined(APPLE)
#include <osal/linux/DomainSocketAddress.h>
#endif
#include <osal/IPV4Address.h>
#include <osal/IPV6Address.h>
#include <osal/MACAddress.h>
#include <osal/UUID.h>
#include "core/core.h"
#include "core/serialization/Serialization.h"
#include "core/nullable.h"

namespace Core {

// String serialization
// Every serialization specialization uses a width parameter, which is used for expansion
// width < 0 Left aligned
// width > 0 right aligned
// width < actual length no alignment
template<typename T = char>
std::basic_string<T> Serialize(bool value, int width = 0);
template<typename T = char>
std::basic_string<T> Serialize(int8_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(uint8_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(int16_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(uint16_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(int32_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(uint32_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(int64_t value, int width = 0, int base = 10);
template<typename T = char>
std::basic_string<T> Serialize(uint64_t value, int width = 0, int base = 10);

enum class FloatingPointRepresentation
{
    Fixed,
    Exponential,
    Mixed
};
template<typename T = char>
std::basic_string<T> Serialize(float value, int width = 0, int precision = 16, FloatingPointRepresentation representation = FloatingPointRepresentation::Mixed);
template<typename T = char>
std::basic_string<T> Serialize(double value, int width = 0, int precision = 16, FloatingPointRepresentation representation = FloatingPointRepresentation::Mixed);
template<typename T = char>
std::basic_string<T> Serialize(long double value, int width = 0, int precision = 16, FloatingPointRepresentation representation = FloatingPointRepresentation::Mixed);
template<typename T = char>
std::basic_string<T> Serialize(const std::string & value, int width = 0, bool quote = false);
template<typename T = char>
std::basic_string<T> Serialize(const std::wstring & value, int width = 0, bool quote = false);
template<typename T = char>
std::basic_string<T> Serialize(const char * value, int width = 0, bool quote = false);
template<typename T = char>
std::basic_string<T> Serialize(const wchar_t * value, int width = 0, bool quote = false);
template<typename T = char>
std::basic_string<T> Serialize(const OSAL::Network::IPV4Address & value);
template<typename T = char>
std::basic_string<T> Serialize(const OSAL::Network::IPV6Address & value);
template<typename T = char>
std::basic_string<T> Serialize(const OSAL::Network::MACAddress & value);
template<typename T = char>
std::basic_string<T> Serialize(const OSAL::UUID & value);
#if defined(LINUX) || defined(APPLE)
template<typename T = char>
std::basic_string<T> Serialize(const OSAL::Network::DomainSocketAddress & value);
#endif

// Byte string serialization
// CORE_EXPORT void Serialize(OSAL::bytearray & result, bool value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, int8_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, uint8_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, int16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, uint16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, int32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, uint32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, int64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, uint64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, float value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, long double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const std::string &value, bool quote = false);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const std::wstring & value, bool quote = false, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const char * value, bool quote = false);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const OSAL::Network::IPV4Address & value);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const OSAL::Network::IPV6Address & value);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const OSAL::Network::MACAddress & value);
// CORE_EXPORT void Serialize(OSAL::bytearray & result, const OSAL::Network::UUID & value);
#if defined(LINUX) || defined(APPLE)
CORE_EXPORT void Serialize(OSAL::bytearray & result, const OSAL::Network::DomainSocketAddress & value);
#endif

template<typename T>
std::basic_string<T> Align(const std::basic_string<T> & text, int width)
{
    if (static_cast<size_t>(std::abs(width)) > text.length())
    {
        if (width < 0)
        {
            return text + std::basic_string<T>(-width - text.length(), T(' '));
        } else
        {
            return std::basic_string<T>(width - text.length(), T(' ')) + text;
        }
    }
    return text;
}

template<typename T>
std::basic_string<T> Serialize(bool value, int width)
{
    return Align(std::basic_string<T>(value ? "true" : "false"), width);
}

template<typename T>
std::basic_string<T> Serialize(int8_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<8> x(static_cast<uint8_t>(value));
                stream << std::setfill(T('0')) << std::setw(8) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(3) << (int)value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << (int)value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(2) << (int)value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(uint8_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<8> x(value);
                stream << std::setfill(T('0')) << std::setw(8) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(3) << (int)value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << (int)value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(2) << (int)value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(int16_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<16> x(static_cast<uint16_t>(value));
                stream << std::setfill(T('0')) << std::setw(16) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(6) << value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(4) << value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(uint16_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<16> x(value);
                stream << std::setfill(T('0')) << std::setw(16) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(6) << value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(4) << value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(int32_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<32> x(static_cast<uint32_t>(value));
                stream << std::setfill(T('0')) << std::setw(32) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(11) << value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(8) << value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(uint32_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<32> x(value);
                stream << std::setfill(T('0')) << std::setw(32) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(11) << value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(8) << value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(int64_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<64> x(static_cast<uint64_t>(value));
                stream << std::setfill(T('0')) << std::setw(64) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(22) << value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(16) << value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(uint64_t value, int width, int base)
{
    std::basic_ostringstream<T> stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<64> x(value);
                stream << std::setfill(T('0')) << std::setw(64) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill(T('0')) << std::setw(22) << value;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill(T('0')) << value;
            }
            break;
        case 16:
            {
                stream << std::hex << std::uppercase << std::setfill(T('0')) << std::setw(16) << value;
            }
            break;
        default:
            ASSERT(false);
    }
    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(float value, int width, int precision, FloatingPointRepresentation representation)
{
    std::basic_ostringstream<T> stream;

    switch (representation)
    {
        case FloatingPointRepresentation::Fixed:
            stream.setf(std::ios_base::floatfield, std::ios_base::fixed);
            stream << std::setfill(T('0')) << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Exponential:
            stream.setf(std::ios_base::floatfield, std::ios_base::scientific);
            stream << std::setfill(T('0')) << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Mixed:
            stream.unsetf(std::ios_base::floatfield);
            stream << std::setfill(T('0')) << std::setprecision(precision) << value;
            break;
    }

    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(double value, int width, int precision, FloatingPointRepresentation representation)
{
    std::basic_ostringstream<T> stream;

    switch (representation)
    {
        case FloatingPointRepresentation::Fixed:
            stream.setf(std::ios_base::floatfield, std::ios_base::fixed);
            stream << std::setfill(T('0')) << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Exponential:
            stream.setf(std::ios_base::floatfield, std::ios_base::scientific);
            stream << std::setfill(T('0')) << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Mixed:
            stream.unsetf(std::ios_base::floatfield);
            stream << std::setfill(T('0')) << std::setprecision(precision) << value;
            break;
    }

    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(long double value, int width, int precision, FloatingPointRepresentation representation)
{
    std::basic_ostringstream<T> stream;

    switch (representation)
    {
        case FloatingPointRepresentation::Fixed:
            stream.setf(std::ios_base::floatfield, std::ios_base::fixed);
            stream << std::setfill(T('0')) << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Exponential:
            stream.setf(std::ios_base::floatfield, std::ios_base::scientific);
            stream << std::setfill(T('0')) << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Mixed:
            stream.unsetf(std::ios_base::floatfield);
            stream << std::setfill(T('0')) << std::setprecision(precision) << value;
            break;
    }

    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(const std::string & value, int width, bool quote)
{
    std::basic_ostringstream<T> stream;

    if (quote)
        stream << "\"";
    stream << value;
    if (quote)
        stream << "\"";

    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(const std::wstring & value, int width, bool quote)
{
    std::basic_ostringstream<T> stream;

    stream << Serialize<T>(OSAL::Strings::WStringToString(value), width, quote);

    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(const char * value, int width, bool quote)
{
    std::basic_ostringstream<T> stream;

    stream << Serialize<T>(std::string(value), width, quote);

    return Align(stream.str(), width);
}

template<typename T>
std::basic_string<T> Serialize(const wchar_t * value, int width, bool quote)
{
    std::basic_ostringstream<T> stream;

    stream << Serialize<T>(std::wstring(value), width, quote);

    return Align(stream.str(), width);
}
template<class T>
std::string Serialize(const nullable<T> & value)
{
    if (value.HasValue())
        return Core::Serialize(value.Value());
    return "null";
}

template<class T>
class StringSerializer : public IStringSerializer<T>
{
public:
    virtual void Serialize(std::string & text, const T &value) override
    {
        text = Core::Serialize(value);
    }
};

template<class T>
class BinarySerializer : public IBinarySerializer<T>
{
public:
    virtual void Serialize(OSAL::bytearray & result, const T &value) override
    {
        Core::Serialize(result, value);
    }
};

} // namespace Core

