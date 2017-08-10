#include "Parse.h"
#include "json/Null.h"
#include "json/Boolean.h"
#include "json/String.h"
#include "json/Number.h"

namespace JSON
{

static bool IsValidKeyStartCharacter(OSAL::Char ch)
{
    return OSAL::Strings::isalpha(ch);
}

static bool IsValidCharacter(OSAL::Char ch)
{
    return OSAL::Strings::isalnum(ch) || (ch == _('-')) || (ch == _('+')) || (ch == _('.'));
}

bool GetTerm(std::basic_istream<OSAL::Char> & stream, OSAL::String & term)
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
        case _(':'):
            term = ch;
            return true;
        case _('"'):
        {
            OSAL::String result;
            result += ch;
            if (!stream.get(ch))
                return false;
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
            while (IsValidCharacter(ch))
            {
                result += ch;
                if (!stream.get(ch))
                    break;
            }
            stream.putback(ch);
            term = result;
            return true;
        }
    }
}

Token GetToken(std::basic_istream<OSAL::Char> & stream)
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
    if (OSAL::IsEqualIgnoreCase(term, _(":")))
        return Token { TokenType::Colon, term };
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
            ++index;
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
        case TokenType::FalseToken:
            return std::make_shared<Boolean>(false);
        case TokenType::TrueToken:
            return std::make_shared<Boolean>(true);
        case TokenType::NullToken:
            return std::make_shared<Null>();
        case TokenType::QuotedString:
            return std::make_shared<String>(token.value);
        case TokenType::Number:
            return std::make_shared<Number>(token.value);
        default:
            return nullptr;
    }
}

} // namespace JSON
