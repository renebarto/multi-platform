#include "json/JSONValue.h"

namespace JSON
{

enum TokenType
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
    Number,
    QuotedString,
};

struct Token
{
    TokenType type;
    OSAL::String value;
};

static bool GetTerm(std::basic_istream<OSAL::Char> & stream, OSAL::String & term)
{
    term = {};
    OSAL::Char ch;
    if (!stream.get(ch))
        return false;
    while (OSAL::Strings::isspace(ch))
    {
        if (!stream.get(ch))
            return false;
    }

    switch (ch)
    {
        case _('{'):
        case _('}'):
        case _('['):
        case _(']'):
        case _(','):
            term = ch;
            return true;
        case _('"'):
        {
            OSAL::String result;
            while (ch != _('"'))
            {
                result += ch;
                if (!stream.get(ch))
                    return false;
            }
            result += ch;
            term = result;
            return true;
        }
        default:
        {
            OSAL::String result;
            while (!OSAL::Strings::isspace(ch))
            {
                result += ch;
                if (!stream.get(ch))
                    break;
            }
            term = result;
            return true;
        }
    }
}

static Token GetToken(std::basic_istream<OSAL::Char> & stream)
{
    OSAL::String term;
    if (!GetTerm(stream, term))
        return Token { TokenType::InvalidToken, term };
    if (OSAL::IsEqualIgnoreCase(term, _("null")))
        return Token { TokenType::NullToken, term };
    if (OSAL::IsEqualIgnoreCase(term, _("false")))
        return Token { TokenType::FalseToken, term };
    if (OSAL::IsEqualIgnoreCase(term, _("true")))
        return Token { TokenType::TrueToken, term };
    if (OSAL::IsEqualIgnoreCase(term, _("{")))
        return Token { TokenType::CurlyBraceOpen, term };
    if (OSAL::IsEqualIgnoreCase(term, _("}")))
        return Token { TokenType::CurlyBraceClose, term };
    if (OSAL::IsEqualIgnoreCase(term, _("[")))
        return Token { TokenType::SquareBracketOpen, term };
    if (OSAL::IsEqualIgnoreCase(term, _("]")))
        return Token { TokenType::SquareBracketClose, term };
    if (OSAL::IsEqualIgnoreCase(term, _(",")))
        return Token { TokenType::Comma, term };
    if ((term.length() >= 2) && (term[0] == _('"')) && (term[term.length() - 1] == _('"')))
    {
        return Token { TokenType::QuotedString, term.substr(1, term.length() - 2)};
    }
    if ((term.length() >= 1) && (OSAL::Strings::isdigit(term[0]) || (term[0] == _('-'))))
    {
        size_t index = 0;
        if (term[index] == _('-'))
        {
            ++index;
            while ((index < term.length()) && (OSAL::Strings::isspace(term[index])))
                ++index;
            if (index >= term.length())
                return Token { TokenType::InvalidToken, term };
        }
        if (!OSAL::Strings::isdigit(term[index]))
            return Token { TokenType::InvalidToken, term };
        ++index;
        while ((index < term.length()) && (OSAL::Strings::isdigit(term[index])))
            ++index;
        if ((index < term.length()) && OSAL::Strings::tolower(term[index]) == _('.'))
        {
            ++index;
            while ((index < term.length()) && (OSAL::Strings::isdigit(term[index])))
                ++index;
        }
        if ((index < term.length()) && OSAL::Strings::tolower(term[index]) == _('e'))
        {
            if ((index < term.length()) && ((term[index] == _('+')) || (term[index] == _('-'))))
                ++index;
            if (index >= term.length())
                return Token { TokenType::InvalidToken, term };
            while ((index < term.length()) && (OSAL::Strings::isdigit(term[index])))
                ++index;
        }
        if (index < term.length())
            return Token { TokenType::InvalidToken, term };
        return Token { TokenType::Number, term };
    }
    return Token { TokenType::InvalidToken, term };
}

ValuePtr Parse(std::basic_istream<OSAL::Char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case FalseToken:
            return std::make_shared<Boolean>(false);
        case TrueToken:
            return std::make_shared<Boolean>(true);
        case NullToken:
            return std::make_shared<Null>();
        default:
            return nullptr;
    }
}

Value::Value()
{

}

Null::Null()
{
}

bool Null::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case NullToken:
            return true;
        default:
            return false;
    }
}

void Null::Serialize(std::basic_ostream<OSAL::Char> & stream)
{
    stream << _("null");
}

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
        case FalseToken:
            _value = false;
            return true;
        case TrueToken:
            _value = true;
            return true;
        default:
            return false;
    }
}

void Boolean::Serialize(std::basic_ostream<OSAL::Char> & stream)
{
    stream << (_value ? _("true") : _("false"));
}

} // namespace JSON
