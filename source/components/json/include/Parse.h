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
};

struct Token
{
    TokenType type;
    std::string value;
};

bool GetTerm(std::basic_istream<char> & stream, std::string & term);
Token GetToken(std::basic_istream<char> & stream);
ValuePtr Parse(std::basic_istream<char> & stream);

} // namespace JSON
