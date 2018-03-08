#include "re/Regex.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace RE;

Regex::Regex(const std::string & pattern)
    : _pattern(pattern)
    , _regexTerms()
    , _currentTerm()
    , _currentFSMState()
    , _fsmRules()
    , _fsm(_fsmRules, StartState, EndState)
{
    if (!BuildFSM())
        throw invalid_argument("Invalid regular expression");
}

bool Regex::Match(const std::string & text)
{
    bool result = PartialMatch(text);
    return (result && _fsm.IsFinalState());
}

bool Regex::PartialMatch(const std::string & text)
{
    _fsm.Reset();
//    size_t index {};
//    for (auto const & term : _regexTerms)
//    {
//        if (index >= text.length())
//            break;
//        if (!Match(text, index))
//            return false;
//    }
    for (size_t index = 0; index < text.length(); ++index)
    {
        int newState = _fsm.HandleInput(text[index]);
        if (newState == ErrorState)
            return false;
    }
    return true;
}

bool Regex::BuildFSM()
{
    size_t index {};
    while(index < _pattern.length())
    {
        char ch = _pattern[index]; 
        if (ch == '\\')
        {
            ++index;
            if (index >= _pattern.length())
                return false;
            ch = _pattern[index];
            CreateLiteral(ch);
            _fsmRules.emplace_back(ch, _currentFSMState, _currentFSMState + 1);
            ++_currentFSMState;
        }
        else if (ch == '^')
        {

        }
        else if (ch == '$')
        {

        }
        else if (ch == '.')
        {

        }
        else if (ch == '*')
        {

        }
        else if (ch == '+')
        {

        }
        else if (ch == '?')
        {

        }
        else if (ch == '(')
        {

        }
        else if (ch == ')')
        {

        }
        else if (ch == '[')
        {

        }
        else if (ch == ']')
        {

        }
        else if (ch == '{')
        {

        }
        else if (ch == '}')
        {

        }
        else if (ch == '|')
        {

        }
        else
        {
            ch = _pattern[index];
            CreateLiteral(ch);
            _fsmRules.emplace_back(ch, _currentFSMState, _currentFSMState + 1);
            ++_currentFSMState;
        }
        ++index;
    }
    AddCurrentTerm();
    _fsmRules[_fsmRules.size() - 1].NextState(EndState);
    return true;
}

void Regex::CreateLiteral(char ch)
{
    if (_currentTerm.Type() != Term::TermType::None)
    {
        AddCurrentTerm();
        _currentTerm.Add(ch);
        _currentTerm = Term(Term::TermType::Literal);
    }
    _currentTerm.Add(ch);
}

void Regex::AddCurrentTerm()
{
    _regexTerms.push_back(_currentTerm);
}

bool Regex::Match(const std::string & text, size_t & index)
{
    return true;
}
