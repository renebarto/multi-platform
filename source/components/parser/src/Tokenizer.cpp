#include "parser/Tokenizer.h"

#include <iostream>

using namespace std;
using namespace Parser;

Tokenizer::Tokenizer(const std::string & path, std::istream & stream)
    : _stream(stream)
    , _tokens()
    , _reader(path, stream)
    , _lexer(_reader)
    , _rules()
{
}

void Tokenizer::SetLexerRules(const LexerRuleSet & rules)
{
    _lexer.SetRules(rules);
}

void Tokenizer::SetRules(const TokenizerRuleSet & rules)
{
    _rules = rules;
}

bool Tokenizer::Tokenize()
{
    Token currentToken;

    while (ReadToken(currentToken))
    {
        _tokens.push_back(currentToken);
//        cout << currentToken << endl;
    }
//    cout << currentToken << endl;
    return true;
}

bool Tokenizer::ReadToken(Token & token)
{
    auto lexerToken = _lexer.ReadToken();
    for (auto const & rule : _rules)
    {
        if (rule.MatchFirst(lexerToken.type))
        {
            if (rule.LookAheadType() != LexerToken::Type::None)
            {
                auto lookAheadToken = _lexer.ReadAheadToken();
                if (rule.Match(lexerToken.type, lookAheadToken.type, lexerToken.value))
                {
                    token = Token(rule.OutType(), lexerToken.value + lookAheadToken.value, lexerToken.location);
                    _lexer.ReadToken();
                    return rule.Result();
                }
            }
            else if (rule.Match(lexerToken.type, lexerToken.value))
            {
                token = Token(rule.OutType(), lexerToken.value, lexerToken.location);
                return rule.Result();
            }
        }
    }
    return false;
}