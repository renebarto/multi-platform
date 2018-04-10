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

class Regex
{
public:
    using NFAState = int;
    using DFAState = int;
    using NonDeterministicFSA = NFA<NFAState, char, int, CharSet>;
    using DeterministicFSA = DFA<DFAState, char, int, CharSet>;
    const NFAState StartState = 1;
    const NFAState EndState = 0;
    const DFAState DFAStartState = 1;
    const DFAState DFAEndState = 0;
    const DFAState DFAErrorState = -1;
    explicit Regex(const std::string & pattern);
    bool Match(const std::string & text);
    bool PartialMatch(const std::string & text);

    const AST & GetAST() const { return _ast; }
    const NonDeterministicFSA  & GetNFA() const { return _nfa; }
    const DeterministicFSA  & GetDFA() const { return _dfa; }
    void PrintTo(std::ostream & stream, size_t indent = 0) const
    {
        _ast.PrintTo(stream, indent);
    }

protected:
    InputReader _reader;
    Tokenizer _tokenizer;
    AST _ast;
    NonDeterministicFSA _nfa;
    DeterministicFSA _dfa;

    bool ParseRegex();
    bool ConvertASTToNFA();
    bool ConvertNFAToDFA();
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
    bool Match(const std::string & text, size_t & index);

    bool NFARuleOverlaps(NFAState startState, char ch);
};

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, const RE::Regex & regex)
{
    regex.PrintTo(stream);
    return stream;
}

