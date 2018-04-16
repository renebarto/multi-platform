#include "parser/Lexer.h"

#include <iostream>

using namespace std;
using namespace Parser;

Lexer::Lexer(InputReader & reader)
    : _reader(reader)
    , _rules()
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
    SourceLocation location = _reader.Location();
    string value;
    const LexerRule * lastMatch {};
    while (!_reader.IsEOF())
    {
        char ch = _reader.ReadAheadChar();

        const LexerRule * rule;

        string newValue = value + ch;
        if (PartialMatch(newValue))
        {
            value = newValue;
            _reader.ReadChar();
            if (FullMatch(value, rule))
            {
                lastMatch = rule;
            }
            continue;
        }
        if (!lastMatch)
        {
            cerr << location << ": No rule matching " << ch << endl;
            value = newValue;
            _reader.ReadChar();
            return LexerToken(LexerToken::Type::None, value, location);
        }
        return LexerToken(lastMatch->Type(), value, location);
    }
    if (lastMatch)
    {
        return LexerToken(lastMatch->Type(), value, location);
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

bool Lexer::FullMatch(const std::string & value, const LexerRule *& activeRule)
{
    activeRule = nullptr;
    for (auto const & rule : _rules)
    {
        if (rule.Match(value))
        {
            activeRule = &rule;
            return true;
        }
    }
    return false;
}

bool Lexer::PartialMatch(const std::string & value)
{
    for (auto const & rule : _rules)
    {
        if (rule.PartialMatch(value))
        {
            return true;
        }
    }
    return false;
}

