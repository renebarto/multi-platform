#include "re/Regex.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace RE {

static const CharSet DigitSet(CharSet::Range('0', '9'));
static const CharSet WordCharSet(CharSet::Range('0', '9') |
                                 CharSet::Range('a', 'z') | CharSet::Range('A', 'Z') |
                                 CharSet::Range('_'));
static const CharSet::Range Cr('\r');
static const CharSet::Range Lf('\n');
static const CharSet::Range Space(' ');
static const CharSet::Range Tab('\t');
static const CharSet::Range VTab('\f');
static const CharSet WhitespaceSet(Cr | Lf | Space | Tab | VTab);

Regex::Regex(const std::string & pattern)
    : _pattern(pattern)
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

bool Regex::ParseEscapedChar(const std::string & pattern, size_t & index, char & ch)
{
    if (index >= pattern.length())
        return false;
    ch = pattern[index];
    return true;
}

bool Regex::ParseLiteral(Term & term, const std::string & pattern, size_t & index)
{
    TermElement element;
    if (index >= pattern.length())
        return false;
    while (index < pattern.length())
    {
        char ch = pattern[index];
        if (ch == '\\')
        {
            char escapedChar;
            ++index;
            if (!ParseEscapedChar(pattern, index, escapedChar))
                return false;
            ++index;
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
                    element = CreateLiteralElement(escapedChar);
                    term.Add(element);
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
            ChangeRepeat(element, 0, -1);
            ++index;
        } else if (ch == '+')
        {
            ChangeRepeat(element, 1, -1);
            ++index;
        } else if (ch == '?')
        {
            ChangeRepeat(element, 0, 1);
            ++index;
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
            element = CreateLiteralElement(ch);
            ++index;
            term.Add(element);
        }
    }
    return true;
}

bool Regex::ParseRange(Term & term, const std::string & pattern, size_t & index, char expectedEndChar)
{
    bool negatedSet = false;
    CharSet charSet;
    if (index >= pattern.length())
        return false;
    if (pattern[index] == '^')
    {
        negatedSet = true;
        ++index;
    }
    if (index >= pattern.length())
        return false;
    while (index < pattern.length())
    {
        char ch = pattern[index];
        if (ch == expectedEndChar)
        {
            ++index;
            break;
        }
        else if (ch == '\\')
        {
            char escapedChar;
            ++index;
            if (!ParseEscapedChar(pattern, index, escapedChar))
                return false;
            ++index;
            switch (escapedChar)
            {
                case 'd':
                    charSet |= DigitSet;
                    break;
                case 'D':
                    charSet |= ~DigitSet;
                    break;
                case 'w':
                    charSet |= WordCharSet;
                    break;
                case 'W':
                    charSet |= ~WordCharSet;
                    break;
                case 's':
                    charSet |= WhitespaceSet;
                    break;
                case 'S':
                    charSet |= ~WhitespaceSet;
                    break;
                case 't':
                    charSet |= Tab;
                    break;
                case 'r':
                    charSet |= Cr;
                    break;
                case 'R':
                    charSet |= (Cr | Lf | VTab);
                    break;
                case 'n':
                    charSet |= Lf;
                    break;
                case 'N':
                    charSet |= ~(Cr | Lf | VTab);
                    break;
                case 'v':
                    charSet |= VTab;
                    break;
                case 'V':
                    charSet |= ~VTab;
                    break;
                default:
                    if (index >= pattern.length())
                        return false;
                    if (pattern[index] == '-')
                    {
                        ++index;
                        if (pattern[index] == '\\')
                        {
                            char escapedChar;
                            ++index;
                            if (!ParseEscapedChar(pattern, index, escapedChar))
                                return false;
                            charSet |= CharSet::Range(ch, escapedChar);
                        }
                        else
                        {
                            char endRangeChar = pattern[index];
                            charSet |= CharSet::Range(ch, endRangeChar);
                        }
                        ++index;
                    }
                    else
                        charSet |= ch;
                    break;
            }
        } else if (ch == '.')
        {
            charSet |= CharSet::All;
        } else if (ch == '[')
        {
            return false;
        } else
        {
            ++index;
            if (index >= pattern.length())
                return false;
            if (pattern[index] == '-')
            {
                ++index;
                if (pattern[index] == '\\')
                {
                    char escapedChar;
                    ++index;
                    if (!ParseEscapedChar(pattern, index, escapedChar))
                        return false;
                    charSet |= CharSet::Range(ch, escapedChar);
                }
                else
                {
                    char endRangeChar = pattern[index];
                    charSet |= CharSet::Range(ch, endRangeChar);
                }
                ++index;
            }
            else
                charSet |= ch;
        }
    }
    TermElement element;
    if (negatedSet)
    {
        element = TermElement(TermElement::Type::NotSet, charSet);
    }
    else
    {
        element = TermElement(TermElement::Type::Set, charSet);
    }
    term.Add(element);
    return true;
}

