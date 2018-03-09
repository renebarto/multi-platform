#pragma once

#include <string>
#include "FSA.h"
#include "CharSet.h"

namespace RE {

class Term
{
public:
    enum TermType
    {
        None,
        Literal,
        OneOf,
        OneNotOf,
        Digit,
        NotDigit,
        WordChar,
        NotWordChar,
        Whitespace,
        NotWhitespace,
    };
    Term()
        : _type(TermType::None)
        , _charSet()
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(TermType type)
        : _type(type)
        , _charSet()
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(TermType type, char ch)
        : _type(type)
        , _charSet(ch)
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(TermType type, CharSet charSet)
        : _type(type)
        , _charSet(charSet)
        , _minCount(1)
        , _maxCount(1)
    {}
    TermType Type() const { return _type; }
    void SetMinMaxCount(int minCount, int maxCount)
    {
        _minCount = minCount;
        _maxCount = maxCount;
    }

    void PrintTo(std::ostream & stream) const
    {
        switch (_type)
        {
            case None:
                stream << "Invalid term"; break;
            case Literal:
                stream << "Literal " << _charSet.First(); break;
            case OneOf:
                stream << "OneOf"; break;
            case OneNotOf:
                stream << "OneNotOf"; break;
            case Digit:
                stream << "Digit"; break;
            case NotDigit:
                stream << "NotDigit"; break;
            case WordChar:
                stream << "WordChar"; break;
            case NotWordChar:
                stream << "NotWordChar"; break;
            case Whitespace:
                stream << "Whitespace"; break;
            case NotWhitespace:
                stream << "NotWhitespace"; break;
        }
        stream << " " << "{" << _minCount << "," << _maxCount << "}";
        stream << std::endl;
    }

protected:
    TermType _type;
    CharSet _charSet;
    int _minCount;
    int _maxCount;
};

using Terms = std::vector<Term>;

class Regex
{
public:
    const int StartState = 1;
    const int EndState = 0;
    const int ErrorState = -1;
    Regex()
        : _pattern()
        , _regexTerms()
        , _currentTerm()
        , _currentFSMState()
        , _fsmRules()
        , _fsm(_fsmRules, StartState, EndState)
    {}
    explicit Regex(const std::string & pattern);
    bool Match(const std::string & text);
    bool PartialMatch(const std::string & text);

    void PrintTo(std::ostream & stream) const
    {
        for (auto const & term : _regexTerms)
        {
            term.PrintTo(stream);
        }
    }

protected:
    std::string _pattern;
    Terms _regexTerms;
    Term _currentTerm;
    int _currentFSMState;
    FSARuleSet<char, int, CharSet> _fsmRules;
    FSA<char, int, CharSet> _fsm;

    bool ParseEscapedChar(const std::string & pattern, size_t & index, char & ch);
    bool ParseLiteral(const std::string & pattern, size_t & index);
    bool Parse(const std::string & pattern, size_t & index);
    bool BuildFSM();
    void CreateDigitTerm();
    void CreateNonDigitTerm();
    void CreateWordCharTerm();
    void CreateNonWordCharTerm();
    void CreateWhitespaceTerm();
    void CreateNonWhitespaceTerm();
    void CreateTabTerm();
    void CreateCarriageReturnTerm();
    void CreateLineBreakTerm();
    void CreateNonLineBreakTerm();
    void CreateLineFeedTerm();
    void CreateVerticalTabTerm();
    void CreateNonVerticalTabTerm();
    void CreateLiteralTerm(char ch);
    void ChangeLastTermRepeat(int minCount, int maxCount);
    void SaveCurrentTerm();
    void AddCurrentTerm();
    bool Match(const std::string & text, size_t & index);
};

std::ostream & operator << (std::ostream & stream, const Term & term)
{
    term.PrintTo(stream);
    return stream;
}

std::ostream & operator << (std::ostream & stream, const Regex & regex)
{
    regex.PrintTo(stream);
    return stream;
}

} // namespace RE
