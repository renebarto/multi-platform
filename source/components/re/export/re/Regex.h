#pragma once

#include <string>
#include "FSA.h"
#include "CharSet.h"

namespace RE {

class TermElement
{
public:
    enum Type
    {
        None,
        Literal,
        Set,
        NotSet,
        Digit,
        NotDigit,
        WordChar,
        NotWordChar,
        Whitespace,
        NotWhitespace,
    };
    TermElement()
        : _type(Type::None)
        , _charSet()
        , _minCount(1)
        , _maxCount(1)
    {}
    TermElement(Type type)
        : _type(type)
        , _charSet()
        , _minCount(1)
        , _maxCount(1)
    {}
    TermElement(Type type, char ch)
        : _type(type)
        , _charSet(ch)
        , _minCount(1)
        , _maxCount(1)
    {}
    TermElement(Type type, CharSet charSet)
        : _type(type)
        , _charSet(charSet)
        , _minCount(1)
        , _maxCount(1)
    {}
    TermElement(Type type, CharSet charSet, int minCount, int maxCount)
        : _type(type)
        , _charSet(charSet)
        , _minCount(minCount)
        , _maxCount(maxCount)
    {}
    Type GetType() const { return _type; }
    void SetMinMaxCount(int minCount, int maxCount)
    {
        _minCount = minCount;
        _maxCount = maxCount;
    }

    friend bool operator == (const TermElement & lhs, const TermElement & rhs)
    {
        return (lhs._type == rhs._type) &&
               (lhs._charSet == rhs._charSet) &&
               (lhs._minCount == rhs._minCount) &&
               (lhs._maxCount == rhs._maxCount);
    }
    friend bool operator != (const TermElement & lhs, const TermElement & rhs)
    {
        return ! operator == (lhs, rhs);
    }
    void PrintTo(std::ostream & stream) const
    {
        switch (_type)
        {
            case None:
                stream << "Invalid term"; break;
            case Literal:
                stream << "Literal " << _charSet; break;
            case Set:
                stream << "OneOf " << _charSet; break;
            case NotSet:
                stream << "OneNotOf " << _charSet; break;
            case Digit:
                stream << "Digit " << _charSet; break;
            case NotDigit:
                stream << "NotDigit " << _charSet; break;
            case WordChar:
                stream << "WordChar " << _charSet; break;
            case NotWordChar:
                stream << "NotWordChar " << _charSet; break;
            case Whitespace:
                stream << "Whitespace " << _charSet; break;
            case NotWhitespace:
                stream << "NotWhitespace " << _charSet; break;
        }
        stream << " " << "{" << _minCount << "," << _maxCount << "}";
        stream << std::endl;
    }

protected:
    Type _type;
    CharSet _charSet;
    int _minCount;
    int _maxCount;
};

using TermElements = std::vector<TermElement>;

class Term
{
public:
    Term()
        : _elements()
        , _subTerms()
    {}
    Term(const TermElement & element)
        : _elements()
        , _subTerms()
    {
        Add(element);
    }
    Term(TermElement && element)
        : _elements()
        , _subTerms()
    {
        Add(std::move(element));
    }
    void Add(const TermElement & element)
    {
        _elements.push_back(element);
    }
    void Add(TermElement && element)
    {
        _elements.push_back(std::move(element));
    }
    void Add(const Term & subTerm)
    {
        _subTerms.push_back(subTerm);
    }
    void Add(Term && subTerm)
    {
        _subTerms.push_back(std::move(subTerm));
    }
    bool Empty() const
    {
        return (_elements.empty() && _subTerms.empty());
    }
    const std::vector<Term> & GetTerms() const { return _subTerms; }
    const TermElements & GetElements() const { return _elements; }

    friend bool operator == (const Term & lhs, const Term & rhs)
    {
        if ((lhs._elements.size() != rhs._elements.size()) ||
            (lhs._subTerms.size() != rhs._subTerms.size()))
            return false;
        bool same = true;
        for (size_t i = 0; same && (i < lhs._elements.size()); ++i)
        {
            if (lhs._elements[i] != rhs._elements[i])
                same = false;
        }
        for (size_t i = 0; same && (i < lhs._subTerms.size()); ++i)
        {
            if (lhs._subTerms[i] != rhs._subTerms[i])
                same = false;
        }
        return same;
    }
    friend bool operator != (const Term & lhs, const Term & rhs)
    {
        return ! operator == (lhs, rhs);
    }
    void PrintTo(std::ostream & stream, size_t indent = 0) const
    {
        for (auto const & element : _elements)
        {
            stream << std::string((indent + 1) * 4, ' ');
            element.PrintTo(stream);
            stream << std::endl;
        }
        for (auto const & subTerm : _subTerms)
        {
            subTerm.PrintTo(stream, indent + 1);
            stream << std::endl;
        }
    }

protected:
    TermElements _elements;
    std::vector<Term> _subTerms;
};

using Terms = std::vector<Term>;

class Regex : public Term
{
public:
    const int StartState = 1;
    const int EndState = 0;
    const int ErrorState = -1;
    Regex()
        : _pattern()
        , _currentFSMState()
        , _fsmRules()
        , _fsm(_fsmRules, StartState, EndState)
    {}
    explicit Regex(const std::string & pattern);
    bool Match(const std::string & text);
    bool PartialMatch(const std::string & text);

protected:
    std::string _pattern;
    int _currentFSMState;
    FSARuleSet<char, int, CharSet> _fsmRules;
    FSA<char, int, CharSet> _fsm;

    bool ParseEscapedChar(const std::string & pattern, size_t & index, char & ch);
    bool ParseLiteral(Term & term, const std::string & pattern, size_t & index);
    bool ParseRange(Term & term, const std::string & pattern, size_t & index, char expectedEndChar);
    bool ParseTerm(Term & term, const std::string & pattern, size_t & index);
    bool BuildFSM();
    TermElement CreateDigitElement();
    TermElement CreateNonDigitElement();
    TermElement CreateWordCharElement();
    TermElement CreateNonWordCharElement();
    TermElement CreateWhitespaceElement();
    TermElement CreateNonWhitespaceElement();
    TermElement CreateTabElement();
    TermElement CreateCarriageReturnElement();
    TermElement CreateLineBreakElement();
    TermElement CreateNonLineBreakElement();
    TermElement CreateLineFeedElement();
    TermElement CreateVerticalTabElement();
    TermElement CreateNonVerticalTabElement();
    TermElement CreateLiteralElement(char ch);
    void ChangeRepeat(TermElement & element, int minCount, int maxCount);
    void ChangeLastTermRepeat(int minCount, int maxCount);
    Term CreateTerm();
    bool Match(const std::string & text, size_t & index);
};

std::ostream & operator << (std::ostream & stream, const Term & term)
{
    term.PrintTo(stream);
    return stream;
}

} // namespace RE
