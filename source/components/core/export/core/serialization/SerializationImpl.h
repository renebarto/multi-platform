#pragma once

#include "core/serialization/Serialization.h"
#include "core/Nullable.h"

namespace Core {

std::string Serialize(bool value);
std::string Serialize(uint8_t value, int base = 10);
std::string Serialize(int16_t value, int base = 10);
std::string Serialize(uint16_t value, int base = 10);
std::string Serialize(int32_t value, int base = 10);
std::string Serialize(uint32_t value, int base = 10);
std::string Serialize(int64_t value, int base = 10);
std::string Serialize(uint64_t value, int base = 10);
std::string Serialize(float value, int precision = 16);
std::string Serialize(double value, int precision = 16);
std::string Serialize(std::string value, bool quote = true);

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
    virtual void Serialize(std::string & text, const T &value) override
    {
        text = Core::Serialize(value);
    }
};

} // namespace Core

