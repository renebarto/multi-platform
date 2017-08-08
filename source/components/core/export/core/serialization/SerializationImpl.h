#pragma once

#include "core/Core.h"
#include "core/serialization/Serialization.h"
#include "core/Nullable.h"

namespace Core {

CORE_EXPORT OSAL::String Serialize(bool value);
CORE_EXPORT OSAL::String Serialize(int8_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(uint8_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(int16_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(uint16_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(int32_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(uint32_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(int64_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(uint64_t value, int base = 10);
CORE_EXPORT OSAL::String Serialize(float value, int precision = 16);
CORE_EXPORT OSAL::String Serialize(double value, int precision = 16);
CORE_EXPORT OSAL::String Serialize(long double value, int precision = 16);
CORE_EXPORT OSAL::String Serialize(OSAL::String value, bool quote = true);

template<typename EnumType>
OSAL::String Serialize(const EnumType & value)
{
    return EnumSerializationInfo<EnumType>::Serialize(value);
}


template<class T>
OSAL::String Serialize(const Nullable<T> & value)
{
    if (value.HasValue())
        return Core::Serialize(value.Value());
    return "null";
}

template<class T>
class StringSerializer : public IStringSerializer<T>
{
public:
    virtual void Serialize(OSAL::String & text, const T &value) override
    {
        text = Core::Serialize(value);
    }
};

} // namespace Core

