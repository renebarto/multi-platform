#include "parser/Lexer.h"

#include <iostream>

using namespace std;
using namespace Parser;

Lexer::Lexer(InputReader & reader)
    : _reader(reader)
    , _rules()
    , _activeRule()
    , _pushbackQueue()
{
}

void Lexer::SetRules(const LexerRuleSet & rules)
{
    _rules = rules;
}

LexerToken Lexer::ReadToken()
{
    if (_pushbackQueue.size() > 0)
    {
        auto token = _pushbackQueue.front();
        _pushbackQueue.pop_front();
        return token;
    }
    FSA location = _reader.Location();
    string value;
    LexerState state {};
    _activeRule = nullptr;
    while (!_reader.IsEOF())
    {
        char ch = _reader.ReadAheadChar();

        if (ActiveRuleMatches(ch, state, value))
            continue;
        auto savedActiveRule = _activeRule;
        _activeRule = nullptr;
        if (savedActiveRule)
        {
            state = savedActiveRule->LostState();
            if (savedActiveRule->EmitOnLost())
                return LexerToken(savedActiveRule->Type(), value, location);
        }

        const LexerRule * currentRule = FindMatchingRule(ch, state, value);
        if (currentRule)
        {
            state = currentRule->FoundState();
            if (currentRule->EmitOnFound())
            {
                return LexerToken(currentRule->Type(), value, location);
            }
            _activeRule = currentRule;
            continue;
        }
        else
            cerr << location << ": No rule matching " << ch << endl;
        value += ch;
        _reader.ReadChar();
        return LexerToken(LexerToken::Type::None, value, location);
    }
    if (_activeRule)
    {
        if (_activeRule->EmitOnLost())
            return LexerToken(_activeRule->Type(), value, location);
    }
    return LexerToken(LexerToken::Type::Eof, location);
}

LexerToken Lexer::ReadAheadToken()
{
    if (_pushbackQueue.size() > 0)
    {
        return _pushbackQueue.front();
    }
    auto token = ReadToken();
    _pushbackQueue.push_back(token);
    return token;
}

bool Lexer::ActiveRuleMatches(char ch, const LexerState & state, std::string & value)
{
    if (!_activeRule)
        return false;
    if (_activeRule->MatchesChar(ch, state)) {
//        cout << _reader.Location() << ": Match: " << *_activeRule;
        if (_activeRule->StoreInput())
            value += ch;
        _reader.ReadChar();
        return true;
    } else if (_activeRule->MatchesEndChar(ch, state))
    {
//        cout << _reader.Location() << ": Match: " << *_activeRule;
        if (_activeRule->StoreInput())
            value += ch;
        _reader.ReadChar();
        return true;
    } else if (_activeRule->IsLiteral())
    {
        string literal;
        literal = _reader.ReadAheadChars(_activeRule->LiteralSize());
        if (_activeRule->MatchesLiteral(literal, state))
        {
            if (_activeRule->StoreInput())
                value += literal;
            _reader.ReadChars(_activeRule->LiteralSize());
//            cout << _reader.Location() << ": Match: " << *_activeRule;
            return true;
        }
    }
    else if (_activeRule->IsEndLiteral())
    {
        string literal;
        literal = _reader.ReadAheadChars(_activeRule->EndLiteralSize());
        if (_activeRule->MatchesEndLiteral(literal, state))
        {
            if (_activeRule->StoreInput())
                value += ch;
            _reader.ReadChar();
//            cout << _reader.Location() << ": Match: " << *_activeRule;
            return true;
        }
        else
        {
            // We found the ending literal, eat it
            _reader.ReadChars(_activeRule->EndLiteralSize());
        }
    }
    return false;
}

const LexerRule * Lexer::FindMatchingRule(char ch, const LexerState & state, std::string & value)
{
    for (auto const & rule : _rules)
    {
        if (rule.MatchesChar(ch, state))
        {
//            cout << _reader.Location() << ": Match: " << rule;
            if (rule.StoreInput())
                value += ch;
            _reader.ReadChar();
            return &rule;
        } else if (rule.MatchesEndChar(ch, state))
        {
//            cout << _reader.Location() << ": Match: " << rule;
            if (rule.StoreInput())
                value += ch;
            _reader.ReadChar();
            return &rule;
        } else if (rule.IsLiteral())
        {
            string literal;
            literal = _reader.ReadAheadChars(rule.LiteralSize());
            if (rule.MatchesLiteral(literal, state))
            {
                if (rule.StoreInput())
                    value += literal;
                _reader.ReadChars(rule.LiteralSize());
//                cout << _reader.Location() << ": Match: " << rule;
                return &rule;
            }
        }
        else if (rule.IsEndLiteral())
        {
            string literal;
            literal = _reader.ReadAheadChars(rule.EndLiteralSize());
            if (rule.MatchesEndLiteral(literal, state))
            {
                if (rule.StoreInput())
                    value += ch;
                _reader.ReadChar();
//                cout << _reader.Location() << ": Match: " << rule;
                return &rule;
            }
        }
    }
    return nullptr;
}
