#include "re/Regex.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace RE {

static const CharSet DigitSet(CharSet::Range('0', '9'));
static const CharSet WordCharSet(CharSet::Range('0', '9') |
                                 CharSet::Range('a', 'z') | CharSet::Range('A', 'Z') |
                                 CharSet::Range('_'));
static const CharSet WhitespaceSet(CharSet::Range('\r') | CharSet::Range('\n') | CharSet::Range(' ') |
                                   CharSet::Range('\t') | CharSet::Range('\f'));

Regex::Regex(const std::string & pattern)
    : _pattern(pattern), _regexTerms(), _currentTerm(), _currentFSMState(), _fsmRules(),
      _fsm(_fsmRules, StartState, EndState)
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

bool Regex::ParseEscapedChar(const std::string & pattern, size_t & index, char & ch)
{
    if (index >= _pattern.length())
        return false;
    ch = pattern[index];
    return true;
}

bool Regex::ParseLiteral(const std::string & pattern, size_t & index)
{
    if (index >= _pattern.length())
        return false;
    while (index < _pattern.length())
    {
        char ch = pattern[index];
        if (ch == '\\')
        {
            char escapedChar;
            ++index;
            if (!ParseEscapedChar(pattern, index, escapedChar))
                return false;
            switch (escapedChar)
            {
                case '.':
                case '*':
                case '+':
                case '?':
                case '$':
                case '^':
                case '/':
                case '\\':
                case '[':
                case ']':
                case '{':
                case '}':
                case '(':
                case ')':
                    CreateLiteralTerm(escapedChar);
                    break;
                default:
                    return false;
            }
        } else if (ch == '^')
        {
            return false;
        } else if (ch == '$')
        {
            return false;
        } else if (ch == '.')
        {
            return true;
        } else if (ch == '*')
        {
            ChangeLastTermRepeat(0, -1);
        } else if (ch == '+')
        {
            ChangeLastTermRepeat(1, -1);
        } else if (ch == '?')
        {
            ChangeLastTermRepeat(0, 1);
        } else if (ch == '(')
        {
            return true;
        } else if (ch == ')')
        {
            return true;
        } else if (ch == '[')
        {
            return true;
        } else if (ch == ']')
        {
            return true;
        } else if (ch == '{')
        {
            // ParseRepeatCounts
            return false;
        } else if (ch == '}')
        {
            return false;
        } else if (ch == '|')
        {
            return false;
        } else
        {
            CreateLiteralTerm(ch);
        }
        ++index;
    }
    return true;
}

bool Regex::Parse(const std::string & pattern, size_t & index)
{
    while (index < _pattern.length())
    {
        char ch = _pattern[index];
        if (ch == '\\')
        {
            char escapedChar;
            if (!ParseEscapedChar(pattern, index, escapedChar))
                return false;
            ++index;
            switch (escapedChar)
            {
                case 'd':
                    CreateDigitTerm();
                    break;
                case 'D':
                    CreateNonDigitTerm();
                    break;
                case 'w':
                    CreateWordCharTerm();
                    break;
                case 'W':
                    CreateNonWordCharTerm();
                    break;
                case 's':
                    CreateWhitespaceTerm();
                    break;
                case 'S':
                    CreateNonWhitespaceTerm();
                    break;
                case 't':
                    CreateTabTerm();
                    break;
                case 'r':
                    CreateCarriageReturnTerm();
                    break;
                case 'R':
                    CreateLineBreakTerm();
                    break;
                case 'n':
                    CreateLineFeedTerm();
                    break;
                case 'N':
                    CreateNonLineBreakTerm();
                    break;
                case 'v':
                    CreateVerticalTabTerm();
                    break;
                case 'V':
                    CreateNonVerticalTabTerm();
                    break;
                default:
                    ParseLiteral(_pattern, index);
                    break;
            }
        } else if (ch == '^')
        {

        } else if (ch == '$')
        {

        } else if (ch == '.')
        {

        } else if (ch == '*')
        {

        } else if (ch == '+')
        {

        } else if (ch == '?')
        {

        } else if (ch == '(')
        {

        } else if (ch == ')')
        {

        } else if (ch == '[')
        {

        } else if (ch == ']')
        {

        } else if (ch == '{')
        {

        } else if (ch == '}')
        {

        } else if (ch == '|')
        {

        } else
        {
            if (!ParseLiteral(_pattern, index))
                return false;
        }
        ++index;
    }
    return true;
}

bool Regex::BuildFSM()
{
    size_t index = 0;
    if (!Parse(_pattern, index))
        return false;
    SaveCurrentTerm();
    return true;
}

void Regex::CreateLiteralTerm(char ch)
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::Literal, ch);
}

void Regex::CreateDigitTerm()
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::Digit, DigitSet);
}

void Regex::CreateNonDigitTerm()
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::NotDigit, ~DigitSet);
}

void Regex::CreateWordCharTerm()
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::WordChar, WordCharSet);
}

void Regex::CreateNonWordCharTerm()
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::NotWordChar, ~WordCharSet);
}

void Regex::CreateWhitespaceTerm()
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::Whitespace, WhitespaceSet);
}

void Regex::CreateNonWhitespaceTerm()
{
    SaveCurrentTerm();
    _currentTerm = Term(Term::TermType::NotWhitespace, ~WhitespaceSet);
}

void Regex::CreateTabTerm()
{

}

void Regex::CreateCarriageReturnTerm()
{

}

void Regex::CreateLineBreakTerm()
{

}

void Regex::CreateNonLineBreakTerm()
{

}

void Regex::CreateLineFeedTerm()
{

}

void Regex::CreateVerticalTabTerm()
{

}

void Regex::CreateNonVerticalTabTerm()
{

}

void Regex::ChangeLastTermRepeat(int minCount, int maxCount)
{
    _currentTerm.SetMinMaxCount(minCount, maxCount);
}

void Regex::SaveCurrentTerm()
{
    if (_currentTerm.Type() != Term::TermType::None)
    {
        AddCurrentTerm();
    }
}

void Regex::AddCurrentTerm()
{
    _regexTerms.push_back(_currentTerm);
}

bool Regex::Match(const std::string & text, size_t & index)
{
    return true;
}

} // namespace RE