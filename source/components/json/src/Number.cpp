#include "json/Number.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>
#include <core/Trace.h>

namespace JSON
{

static constexpr size_t IndentSize = 4;
static constexpr int NumDigitsFloat = 8;

Number::Number()
    : _value()
{
}

Number::Number(const std::string & value)
    : _value(value)
{
}

Number::Number(int8_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(uint8_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(int16_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(uint16_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(int32_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(uint32_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(int64_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(uint64_t value)
    : _value(Core::Serialize(value))
{
}

Number::Number(float value)
    : _value(Core::Serialize(value, 0, NumDigitsFloat, Core::FloatingPointRepresentation::Mixed))
{
}

Number::Number(double value)
    : _value(Core::Serialize(value))
{
}

Number::Number(long double value)
    : _value(Core::Serialize(value))
{
}

bool Number::GetValue(int8_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint8_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(int16_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint16_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(int32_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint32_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(int64_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(uint64_t & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(float & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(double & value) const
{
    value = {};
    return Core::Deserialize(_value, value);
}

bool Number::GetValue(long double & value) const
{
    value = {};
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
    _value = Core::Serialize(value, 0, NumDigitsFloat, Core::FloatingPointRepresentation::Mixed);
}

void Number::SetValue(double value)
{
    _value = Core::Serialize(value);
}

void Number::SetValue(long double value)
{
    _value = Core::Serialize(value);
}

bool Number::Deserialize(std::basic_istream<char> & stream)
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

void Number::Serialize(std::basic_ostream<char> & stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << _value;
}

bool Number::EqualTo(std::shared_ptr<Value> other) const
{
    std::shared_ptr<Number> otherAsThis = std::dynamic_pointer_cast<Number>(other);
    if (!otherAsThis)
    {
        Core::TraceError("other value is not convertible to Number");
        return false;
    }
    return _value == otherAsThis->_value;
}

} // namespace JSON
