#pragma once

#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include "parser/Lexer.h"
#include "parser/SourceLocation.h"

namespace Parser {

enum class TokenType
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
    Ampersand,
    Comma,
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
    Namespace,
    InterfaceID,
    Interface,
    ParamIn,
    ParamOut,
    ParamInOut,
    ParamSizeIs,
    QualifierConst,
    VoidType,
    StringType,
    SInt8Type,
    SInt16Type,
    SInt32Type,
    SInt64Type,
    UInt8Type,
    UInt16Type,
    UInt32Type,
    UInt64Type,
    ScopeResolution,
};

struct Token
{
    Token()
        : type()
        , value()
        , location()
    {}
    Token(TokenType aType, FSA aLocation)
        : type(aType)
        , value()
        , location(aLocation)
    {}
    Token(TokenType aType, const std::string & aValue, FSA aLocation)
        : type(aType)
        , value(aValue)
        , location(aLocation)
    {}
    TokenType type;
    std::string value;
    FSA location;
};

using TokenList = std::vector<Token>;

class TokenizerRule
{
public:
    TokenizerRule() = delete;

    static TokenizerRule CreateSimple(LexerToken::Type inType, TokenType outType)
    {
        return TokenizerRule(inType, LexerToken::Type::None, "", outType, true);
    }
    static TokenizerRule CreateCombi(LexerToken::Type inType, LexerToken::Type lookAheadType, TokenType outType)
    {
        return TokenizerRule(inType, lookAheadType, "", outType, true);
    }
    static TokenizerRule CreateEOF()
    {
        return TokenizerRule(LexerToken::Type::Eof, LexerToken::Type::None, "", TokenType::Eof, false);
    }
    static TokenizerRule CreateLookup(LexerToken::Type inType, const std::string & value, TokenType outType)
    {
        return TokenizerRule(inType, LexerToken::Type::None, value, outType, true);
    }

    bool MatchFirst(LexerToken::Type type) const
    {
        return (type == _inType);
    }
    bool Match(LexerToken::Type type, const std::string & value) const
    {
        return (type == _inType) && (_lookAheadType == LexerToken::Type::None) && (_lookupValue.empty() || (value == _lookupValue));
    }
    bool Match(LexerToken::Type type, LexerToken::Type lookAheadType, const std::string & value) const
    {
        return (type == _inType) && (lookAheadType == _lookAheadType) && (_lookupValue.empty() || (value == _lookupValue));
    }
    LexerToken::Type InType() const { return _inType; }
    LexerToken::Type LookAheadType() const { return _lookAheadType; }
    const std::string & LookupValue() const { return _lookupValue; }
    TokenType OutType() const { return _outType; }
    bool Result() const { return _result; }

protected:
    LexerToken::Type _inType;
    LexerToken::Type _lookAheadType;
    std::string _lookupValue;
    TokenType _outType;
    bool _result;

public:
    TokenizerRule(LexerToken::Type inType, LexerToken::Type lookAheadType, const std::string & lookupValue,
                  TokenType outType, bool result)
        : _inType(inType)
        , _lookAheadType(lookAheadType)
        , _lookupValue(lookupValue)
        , _outType(outType)
        , _result(result)
    {}
    TokenizerRule(const TokenizerRule & other)
        : _inType(other._inType)
        , _lookAheadType(other._lookAheadType)
        , _lookupValue(other._lookupValue)
        , _outType(other._outType)
        , _result(other._result)
    {
    }
    TokenizerRule & operator = (const TokenizerRule & other)
    {
        if (this != &other)
        {
            _inType = other._inType;
            _lookAheadType = other._lookAheadType;
            _lookupValue = other._lookupValue;
            _outType = other._outType;
            _result = other._result;
        }
        return *this;
    }
};

using TokenizerRuleSet = std::vector<TokenizerRule>;

class TokenIterator
{
public:
    TokenIterator(const TokenList & tokens)
        : _current(tokens.begin())
        , _end(tokens.end())
    {}
    bool AtEnd() const { return _current == _end; }
    void Next() { ++_current; }
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
    Tokenizer(const std::string & path, std::istream & stream);

    void SetLexerRules(const LexerRuleSet & rules);
    void SetRules(const TokenizerRuleSet & rules);

    bool Tokenize();
    const TokenList & GetTokens() const { return _tokens; }
    TokenIterator GetTokenIterator() const { return _tokens; }

protected:
    std::istream & _stream;
    TokenList _tokens;
    InputReader _reader;
    Lexer _lexer;
    TokenizerRuleSet _rules;

    bool ReadToken(Token & token);
};

} // namespace Parser

