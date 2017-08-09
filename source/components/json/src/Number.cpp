#include "json/Number.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

namespace JSON
{

Number::Number()
    : _value()
{
}

Number::Number(const OSAL::String & value)
    : _value(value)
{
}

bool Number::GetValue(int8_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint8_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(int16_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint16_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(int32_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint32_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(int64_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint64_t & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(float & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(double & value) const
{
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(long double & value) const
{
    return Core::Deserialize(_value, value);
}

void Number::SetValue(int8_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(uint8_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(int16_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(uint16_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(int32_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(uint32_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(int64_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(uint64_t value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(float value)
{
    _value = Core::Serialize(value, 8);
}

void Number::SetValue(double value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(long double value)
{
    _value = Core::Serialize(value);
}

bool Number::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::Number:
            _value = token.value;
            return true;
        default:
            _value = {};
            return false;
    }
}

void Number::Serialize(std::basic_ostream<OSAL::Char> & stream) const
{
    stream << _value;
}

} // namespace JSON
