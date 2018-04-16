#pragma once

#include <deque>
#include <string>
#include <vector>
#include "parser/InputReader.h"
#include "regex/RE.h"

namespace Parser {

struct LexerToken
{
    enum class Type
    {
        None,
        SquareBracketOpen,
        SquareBracketClose,
        CurlyBraceOpen,
        CurlyBraceClose,
        ParenthesisOpen,
        ParenthesisClose,
        Asterisk,
        Semicolon,
        Colon,
        Comma,
        Ampersand,
        Identifier,
        DecNumber,
        OctNumber,
        HexNumber,
        String,
        Whitespace,
        NewLine,
        LineComment,
        BlockComment,
        Eof,
    };

    LexerToken()
        : type(Type::None)
        , value()
        , location()
    {}
    LexerToken(LexerToken::Type aType, SourceLocation aLocation)
        : type(aType)
        , value()
        , location(aLocation)
    {}
    LexerToken(LexerToken::Type aType, const std::string & aValue, SourceLocation aLocation)
        : type(aType)
        , value(aValue)
        , location(aLocation)
    {}
    bool IsValid() const { return type != Type::None; }

    Type type;
    std::string value;
    SourceLocation location;
};

class LexerRule
{
public:
    LexerRule() = delete;

    static LexerRule Create(Regex::RE && regex, LexerToken::Type type)
    {
        return LexerRule(std::move(regex), type);
    }

    bool Match(const std::string & value) const
    {
        return _regex.Match(value);
    }
    bool PartialMatch(const std::string & value) const
    {
        return _regex.PartialMatch(value);
    }
    LexerToken::Type Type() const
    {
        return _type;
    }
    const Regex::RE & Regex() const { return _regex; }

protected:
    Regex::RE _regex;
    LexerToken::Type _type;

public:
    LexerRule(Regex::RE && regex, LexerToken::Type type)
        : _regex(std::move(regex))
        , _type(type)
    {}
    LexerRule(const LexerRule & other)
        : _regex(other._regex)
        , _type(other._type)
    {
    }
    LexerRule & operator = (const LexerRule & other)
    {
        if (this != &other)
        {
            _regex = other._regex;
            _type = other._type;
        }
        return *this;
    }
    LexerRule & operator = (LexerRule && other)
    {
        if (this != &other)
        {
            _regex = std::move(other._regex);
            _type = other._type;
        }
        return *this;
    }
};

using LexerRuleSet = std::vector<LexerRule>;

class Lexer
{
public:
    Lexer(InputReader & reader);

    void SetRules(const LexerRuleSet & rules);

    LexerToken ReadToken();
    LexerToken ReadAheadToken();

protected:
    InputReader & _reader;
    LexerRuleSet _rules;
    std::deque<LexerToken> _pushbackQueue;

    bool FullMatch(const std::string & value, const LexerRule *& activeRule);
    bool PartialMatch(const std::string & value);
};

} // namespace Parser

inline std::ostream & operator << (std::ostream & stream, const Parser::LexerToken::Type & type)
{
    switch (type)
    {
        case Parser::LexerToken::Type::None:
            stream << "None"; break;
        case Parser::LexerToken::Type::SquareBracketOpen:
            stream << "["; break;
        case Parser::LexerToken::Type::SquareBracketClose:
            stream << "]"; break;
        case Parser::LexerToken::Type::CurlyBraceOpen:
            stream << "{"; break;
        case Parser::LexerToken::Type::CurlyBraceClose:
            stream << "}"; break;
        case Parser::LexerToken::Type::ParenthesisOpen:
            stream << "("; break;
        case Parser::LexerToken::Type::ParenthesisClose:
            stream << ")"; break;
        case Parser::LexerToken::Type::Asterisk:
            stream << "*"; break;
        case Parser::LexerToken::Type::Semicolon:
            stream << ";"; break;
        case Parser::LexerToken::Type::Colon:
            stream << ":"; break;
        case Parser::LexerToken::Type::Ampersand:
            stream << "&"; break;
        case Parser::LexerToken::Type::Comma:
            stream << ","; break;
        case Parser::LexerToken::Type::Identifier:
            stream << "Identifier"; break;
        case Parser::LexerToken::Type::HexNumber:
            stream << "HexNumber"; break;
        case Parser::LexerToken::Type::OctNumber:
            stream << "OctNumber"; break;
        case Parser::LexerToken::Type::DecNumber:
            stream << "DecNumber"; break;
        case Parser::LexerToken::Type::String:
            stream << "String"; break;
        case Parser::LexerToken::Type::Whitespace:
            stream << "SP"; break;
        case Parser::LexerToken::Type::NewLine:
            stream << "LF"; break;
        case Parser::LexerToken::Type::LineComment:
            stream << "LineComment"; break;
        case Parser::LexerToken::Type::BlockComment:
            stream << "BlockComment"; break;
        case Parser::LexerToken::Type::Eof:
            stream << "EOF"; break;
    }
    return stream;
}
inline std::ostream & operator << (std::ostream & stream, const Parser::LexerToken & token)
{
    stream << token.location << ": ";
    switch (token.type)
    {
        case Parser::LexerToken::Type::None:
            stream << "None"; break;
        case Parser::LexerToken::Type::SquareBracketOpen:
            stream << "["; break;
        case Parser::LexerToken::Type::SquareBracketClose:
            stream << "]"; break;
        case Parser::LexerToken::Type::CurlyBraceOpen:
            stream << "{"; break;
        case Parser::LexerToken::Type::CurlyBraceClose:
            stream << "}"; break;
        case Parser::LexerToken::Type::ParenthesisOpen:
            stream << "("; break;
        case Parser::LexerToken::Type::ParenthesisClose:
            stream << ")"; break;
        case Parser::LexerToken::Type::Asterisk:
            stream << "*"; break;
        case Parser::LexerToken::Type::Semicolon:
            stream << ";"; break;
        case Parser::LexerToken::Type::Colon:
            stream << ":"; break;
        case Parser::LexerToken::Type::Ampersand:
            stream << "&"; break;
        case Parser::LexerToken::Type::Comma:
            stream << ","; break;
        case Parser::LexerToken::Type::Identifier:
            stream << "Identifier " << token.value << std::endl; break;
        case Parser::LexerToken::Type::HexNumber:
            stream << "HexNumber " << token.value << std::endl; break;
        case Parser::LexerToken::Type::OctNumber:
            stream << "OctNumber " << token.value << std::endl; break;
        case Parser::LexerToken::Type::DecNumber:
            stream << "DecNumber " << token.value << std::endl; break;
        case Parser::LexerToken::Type::String:
            stream << "String " << token.value << std::endl; break;
        case Parser::LexerToken::Type::Whitespace:
            stream << "Whitespace " << token.value << std::endl; break;
        case Parser::LexerToken::Type::NewLine:
            stream << "LF" << token.value << std::endl; break;
        case Parser::LexerToken::Type::LineComment:
            stream << "//" << token.value << std::endl; break;
        case Parser::LexerToken::Type::BlockComment:
            stream << "/*" << token.value << "*/" << std::endl; break;
        case Parser::LexerToken::Type::Eof:
            stream << "<EOF>" << std::endl; break;
        default:
            stream << "Unknown" << std::endl;
    }
    return stream;
}
inline std::ostream & operator << (std::ostream & stream, const Parser::LexerRule & rule)
{
    stream << "Regex    " << rule.Regex() << std::endl;
    stream << "Type:    " << rule.Type() << std::endl;
    return stream;
}
