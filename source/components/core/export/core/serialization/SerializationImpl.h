#pragma once

#include "core/core.h"
#include "core/serialization/Serialization.h"
#include "core/Nullable.h"

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
CORE_EXPORT void Serialize (OSAL::ByteArray & result, bool value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, int8_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, uint8_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, int16_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, uint16_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, int32_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, uint32_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, int64_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, uint64_t value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, float value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, double value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, long double value);
CORE_EXPORT void Serialize (OSAL::ByteArray & result, std::string value);

template<typename EnumType>
std::string Serialize(const EnumType & value)
{
    return EnumSerializationInfo<EnumType>::Serialize(value);
}


template<class T>
std::string Serialize(const Nullable<T> & value)
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
    virtual void Serialize(OSAL::ByteArray & result, const T &value) override
    {
        Core::Serialize(result, value);
    }
};

} // namespace Core