inline std::ostream & operator << (std::ostream & stream, const Parser::TokenType & type)
{
    switch (type)
    {
        case Parser::TokenType::None:
            stream << "None"; break;
        case Parser::TokenType::SquareBracketOpen:
            stream << "["; break;
        case Parser::TokenType::SquareBracketClose:
            stream << "]"; break;
        case Parser::TokenType::CurlyBraceOpen:
            stream << "{"; break;
        case Parser::TokenType::CurlyBraceClose:
            stream << "}"; break;
        case Parser::TokenType::ParenthesisOpen:
            stream << "("; break;
        case Parser::TokenType::ParenthesisClose:
            stream << ")"; break;
        case Parser::TokenType::Asterisk:
            stream << "*"; break;
        case Parser::TokenType::Semicolon:
            stream << ";"; break;
        case Parser::TokenType::Colon:
            stream << ":"; break;
        case Parser::TokenType::Ampersand:
            stream << "&"; break;
        case Parser::TokenType::Comma:
            stream << ","; break;
        case Parser::TokenType::Identifier:
            stream << "Identifier"; break;
        case Parser::TokenType::HexNumber:
            stream << "HexNumber"; break;
        case Parser::TokenType::OctNumber:
            stream << "OctNumber"; break;
        case Parser::TokenType::DecNumber:
            stream << "DecNumber"; break;
        case Parser::TokenType::String:
            stream << "String"; break;
        case Parser::TokenType::Whitespace:
            stream << "Whitespace"; break;
        case Parser::TokenType::NewLine:
            stream << "Newline"; break;
        case Parser::TokenType::LineComment:
            stream << "LineComment"; break;
        case Parser::TokenType::BlockComment:
            stream << "BlockComment"; break;
        case Parser::TokenType::Eof:
            stream << "<EOF>"; break;
        case Parser::TokenType::Namespace:
            stream << "namespace"; break;
        case Parser::TokenType::InterfaceID:
            stream << "id"; break;
        case Parser::TokenType::Interface:
            stream << "interface"; break;
        case Parser::TokenType::ParamIn:
            stream << "in"; break;
        case Parser::TokenType::ParamOut:
            stream << "out"; break;
        case Parser::TokenType::ParamInOut:
            stream << "inout"; break;
        case Parser::TokenType::ParamSizeIs:
            stream << "size_is"; break;
        case Parser::TokenType::QualifierConst:
            stream << "const"; break;
        case Parser::TokenType::VoidType:
            stream << "void"; break;
        case Parser::TokenType::StringType:
            stream << "string"; break;
        case Parser::TokenType::SInt8Type:
            stream << "sint8"; break;
        case Parser::TokenType::SInt16Type:
            stream << "sint16"; break;
        case Parser::TokenType::SInt32Type:
            stream << "sint32"; break;
        case Parser::TokenType::SInt64Type:
            stream << "sint64"; break;
        case Parser::TokenType::UInt8Type:
            stream << "uint8"; break;
        case Parser::TokenType::UInt16Type:
            stream << "uint16"; break;
        case Parser::TokenType::UInt32Type:
            stream << "uint32"; break;
        case Parser::TokenType::UInt64Type:
            stream << "uint64"; break;
        case Parser::TokenType::ScopeResolution:
            stream << "::"; break;
    }
    return stream;
}

inline std::ostream & operator << (std::ostream & stream, const Parser::Token & token)
{
    stream << token.location << ":";
    switch (token.type)
    {
        case Parser::TokenType::None:
            stream << "None"; break;
        case Parser::TokenType::SquareBracketOpen:
            stream << "["; break;
        case Parser::TokenType::SquareBracketClose:
            stream << "]"; break;
        case Parser::TokenType::CurlyBraceOpen:
            stream << "{"; break;
        case Parser::TokenType::CurlyBraceClose:
            stream << "}"; break;
        case Parser::TokenType::ParenthesisOpen:
            stream << "("; break;
        case Parser::TokenType::ParenthesisClose:
            stream << ")"; break;
        case Parser::TokenType::Asterisk:
            stream << "*"; break;
        case Parser::TokenType::Semicolon:
            stream << ";"; break;
        case Parser::TokenType::Colon:
            stream << ":"; break;
        case Parser::TokenType::Ampersand:
            stream << "&"; break;
        case Parser::TokenType::Comma:
            stream << ","; break;
        case Parser::TokenType::Identifier:
            stream << "Identifier " << token.value; break;
        case Parser::TokenType::HexNumber:
            stream << "HexNumber " << token.value; break;
        case Parser::TokenType::OctNumber:
            stream << "OctNumber " << token.value; break;
        case Parser::TokenType::DecNumber:
            stream << "DecNumber " << token.value; break;
        case Parser::TokenType::String:
            stream << "String " << token.value; break;
        case Parser::TokenType::Whitespace:
            stream << "Whitespace " << token.value; break;
        case Parser::TokenType::NewLine:
            stream << "LF" << token.value; break;
        case Parser::TokenType::LineComment:
            stream << "//" << token.value; break;
        case Parser::TokenType::BlockComment:
            stream << "/*" << token.value << "*/"; break;
        case Parser::TokenType::Eof:
            stream << "<EOF>"; break;
        case Parser::TokenType::Namespace:
            stream << "namespace " << token.value; break;
        case Parser::TokenType::InterfaceID:
            stream << "id " << token.value; break;
        case Parser::TokenType::Interface:
            stream << "interface " << token.value; break;
        case Parser::TokenType::ParamIn:
            stream << "in"; break;
        case Parser::TokenType::ParamOut:
            stream << "out"; break;
        case Parser::TokenType::ParamInOut:
            stream << "inout"; break;
        case Parser::TokenType::ParamSizeIs:
            stream << "size_is"; break;
        case Parser::TokenType::QualifierConst:
            stream << "const"; break;
        case Parser::TokenType::VoidType:
            stream << "void"; break;
        case Parser::TokenType::StringType:
            stream << "string"; break;
        case Parser::TokenType::SInt8Type:
            stream << "sint8"; break;
        case Parser::TokenType::SInt16Type:
            stream << "sint16"; break;
        case Parser::TokenType::SInt32Type:
            stream << "sint32"; break;
        case Parser::TokenType::SInt64Type:
            stream << "sint64"; break;
        case Parser::TokenType::UInt8Type:
            stream << "uint8"; break;
        case Parser::TokenType::UInt16Type:
            stream << "uint16"; break;
        case Parser::TokenType::UInt32Type:
            stream << "uint32"; break;
        case Parser::TokenType::UInt64Type:
            stream << "uint64"; break;
        case Parser::TokenType::ScopeResolution:
            stream << "::"; break;
    }
    return stream;
}

