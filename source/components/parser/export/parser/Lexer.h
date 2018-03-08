#pragma once

#include <deque>
#include <string>
#include <vector>
#include "parser/InputReader.h"

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
    LexerToken(LexerToken::Type aType, FSA aLocation)
        : type(aType)
        , value()
        , location(aLocation)
    {}
    LexerToken(LexerToken::Type aType, const std::string & aValue, FSA aLocation)
        : type(aType)
        , value(aValue)
        , location(aLocation)
    {}
    bool IsValid() const { return type != Type::None; }

    Type type;
    std::string value;
    FSA location;
};

enum LexerState
{
    Any = 0,
    LineComment,
    BlockComment,
    Whitespace,
    Identifier,
    HexNumber,
    OctNumber,
    DecNumber,
    String,
};

class LexerRule
{
public:
    LexerRule() = delete;

    static LexerRule CreateSingleChar(const char * inCharSet,
                                      LexerToken::Type type)
    {
        return LexerRule(inCharSet, nullptr, nullptr, nullptr,
                         LexerState::Any, LexerState::Any, LexerState::Any, type,
                         true, true, false);
    }
    static LexerRule CreateStartCharSet(const char * inCharSet,
                                        LexerState nextState,
                                        bool storeInput)
    {
        return LexerRule(inCharSet, nullptr, nullptr, nullptr, 
                         LexerState::Any, nextState, nextState, LexerToken::Type::None,
                         storeInput, false, false);
    }
    static LexerRule CreateContinueCharSet(const char * inCharSet,
                                           LexerState currentState,
                                           LexerToken::Type type,
                                           bool storeInput)
    {
        return LexerRule(inCharSet, nullptr, nullptr, nullptr,
                         currentState, currentState, LexerState::Any, type,
                         storeInput, false, true);
    }
    static LexerRule CreateEndCharSet(const char * endCharSet,
                                      LexerState currentState,
                                      LexerToken::Type type,
                                      bool storeInput)
    {
        return LexerRule(nullptr, endCharSet, nullptr, nullptr, 
                         currentState, currentState, LexerState::Any, type,
                         storeInput, false, true);
    }
    static LexerRule CreateStartLiteral(const char * literal,
                                        LexerState nextState,
                                        bool storeInput)
    {
        return LexerRule(nullptr, nullptr, literal, nullptr,
                         LexerState::Any, nextState, nextState, LexerToken::Type::None,
                         storeInput, false, false);
    }
    static LexerRule CreateEndLiteral(const char * literal,
                                      LexerState currentState,
                                      LexerToken::Type type,
                                      bool storeInput)
    {
        return LexerRule(nullptr, nullptr, nullptr, literal,
                         currentState, currentState, LexerState::Any, type,
                         storeInput, false, true);
    }

    static LexerRule CreateEndLiteral(const char * endLiteral,
                                      LexerState currentState, LexerState foundState, LexerState lostState,
                                      LexerToken::Type type,
                                      bool storeInput, bool emitWhenFound, bool emitWhenLost)
    {
        return LexerRule(nullptr, nullptr, nullptr, endLiteral, 
                         currentState, foundState, lostState, type, 
                         storeInput, emitWhenFound, emitWhenLost);
    }

