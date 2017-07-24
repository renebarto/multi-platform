#pragma once

#include "core/serialization/Serialization.h"
#include "core/Nullable.h"

namespace Core {

OSAL::String Serialize(bool value);
OSAL::String Serialize(uint8_t value, int base = 10);
OSAL::String Serialize(int16_t value, int base = 10);
OSAL::String Serialize(uint16_t value, int base = 10);
OSAL::String Serialize(int32_t value, int base = 10);
OSAL::String Serialize(uint32_t value, int base = 10);
OSAL::String Serialize(int64_t value, int base = 10);
OSAL::String Serialize(uint64_t value, int base = 10);
OSAL::String Serialize(float value, int precision = 16);
OSAL::String Serialize(double value, int precision = 16);
OSAL::String Serialize(OSAL::String value, bool quote = true);

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
    virtual void Serialize(OSAL::String & text, const T &value) override
    {
        text = Core::Serialize(value);
    }
};

} // namespace Core