bool Regex::ParseTerm(Term & term, const std::string & pattern, size_t & index)
{
    TermElement element;
    while (index < pattern.length())
    {
        char ch = pattern[index];
        if (ch == '\\')
        {
            char escapedChar;
            ++index;
            if (!ParseEscapedChar(pattern, index, escapedChar))
                return false;
            ++index;
            switch (escapedChar)
            {
                case 'd':
                    element = CreateDigitElement();
                    term.Add(element);
                    break;
                case 'D':
                    element = CreateNonDigitElement();
                    term.Add(element);
                    break;
                case 'w':
                    element = CreateWordCharElement();
                    term.Add(element);
                    break;
                case 'W':
                    element = CreateNonWordCharElement();
                    term.Add(element);
                    break;
                case 's':
                    element = CreateWhitespaceElement();
                    term.Add(element);
                    break;
                case 'S':
                    element = CreateNonWhitespaceElement();
                    term.Add(element);
                    break;
                case 't':
                    element = CreateTabElement();
                    term.Add(element);
                    break;
                case 'r':
                    element = CreateCarriageReturnElement();
                    term.Add(element);
                    break;
                case 'R':
                    element = CreateLineBreakElement();
                    term.Add(element);
                    break;
                case 'n':
                    element = CreateLineFeedElement();
                    term.Add(element);
                    break;
                case 'N':
                    element = CreateNonLineBreakElement();
                    term.Add(element);
                    break;
                case 'v':
                    element = CreateVerticalTabElement();
                    term.Add(element);
                    break;
                case 'V':
                    element = CreateNonVerticalTabElement();
                    term.Add(element);
                    break;
                default:
                    Term subTerm;
                    if (!ParseLiteral(subTerm, pattern, index))
                        return false;
                    term.Add(subTerm);
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
            Term subTerm;
            index++;
            ParseRange(subTerm, pattern, index, ']');
            term.Add(subTerm);
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
            Term subTerm;
            if (!ParseLiteral(subTerm, pattern, index))
                return false;
            term.Add(subTerm);
        }
    }
    return true;
}

bool Regex::BuildFSM()
{
    size_t index = 0;
    if (!ParseTerm(*this, _pattern, index))
        return false;
    return true;
}

TermElement Regex::CreateLiteralElement(char ch)
{
    return TermElement(TermElement::Type::Literal, ch);
}

TermElement Regex::CreateDigitElement()
{
    return TermElement(TermElement::Type::Digit, DigitSet);
}

TermElement Regex::CreateNonDigitElement()
{
    return TermElement(TermElement::Type::NotDigit, ~DigitSet);
}

TermElement Regex::CreateWordCharElement()
{
    return TermElement(TermElement::Type::WordChar, WordCharSet);
}

TermElement Regex::CreateNonWordCharElement()
{
    return TermElement(TermElement::Type::NotWordChar, ~WordCharSet);
}

TermElement Regex::CreateWhitespaceElement()
{
    return TermElement(TermElement::Type::Whitespace, WhitespaceSet);
}

TermElement Regex::CreateNonWhitespaceElement()
{
    return TermElement(TermElement::Type::NotWhitespace, ~WhitespaceSet);
}

TermElement Regex::CreateTabElement()
{
    return TermElement();
}

TermElement Regex::CreateCarriageReturnElement()
{
    return TermElement();
}

TermElement Regex::CreateLineBreakElement()
{
    return TermElement();
}

TermElement Regex::CreateNonLineBreakElement()
{
    return TermElement();
}

TermElement Regex::CreateLineFeedElement()
{
    return TermElement();
}

TermElement Regex::CreateVerticalTabElement()
{
    return TermElement();
}

TermElement Regex::CreateNonVerticalTabElement()
{
    return TermElement();
}

void Regex::ChangeRepeat(TermElement & element, int minCount, int maxCount)
{
    element.SetMinMaxCount(minCount, maxCount);
}

void Regex::ChangeLastTermRepeat(int minCount, int maxCount)
{
}

Term Regex::CreateTerm()
{
    return Term();
}

bool Regex::Match(const std::string & text, size_t & index)
{
    return true;
}

} // namespace RE