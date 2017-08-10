#include "json/String.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

namespace JSON
{

static constexpr size_t IndentSize = 4;

String::String()
    : _value()
{
}

String::String(const OSAL::String & value)
    : _value(value)
{
}

bool String::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::QuotedString:
            _value = token.value;
            return true;
        default:
            _value = {};
            return false;
    }
}

void String::Serialize(std::basic_ostream<OSAL::Char> & stream, int UNUSED(indentDepth)) const
{
    for (int i = 0; i < indentDepth; i++)
    {
        stream << OSAL::String(IndentSize, _(' '));
    }
    stream << _('"') << _value << _('"');
}

} // namespace JSON
