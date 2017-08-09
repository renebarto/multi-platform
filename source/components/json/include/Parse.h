#include "json/Value.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>

namespace JSON
{

enum class TokenType
{
    InvalidToken,
    NullToken,
    TrueToken,
    FalseToken,
    CurlyBraceOpen,
    CurlyBraceClose,
    SquareBracketOpen,
    SquareBracketClose,
    Comma,
    Colon,
    Number,
    QuotedString,
    Key,
};

struct Token
{
    TokenType type;
    OSAL::String value;
};

bool GetTerm(std::basic_istream<OSAL::Char> & stream, OSAL::String & term);
Token GetToken(std::basic_istream<OSAL::Char> & stream);
ValuePtr Parse(std::basic_istream<OSAL::Char> & stream);

} // namespace JSON
