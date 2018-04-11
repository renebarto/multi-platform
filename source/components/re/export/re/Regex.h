#pragma once

#include <string>
#include "re/DFA.h"
#include "re/CharSet.h"
#include "re/InputReader.h"
#include "re/Tokenizer.h"
#include "re/Term.h"
#include "re/AST.h"
#include "re/NFA.h"
#include "re/DFA.h"

namespace RE {

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

class Regex
{
public:
    using State = StringMatch::State;
    static constexpr State StartState = 1;
    static constexpr State EndState = 0;
    static constexpr State ErrorState = -1;

    explicit Regex(const std::string & pattern);
    bool Match(const std::string & text);
    bool PartialMatch(const std::string & text);

    const AST & GetAST() const { return _ast; }
    const StringMatch & GetNFA() const { return _nfa; }
    void PrintTo(std::ostream & stream, size_t indent = 0) const
    {
        _ast.PrintTo(stream, indent);
    }

protected:
    InputReader _reader;
    Tokenizer _tokenizer;
    AST _ast;
    StringMatch _nfa;

    bool ParseRegex();
    bool ConvertASTToNFA();
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

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, const RE::Regex & regex)
{
    regex.PrintTo(stream);
    return stream;
}

