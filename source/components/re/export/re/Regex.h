#pragma once

#include <string>
#include "FSA.h"

namespace RE {

class CharSet : public InputSet<char>
{
public:
    CharSet()
        : InputSet()
    {}
    CharSet(char value)
        : InputSet(value)
    {}
    CharSet(const std::string & values)
        : InputSet()
    {
        Add(values.begin(), values.end());
    }
};

class Term
{
public:
    enum TermType
    {
        None,
        Literal,
        OneOf,
        OneNotOf,
    };
    Term()
        : _type(TermType::None)
    {}
    Term(TermType type)
        : _type(type)
    {}
    TermType Type() const { return _type; }
    void Add(char ch)
    {
        _value += ch;
    }

protected:
    TermType _type;
    std::string _value;
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

protected:
    std::string _pattern;
    Terms _regexTerms;
    Term _currentTerm;
    int _currentFSMState;
    FSARuleSet<char, int, CharSet> _fsmRules;
    FSA<char, int, CharSet> _fsm;

    bool BuildFSM();
    void CreateLiteral(char ch);
    void AddCurrentTerm();
    bool Match(const std::string & text, size_t & index);
};

} // namespace RE
