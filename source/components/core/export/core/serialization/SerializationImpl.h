#pragma once

#include "core/core.h"
#include "core/serialization/Serialization.h"
#include "core/nullable.h"

namespace Core {

CORE_EXPORT std::string Serialize(bool value);
CORE_EXPORT std::string Serialize(int8_t value, int base = 10);
CORE_EXPORT std::string Serialize(uint8_t value, int base = 10);
CORE_EXPORT std::string Serialize(int16_t value, int base = 10);
CORE_EXPORT std::string Serialize(uint16_t value, int base = 10);
CORE_EXPORT std::string Serialize(int32_t value, int base = 10);
CORE_EXPORT std::string Serialize(uint32_t value, int base = 10);
CORE_EXPORT std::string Serialize(int64_t value, int base = 10);
CORE_EXPORT std::string Serialize(uint64_t value, int base = 10);
CORE_EXPORT std::string Serialize(float value, int precision = 16);
CORE_EXPORT std::string Serialize(double value, int precision = 16);
CORE_EXPORT std::string Serialize(long double value, int precision = 16);
CORE_EXPORT std::string Serialize(std::string value, bool quote = true);
CORE_EXPORT void Serialize(OSAL::bytearray & result, bool value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, int8_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, uint8_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, int16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, uint16_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, int32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, uint32_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, int64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, uint64_t value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, float value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, long double value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);
CORE_EXPORT void Serialize(OSAL::bytearray & result, std::string value, OSAL::Endianness endianness = OSAL::Endianness::LittleEndian);

template<typename EnumType>
std::string Serialize(const EnumType & value)
{
    return EnumSerializationInfo<EnumType>::Serialize(value);
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

