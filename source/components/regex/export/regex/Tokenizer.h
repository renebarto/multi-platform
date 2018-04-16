#pragma once

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "regex/InputReader.h"

namespace Regex {

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
    Caret,
    Space,
    Tab,
    NewLine,
    Return,
    Digit, // '0'..'9'
    NotDigit,
    WordChar, // '0'..'9', 'A'..'Z', 'a'..'z', '_'
    NotWordChar,
    AlphaChar, // 'A'..'Z', 'a'..'z', '_'
    NotAlphaChar,
    WhitespaceChar, // ' ', '\t', '\f', '\r', '\n'
    NotWhitespaceChar,
};
using TokenTypeSet = std::set<TokenType>;

struct Token
{
    Token()
        : type()
        , value()
    {}
    Token(const Token & other)
        : type(other.type)
        , value(other.value)
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

    Token & operator = (const Token & other)
    {
        if (this != &other)
        {
            type = other.type;
            value = other.value;
        }
        return *this;
    }
    Token & operator = (Token && other)
    {
        if (this != &other)
        {
            type = std::move(other.type);
            value = std::move(other.value);
        }
        return *this;
    }

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
            case TokenType::None:
                stream << "None"; break;
            case TokenType::SquareBracketOpen:
                stream << "["; break;
            case TokenType::SquareBracketClose:
                stream << "]"; break;
            case TokenType::CurlyBraceOpen:
                stream << "{"; break;
            case TokenType::CurlyBraceClose:
                stream << "}"; break;
            case TokenType::ParenthesisOpen:
                stream << "("; break;
            case TokenType::ParenthesisClose:
                stream << ")"; break;
            case TokenType::Literal:
                stream << "Literal " << value; break;
            case TokenType::Dot:
                stream << "."; break;
            case TokenType::Asterisk:
                stream << "*"; break;
            case TokenType::Plus:
                stream << "+"; break;
            case TokenType::QuestionMark:
                stream << "?"; break;
            case TokenType::Dash:
                stream << "-"; break;
            case TokenType::Or:
                stream << "|"; break;
            case TokenType::Caret:
                stream << "^"; break;
            case TokenType::Space:
                stream << "Space" << value; break;
            case TokenType::Tab:
                stream << "Tab" << value; break;
            case TokenType::NewLine:
                stream << "LF" << value; break;
            case TokenType::Return:
                stream << "CR" << value; break;
            case TokenType::Digit:
                stream << "Digit"; break;
            case TokenType::NotDigit:
                stream << "~Digit"; break;
            case TokenType::WordChar:
                stream << "Wordchar"; break;
            case TokenType::NotWordChar:
                stream << "~Wordchar"; break;
            case TokenType::AlphaChar:
                stream << "AlphaChar"; break;
            case TokenType::NotAlphaChar:
                stream << "~AlphaChar"; break;
            case TokenType::WhitespaceChar:
                stream << "Whitespace"; break;
            case TokenType::NotWhitespaceChar:
                stream << "~Whitespace"; break;
        }
    }
};

inline void PrintTo(const Token & token, std::ostream & stream)
{
    token.PrintTo(stream);
}

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
    Tokenizer(Tokenizer && other)
        : _tokens(std::move(other._tokens))
        , _reader(std::move(other._reader))
    {}
    Tokenizer & operator = (const Tokenizer & other)
    {
        if (this != &other)
        {
            _tokens = other._tokens;
            _reader = other._reader;
        }
        return *this;
    }

    bool Tokenize();
    const TokenList & GetTokens() const { return _tokens; }
    TokenIterator GetTokenIterator() const { return _tokens; }

protected:
    TokenList _tokens;
    InputReader * _reader;

    bool ReadToken(Token & token);
};

} // namespace Regex

inline std::ostream & operator << (std::ostream & stream, const Regex::TokenType & type)
{
    switch (type)
    {
        case Regex::TokenType::None:
            stream << "None"; break;
        case Regex::TokenType::SquareBracketOpen:
            stream << "["; break;
        case Regex::TokenType::SquareBracketClose:
            stream << "]"; break;
        case Regex::TokenType::CurlyBraceOpen:
            stream << "{"; break;
        case Regex::TokenType::CurlyBraceClose:
            stream << "}"; break;
        case Regex::TokenType::ParenthesisOpen:
            stream << "("; break;
        case Regex::TokenType::ParenthesisClose:
            stream << ")"; break;
        case Regex::TokenType::Literal:
            stream << "Literal"; break;
        case Regex::TokenType::Asterisk:
            stream << "*"; break;
        case Regex::TokenType::Dot:
            stream << "."; break;
        case Regex::TokenType::Plus:
            stream << "-"; break;
        case Regex::TokenType::QuestionMark:
            stream << "?"; break;
        case Regex::TokenType::Dash:
            stream << "-"; break;
        case Regex::TokenType::Or:
            stream << "|"; break;
        case Regex::TokenType::Caret:
            stream << "^"; break;
        case Regex::TokenType::Space:
            stream << "Whitespace"; break;
        case Regex::TokenType::Tab:
            stream << "Tab"; break;
        case Regex::TokenType::NewLine:
            stream << "Newline"; break;
        case Regex::TokenType::Return:
            stream << "Return"; break;
        case Regex::TokenType::Digit:
            stream << "Digit"; break;
        case Regex::TokenType::NotDigit:
            stream << "~Digit"; break;
        case Regex::TokenType::WordChar:
            stream << "Wordchar"; break;
        case Regex::TokenType::NotWordChar:
            stream << "~Wordchar"; break;
        case Regex::TokenType::AlphaChar:
            stream << "AlphaChar"; break;
        case Regex::TokenType::NotAlphaChar:
            stream << "~AlphaChar"; break;
        case Regex::TokenType::WhitespaceChar:
            stream << "Whitespace"; break;
        case Regex::TokenType::NotWhitespaceChar:
            stream << "~Whitespace"; break;
    }
    return stream;
}

inline std::ostream & operator << (std::ostream & stream, const Regex::Token & token)
{
    token.PrintTo(stream);
    return stream;
}

namespace Regex {

inline void PrintTo(const TokenType & type, std::ostream & stream)
{
    stream << type;
}

} // namespace Regex
