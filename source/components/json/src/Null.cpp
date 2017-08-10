#include "json/Null.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

namespace JSON
{

static constexpr size_t IndentSize = 4;

Null::Null()
{
}

bool Null::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::NullToken:
            return true;
        default:
            return false;
    }
}

void Null::Serialize(std::basic_ostream<OSAL::Char> & stream, int UNUSED(indentDepth)) const
{
    for (int i = 0; i < indentDepth; i++)
    {
        stream << OSAL::String(IndentSize, _(' '));
    }
    stream << _("null");
}

} // namespace JSON
