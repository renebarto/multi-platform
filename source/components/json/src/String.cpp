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

String::String(const std::string & value)
    : _value(value)
{
}

bool String::Deserialize(std::istream & stream)
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

void String::Serialize(std::ostream & stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << '"' << _value << '"';
}

} // namespace JSON
