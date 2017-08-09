#include "json/Null.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

namespace JSON
{

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

void Null::Serialize(std::basic_ostream<OSAL::Char> & stream) const
{
    stream << _("null");
}

} // namespace JSON
