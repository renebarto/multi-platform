#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "re/InputReader.h"

namespace RE {

class InputReader;

enum class TokenType
{
    None,
    SquareBracketOpen,
    SquareBracketClose,
    CurlyBraceOpen,
    CurlyBraceClose,
    ParenthesisOpen,
    ParenthesisClose,
    Literal,
    Dot,
    Asterisk,
    Plus,
    QuestionMark,
    Dash,
    Or,
    Whitespace,
    NewLine,
    Digit,
    NotDigit,
    WordChar,
    NotWordChar,
    WhitespaceChar,
    NotWhitespaceChar,
};

struct Token
{
    Token()
        : type()
        , value()
    {}
    Token(TokenType aType)
        : type(aType)
        , value()
    {}
    Token(TokenType aType, char aValue)
        : type(aType)
        , value(aValue)
    {}
    TokenType type;
    char value;

    friend bool operator == (const Token & lhs, const Token & rhs)
    {
        return (lhs.type == rhs.type) && (lhs.value == rhs.value);
    }
    friend bool operator != (const Token & lhs, const Token & rhs)
    {
        return ! operator == (lhs, rhs);
    }
    void PrintTo(std::ostream & stream) const
    {
        switch (type)
        {
            case RE::TokenType::None:
                stream << "None"; break;
            case RE::TokenType::SquareBracketOpen:
                stream << "["; break;
            case RE::TokenType::SquareBracketClose:
                stream << "]"; break;
            case RE::TokenType::CurlyBraceOpen:
                stream << "{"; break;
            case RE::TokenType::CurlyBraceClose:
                stream << "}"; break;
            case RE::TokenType::ParenthesisOpen:
                stream << "("; break;
            case RE::TokenType::ParenthesisClose:
                stream << ")"; break;
            case RE::TokenType::Literal:
                stream << "Literal " << value; break;
            case RE::TokenType::Dot:
                stream << "."; break;
            case RE::TokenType::Asterisk:
                stream << "*"; break;
            case RE::TokenType::Plus:
                stream << "+"; break;
            case RE::TokenType::QuestionMark:
                stream << "?"; break;
            case RE::TokenType::Dash:
                stream << "-"; break;
            case RE::TokenType::Or:
                stream << "|"; break;
            case RE::TokenType::Whitespace:
                stream << "Whitespace " << value; break;
            case RE::TokenType::NewLine:
                stream << "LF" << value; break;
            case RE::TokenType::Digit:
                stream << "Digit"; break;
            case RE::TokenType::NotDigit:
                stream << "~Digit"; break;
            case RE::TokenType::WordChar:
                stream << "Worchar"; break;
            case RE::TokenType::NotWordChar:
                stream << "~Worchar"; break;
            case RE::TokenType::WhitespaceChar:
                stream << "Whitespace"; break;
            case RE::TokenType::NotWhitespaceChar:
                stream << "~Whitespace"; break;
        }
    }
};

using TokenList = std::vector<Token>;

class TokenIterator
{
public:
    TokenIterator(const TokenList & tokens)
        : _current(tokens.begin())
        , _end(tokens.end())
    {}
    bool AtEnd() const { return _current >= _end; }
    void Next() { ++_current; }
    TokenIterator & operator ++() { ++_current; return *this; }
    TokenIterator operator ++(int)
    {
        TokenIterator result(_current, _end);
        ++_current;
        return result;
    }
    TokenIterator operator + (int index)
    {
        return TokenIterator(_current + index, _end);
    }
    const Token * operator -> () const { return &(*_current); }

protected:
    TokenList::const_iterator _current;
    TokenList::const_iterator _end;

    TokenIterator(TokenList::const_iterator begin, TokenList::const_iterator end)
        : _current(begin)
        , _end(end)
    {}
};

class Tokenizer
{
public:
    Tokenizer(InputReader & reader);

    bool Tokenize();
    const TokenList & GetTokens() const { return _tokens; }
    TokenIterator GetTokenIterator() const { return _tokens; }

protected:
    TokenList _tokens;
    InputReader & _reader;

    bool ReadToken(Token & token);
};

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, const RE::TokenType & type)
{
    switch (type)
    {
        case RE::TokenType::None:
            stream << "None"; break;
        case RE::TokenType::SquareBracketOpen:
            stream << "["; break;
        case RE::TokenType::SquareBracketClose:
            stream << "]"; break;
        case RE::TokenType::CurlyBraceOpen:
            stream << "{"; break;
        case RE::TokenType::CurlyBraceClose:
            stream << "}"; break;
        case RE::TokenType::ParenthesisOpen:
            stream << "("; break;
        case RE::TokenType::ParenthesisClose:
            stream << ")"; break;
        case RE::TokenType::Literal:
            stream << "Literal"; break;
        case RE::TokenType::Asterisk:
            stream << "*"; break;
        case RE::TokenType::Dot:
            stream << "."; break;
        case RE::TokenType::Plus:
            stream << "-"; break;
        case RE::TokenType::QuestionMark:
            stream << "?"; break;
        case RE::TokenType::Dash:
            stream << "-"; break;
        case RE::TokenType::Or:
            stream << "|"; break;
        case RE::TokenType::Whitespace:
            stream << "Whitespace"; break;
        case RE::TokenType::NewLine:
            stream << "Newline"; break;
        case RE::TokenType::Digit:
            stream << "Digit"; break;
        case RE::TokenType::NotDigit:
            stream << "~Digit"; break;
        case RE::TokenType::WordChar:
            stream << "Worchar"; break;
        case RE::TokenType::NotWordChar:
            stream << "~Worchar"; break;
        case RE::TokenType::WhitespaceChar:
            stream << "Whitespace"; break;
        case RE::TokenType::NotWhitespaceChar:
            stream << "~Whitespace"; break;
    }
    return stream;
}

inline std::ostream & operator << (std::ostream & stream, const RE::Token & token)
{
    token.PrintTo(stream);
    return stream;
}

