#pragma once

#include <string>
#include "re/DFA.h"
#include "re/CharSet.h"
#include "re/InputReader.h"
#include "re/Tokenizer.h"
#include "re/Term.h"
#include "re/AST.h"

namespace RE {

class Regex
{
public:
    const int StartState = 1;
    const int EndState = 0;
    const int ErrorState = -1;
    explicit Regex(const std::string & pattern);
    bool Match(const std::string & text);
    bool PartialMatch(const std::string & text);

    const AST & GetAST() const { return _ast; }
    void PrintTo(std::ostream & stream, size_t indent = 0) const
    {
        _ast.PrintTo(stream, indent);
    }

protected:
    InputReader _reader;
    Tokenizer _tokenizer;
    AST _ast;

    bool ParseRegex();
    bool Have(const TokenIterator & it, TokenType type);
    bool Expect(TokenIterator & it, TokenType type);
    void AddNodeAndCheckMultiplicity(TokenIterator & it, ASTNode::Ptr & root, const ASTNode::Ptr & node);
    ASTNode::Ptr ParseAlternative(TokenIterator & it);
    ASTNode::Ptr ParseRange(TokenIterator & it);
    Term CreateDigitElement();
    Term CreateNonDigitElement();
    Term CreateWordCharElement();
    Term CreateNonWordCharElement();
    Term CreateWhitespaceElement();
    Term CreateNonWhitespaceElement();
    Term CreateTabElement();
    Term CreateCarriageReturnElement();
    Term CreateLineBreakElement();
    Term CreateNonLineBreakElement();
    Term CreateLineFeedElement();
    Term CreateVerticalTabElement();
    Term CreateNonVerticalTabElement();
    Term CreateLiteralElement(char ch);
    Term CreateLiteralElement(CharSet charSet);
    void ChangeRepeat(Term & element, int minCount, int maxCount);
    void ChangeLastTermRepeat(int minCount, int maxCount);
    Term CreateTerm();
    bool Match(const std::string & text, size_t & index);

};

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, const RE::Regex & regex)
{
    regex.PrintTo(stream);
    return stream;
}