    bool MatchesChar(char ch, LexerState state) const
    {
        if (state != _currentState)
            return false;
        if (_inCharSet != nullptr)
        {
            if (strchr(_inCharSet, ch) != nullptr)
                return true;
        }
        return false;
    }
    bool MatchesEndChar(char ch, LexerState state) const
    {
        if (state != _currentState)
            return false;
        if (_endCharSet != nullptr)
        {
            if (strchr(_endCharSet, ch) == nullptr)
                return true;
        }
        return false;
    }
    bool MatchesLiteral(const std::string & literal, LexerState state) const
    {
        if (state != _currentState)
            return false;
        if (_literal != nullptr)
        {
            if (strcmp(_literal, literal.c_str()) == 0)
                return true;
        }
        return false;
    }
    bool MatchesEndLiteral(const std::string & literal, LexerState state) const
    {
        if (state != _currentState)
            return false;
        if (_endLiteral != nullptr)
        {
            if (strcmp(_endLiteral, literal.c_str()) != 0)
                return true;
        }
        return false;
    }
    LexerState StartState() const
    {
        return _currentState;
    }
    LexerState FoundState() const
    {
        return _foundState;
    }
    LexerState LostState() const
    {
        return _lostState;
    }
    bool StoreInput() const
    {
        return _storeInput;
    }
    bool EmitOnFound() const
    {
        return (_type != LexerToken::Type::None) && _emitWhenFound;
    }
    bool EmitOnLost() const
    {
        return (_type != LexerToken::Type::None) && _emitWhenLost;
    }
    LexerToken::Type Type() const
    {
        return _type;
    }
    bool IsLiteral() const
    {
        return (_literal != nullptr);
    }
    bool IsEndLiteral() const
    {
        return (_endLiteral != nullptr);
    }
    size_t LiteralSize() const
    {
        return (_literal != nullptr) ? strlen(_literal) : 0;
    }
    size_t EndLiteralSize() const
    {
        return (_endLiteral != nullptr) ? strlen(_endLiteral) : 0;
    }
    const char * MatchChars() const
    {
        return _inCharSet;
    }
    const char * EndMatchChars() const
    {
        return _endCharSet;
    }
    const char * Literal() const
    {
        return _literal;
    }
    const char * EndLiteral() const
    {
        return _endLiteral;
    }

protected:
    const char * _inCharSet;
    const char * _endCharSet;
    const char * _literal;
    const char * _endLiteral;
    LexerState _currentState;
    LexerState _foundState;
    LexerState _lostState;
    LexerToken::Type _type;
    bool _storeInput;
    bool _emitWhenFound;
    bool _emitWhenLost;

public:
    LexerRule(const char * inCharSet, const char * endCharSet, const char * literal, const char * endLiteral,
              LexerState currentState, LexerState foundState, LexerState lostState,
              LexerToken::Type type,
              bool storeInput, bool emitWhenFound, bool emitWhenLost)
        : _inCharSet(inCharSet)
        , _endCharSet(endCharSet)
        , _literal(literal)
        , _endLiteral(endLiteral)
        , _currentState(currentState)
        , _foundState(foundState)
        , _lostState(lostState)
        , _type(type)
        , _storeInput(storeInput)
        , _emitWhenFound(emitWhenFound)
        , _emitWhenLost(emitWhenLost)
    {}
    LexerRule(const LexerRule & other)
        : _inCharSet(other._inCharSet)
        , _endCharSet(other._endCharSet)
        , _literal(other._literal)
        , _endLiteral(other._endLiteral)
        , _currentState(other._currentState)
        , _foundState(other._foundState)
        , _lostState(other._lostState)
        , _type(other._type)
        , _storeInput(other._storeInput)
        , _emitWhenFound(other._emitWhenFound)
        , _emitWhenLost(other._emitWhenLost)
    {
    }
    LexerRule & operator = (const LexerRule & other)
    {
        if (this != &other)
        {
            _inCharSet = other._inCharSet;
            _endCharSet = other._endCharSet;
            _literal = other._literal;
            _endLiteral = other._endLiteral;
            _currentState = other._currentState;
            _foundState = other._foundState;
            _lostState = other._lostState;
            _type = other._type;
            _storeInput = other._storeInput;
            _emitWhenFound = other._emitWhenFound;
            _emitWhenLost = other._emitWhenLost;
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
    const LexerRule * _activeRule;
    std::deque<LexerToken> _pushbackQueue;

    const LexerRule * FindMatchingRule(char ch, const LexerState & state, std::string & value);
    bool ActiveRuleMatches(char ch, const LexerState & state, std::string & value);
};

} // namespace Parser

inline std::ostream & operator << (std::ostream & stream, const Parser::LexerState & state)
{
    switch (state)
    {
        case Parser::LexerState::Any:
            stream << "Any"; break;
        case Parser::LexerState::LineComment:
            stream << "LineComment"; break;
        case Parser::LexerState::BlockComment:
            stream << "BlockComment"; break;
        case Parser::LexerState::Whitespace:
            stream << "Whitespace"; break;
        case Parser::LexerState::Identifier:
            stream << "Identifier"; break;
        case Parser::LexerState::HexNumber:
            stream << "HexNumber"; break;
        case Parser::LexerState::OctNumber:
            stream << "OctNumber"; break;
        case Parser::LexerState::DecNumber:
            stream << "DecNumber"; break;
        case Parser::LexerState::String:
            stream << "String"; break;
        default:
            stream << "Unknown state";
    }
    return stream;
}
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
    if (rule.MatchChars() != nullptr)
        stream << "Match      " << rule.MatchChars() << std::endl;
    if (rule.EndMatchChars() != nullptr)
        stream << "EndMatch   " << rule.EndMatchChars() << std::endl;
    if (rule.Literal() != nullptr)
        stream << "Literal    " << rule.Literal() << std::endl;
    if (rule.EndLiteral() != nullptr)
        stream << "EndLiteral " << rule.EndLiteral() << std::endl;
    stream << rule.StartState() << " -> " << rule.FoundState() << " / " << rule.LostState() << std::endl;
    stream << "Type:      " << rule.Type() << std::endl;
    stream << "StoreInput:" << rule.StoreInput() << std::endl;
    stream << "Emit found:" << rule.EmitOnFound() << std::endl;
    stream << "Emit lost: " << rule.EmitOnLost() << std::endl;
    return stream;
}
