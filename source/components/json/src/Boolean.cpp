#include "json/Boolean.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>
#include <core/Trace.h>

namespace JSON
{

static constexpr size_t IndentSize = 4;

Boolean::Boolean()
    : _value()
{
}

Boolean::Boolean(bool value)
    : _value(value)
{
}

bool Boolean::Deserialize(std::basic_istream<char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::FalseToken:
            _value = false;
            return true;
        case TokenType::TrueToken:
            _value = true;
            return true;
        default:
            _value = {};
            return false;
    }
}

void Boolean::Serialize(std::basic_ostream<char> & stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << (_value ? "true" : "false");
}

bool Boolean::EqualTo(std::shared_ptr<Value> other) const
{
    std::shared_ptr<Boolean> otherAsThis = std::dynamic_pointer_cast<Boolean>(other);
    if (!otherAsThis)
    {
        Core::TraceError("other value is not convertible to Boolean");
        return false;
    }
    return _value == otherAsThis->_value;
}

} // namespace JSON
