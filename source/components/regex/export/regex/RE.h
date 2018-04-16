#pragma once

#include <string>
#include "regex/DFA.h"
#include "regex/CharSet.h"
#include "regex/InputReader.h"
#include "regex/Tokenizer.h"
#include "regex/Term.h"
#include "regex/AST.h"
#include "regex/NFA.h"
#include "regex/DFA.h"

namespace Regex {

class StringMatch : public NFA<int, char, int, CharSet>
{
public:
    using State = int;

    StringMatch(State startState, State endState, State errorState)
        : NFA(startState, endState, errorState)
    {}
    StringMatch(const RuleSet & rules, State startState, State endState, State errorState)
        : NFA(rules, startState, endState, errorState)
    {}

    bool Match(const std::string & input)
    {
        Reset();
        for (auto ch : input)
        {
            HandleInput(ch);
        }
        return HasFinalState();
    }
    bool PartialMatch(const std::string & input)
    {
        Reset();
        for (auto ch : input)
        {
            HandleInput(ch);
        }
        return CurrentStates() != StringMatch::StateSet {_errorState};
    }
};

class RE
{
public:
    using State = StringMatch::State;
    static constexpr State StartState = 1;
    static constexpr State EndState = 0;
    static constexpr State TempEndState = 65536;
    static constexpr State ErrorState = -1;

    explicit RE(const std::string & pattern);
    RE(const RE & other);
    RE & operator = (const RE & other);

    bool Match(const std::string & text) const;
    bool PartialMatch(const std::string & text) const;

    const AST & GetAST() const { return _ast; }
    const StringMatch & GetNFA() const { return _nfa; }
    void PrintTo(std::ostream & stream, size_t indent = 0) const
    {
        _ast.PrintTo(stream, indent);
    }

protected:
    std::string _pattern;
    InputReader _reader;
    Tokenizer _tokenizer;
    AST _ast;
    mutable StringMatch _nfa;

    bool ParseRegex();
    bool ConvertASTToNFA();
    bool ConvertASTToNFA(ASTNode::Ptr base, State startState, State endState, State & actualEndState);
    bool Have(const TokenIterator & it, TokenType type);
    bool Have(const TokenIterator & it, const TokenTypeSet & types);
    bool Expect(TokenIterator & it, TokenType type);
    bool Expect(TokenIterator & it, const TokenTypeSet & types);
    void AddNodeAndCheckMultiplicity(TokenIterator & it, ASTNode::Ptr & root, const ASTNode::Ptr & node);
    ASTNode::Ptr ParseExpression(TokenIterator & it, const TokenTypeSet & terminators);
    ASTNode::Ptr ParseAlternative(TokenIterator & it, const TokenTypeSet & terminators);
    ASTNode::Ptr ParseRange(TokenIterator & it);
    Term CreateDigitElement();
    Term CreateNotDigitElement();
    Term CreateWordCharElement();
    Term CreateNotWordCharElement();
    Term CreateAlphaCharElement();
    Term CreateNotAlphaCharElement();
    Term CreateWhitespaceCharElement();
    Term CreateNotWhitespaceCharElement();
    Term CreateLiteralElement(char ch);
    Term CreateLiteralElement(CharSet charSet);
};

} // namespace Regex

inline std::ostream & operator << (std::ostream & stream, const Regex::RE & regex)
{
    regex.PrintTo(stream);
    return stream;
}

