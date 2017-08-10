#include "json/Boolean.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

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

bool Boolean::Deserialize(std::basic_istream<OSAL::Char> & stream)
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

void Boolean::Serialize(std::basic_ostream<OSAL::Char> & stream, int UNUSED(indentDepth)) const
{
    for (int i = 0; i < indentDepth; i++)
    {
        stream << OSAL::String(IndentSize, _(' '));
    }
    stream << (_value ? _("true") : _("false"));
}

} // namespace JSON
