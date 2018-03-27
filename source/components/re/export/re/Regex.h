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
    using State = int;
    using NonDeterministicFSA = NFA<State, char, int, CharSet>;
    using DeterministicFSA = DFA<State, char, int, CharSet>;
    const State StartState = 1;
    const State EndState = 0;
    const State ErrorState = -1;
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
    bool Expect(TokenIterator & it, TokenType type);
    void AddNodeAndCheckMultiplicity(TokenIterator & it, ASTNode::Ptr & root, const ASTNode::Ptr & node);
    ASTNode::Ptr ParseAlternative(TokenIterator & it);
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
};

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, const RE::Regex & regex)
{
    regex.PrintTo(stream);
    return stream;
}

