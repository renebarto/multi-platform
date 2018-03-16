#include "re/Regex.h"

#include <cassert>
#include <iostream>
#include <sstream>

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
    : _reader(pattern)
    , _tokenizer(_reader)
    , _ast()
{
    if (!ParseRegex())
        throw invalid_argument("Invalid regular expression");
}

bool Regex::Match(const std::string & text)
{
    return false;
}

bool Regex::PartialMatch(const std::string & text)
{
    return false;
}

bool Regex::ParseRegex()
{
    if (!_tokenizer.Tokenize())
        return false;
    auto it = _tokenizer.GetTokenIterator();
    if (_tokenizer.GetTokenIterator().AtEnd())
        return true;
    ASTNode::Ptr root = Parse(it);
    _ast.Root(root);
    if (root == nullptr)
        return false;
    return true;
}

ASTNode::Ptr Regex::Parse(TokenIterator & it)
{
    if (it.AtEnd())
        return ASTLeafNull::Create();
    switch (it->type)
    {
        case TokenType::Literal:
            {
                ASTNode::Ptr node = ASTLeaf::Create(Term(CreateLiteralElement(it->value)));
                ++it;
                if (it.AtEnd())
                {
                    return node;
                }
                else
                {
                    switch (it->type)
                    {
                        case TokenType::Literal:
                            {
                                ASTNode::Ptr rhs = Parse(it);
                                ASTNode::Ptr lhs = node;
                                return ASTBinaryOperation::Create(ASTNode::Operation::Concat, lhs, rhs);
                            }
                            break;
                        case TokenType::Or:
                            {
                                ++it;
                                ASTNode::Ptr rhs = Parse(it);
                                ASTNode::Ptr lhs = node;
                                return ASTBinaryOperation::Create(ASTNode::Operation::Or, lhs, rhs);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
        case TokenType::Or:
            {
                ++it;
                ASTNode::Ptr rhs = Parse(it);
                return ASTBinaryOperation::Create(ASTNode::Operation::Or, ASTLeafNull::Create(), rhs);
            }
            break;
        default:
            ++it;
            break;
    }
    return nullptr;
}

//bool Regex::GetEscapedChar(char & ch)
//{
//    return _reader.PeekChar(ch);
//}
//
//bool Regex::ParseEscapedChar(Term & term)
//{
//    char escapedChar;
//    _reader.Advance();
//    if (!GetEscapedChar(escapedChar))
//        return false;
//    _reader.Advance();
//    Term element;
//    switch (escapedChar)
//    {
//        case 'd':
//            element = CreateDigitElement();
//            term.Add(element);
//            break;
//        case 'D':
//            element = CreateNonDigitElement();
//            term.Add(element);
//            break;
//        case 'w':
//            element = CreateWordCharElement();
//            term.Add(element);
//            break;
//        case 'W':
//            element = CreateNonWordCharElement();
//            term.Add(element);
//            break;
//        case 's':
//            element = CreateWhitespaceElement();
//            term.Add(element);
//            break;
//        case 'S':
//            element = CreateNonWhitespaceElement();
//            term.Add(element);
//            break;
//        case 't':
//            element = CreateTabElement();
//            term.Add(element);
//            break;
//        case 'r':
//            element = CreateCarriageReturnElement();
//            term.Add(element);
//            break;
//        case 'R':
//            element = CreateLineBreakElement();
//            term.Add(element);
//            break;
//        case 'n':
//            element = CreateLineFeedElement();
//            term.Add(element);
//            break;
//        case 'N':
//            element = CreateNonLineBreakElement();
//            term.Add(element);
//            break;
//        case 'v':
//            element = CreateVerticalTabElement();
//            term.Add(element);
//            break;
//        case 'V':
//            element = CreateNonVerticalTabElement();
//            term.Add(element);
//            break;
//        default:
//            element = CreateLiteralElement(escapedChar);
//            term.Add(element);
//            break;
//    }
//    return true;
//}
//bool Regex::ParseLiteral(Term & term)
//{
//    if (_reader.AtEnd())
//        return false;
//    char ch;
//    if (!_reader.GetChar(ch))
//        return false;
//    Term element = CreateLiteralElement(ch);
//    term.Add(element);
//    return true;
//}
//
//bool Regex::ParseRange(Term & term, char expectedEndChar)
//{
//    bool negatedSet = false;
//    CharSet charSet;
//    if (_reader.AtEnd())
//        return false;
//    char ch;
//    if (!_reader.PeekChar(ch))
//        return false;
//    if (ch == '^')
//    {
//        negatedSet = true;
//        _reader.Advance();
//    }
//    if (_reader.AtEnd())
//        return false;
//    while (!_reader.AtEnd())
//    {
//        if (!_reader.PeekChar(ch))
//            return false;
//        if (ch == expectedEndChar)
//        {
//            _reader.Advance();
//            break;
//        }
//        else if (ch == '\\')
//        {
//            char escapedChar;
//            _reader.Advance();
//            if (!GetEscapedChar(escapedChar))
//                return false;
//            _reader.Advance();
//            switch (escapedChar)
//            {
//                case 'd':
//                    charSet |= DigitSet;
//                    break;
//                case 'D':
//                    charSet |= ~DigitSet;
//                    break;
//                case 'w':
//                    charSet |= WordCharSet;
//                    break;
//                case 'W':
//                    charSet |= ~WordCharSet;
//                    break;
//                case 's':
//                    charSet |= WhitespaceSet;
//                    break;
//                case 'S':
//                    charSet |= ~WhitespaceSet;
//                    break;
//                case 't':
//                    charSet |= Tab;
//                    break;
//                case 'r':
//                    charSet |= Cr;
//                    break;
//                case 'R':
//                    charSet |= (Cr | Lf | VTab);
//                    break;
//                case 'n':
//                    charSet |= Lf;
//                    break;
//                case 'N':
//                    charSet |= ~(Cr | Lf | VTab);
//                    break;
//                case 'v':
//                    charSet |= VTab;
//                    break;
//                case 'V':
//                    charSet |= ~VTab;
//                    break;
//                default:
//                    if (_reader.AtEnd())
//                        return false;
//                    char ch1;
//                    if (!_reader.PeekChar(ch1))
//                        return false;
//                    if (ch1 == '-')
//                    {
//                        char endRangeChar;
//                        if (!_reader.GetChar(endRangeChar))
//                            return false;
//                        if (endRangeChar == '\\')
//                        {
//                            char escapedChar2;
//                            _reader.Advance();
//                            if (!GetEscapedChar(escapedChar2))
//                                return false;
//                            charSet |= CharSet::Range(escapedChar, escapedChar2);
//                        }
//                        else
//                        {
//                            charSet |= CharSet::Range(escapedChar, endRangeChar);
//                        }
//                        _reader.Advance();
//                    }
//                    else
//                        charSet |= ch;
//                    break;
//            }
//        } else if (ch == '.')
//        {
//            charSet |= CharSet::All;
//        } else if (ch == '[')
//        {
//            return false;
//        } else
//        {
//            _reader.Advance();
//            if (_reader.AtEnd())
//                return false;
//            char ch1;
//            if (!_reader.PeekChar(ch1))
//                return false;
//            if (ch1 == '-')
//            {
//                char endRangeChar;
//                if (!_reader.GetChar(endRangeChar))
//                    return false;
//                if (endRangeChar == '\\')
//                {
//                    char escapedChar;
//                    _reader.Advance();
//                    if (!GetEscapedChar(escapedChar))
//                        return false;
//                    charSet |= CharSet::Range(ch, escapedChar);
//                }
//                else
//                {
//                    charSet |= CharSet::Range(ch, endRangeChar);
//                }
//                _reader.Advance();
//            }
//            else
//                charSet |= ch;
//        }
//    }
//    Term element;
//    if (negatedSet)
//    {
//        element = Term(Term::Type::NotSet, charSet);
//    }
//    else
//    {
//        element = Term(Term::Type::Set, charSet);
//    }
//    term.Add(element);
//    return true;
//}
//
//bool Regex::ParseMultiplicity(int & minCount, int & maxCount, char expectedEndChar)
//{
//    if (_reader.AtEnd())
//        return false;
//    bool haveMinCount {};
//    string value;
//    while (!_reader.AtEnd())
//    {
//        char ch;
//        if (!_reader.PeekChar(ch))
//            return false;
//        if (ch == expectedEndChar)
//        {
//            _reader.Advance();
//            istringstream stream(value);
//            int count;
//            stream >> count;
//            if (haveMinCount)
//                maxCount = count;
//            else
//                maxCount = minCount = count;
//            break;
//        } else if (DigitSet.Contains(ch))
//        {
//            value += ch;
//            _reader.Advance();
//        } else if (ch == ' ')
//        {
//            _reader.Advance();
//        } else if (ch == ',')
//        {
//            if (haveMinCount)
//                return false;
//            istringstream stream(value);
//            stream >> minCount;
//            haveMinCount = true;
//        } else
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//bool Regex::ParseAlternative(Term & term)
//{
//    if (_reader.AtEnd())
//        return true;
//    while (!_reader.AtEnd())
//    {
//        Term subTerm;
//        if (!ParseTerm(subTerm, '|'))
//            return false;
//        term.Add(subTerm);
//        if (_reader.Have('|'))
//            return true;
//    }
//    return true;
//}

//bool Regex::ParseTerm(Term & term, char expectedEndChar)
//{
//    if (_reader.AtEnd())
//        return true;
//    while (!_reader.AtEnd())
//    {
//        char ch;
//        if (!_reader.GetChar(ch))
//            return true;
//        if (ch == expectedEndChar)
//        {
//            return true;
//        } else if (ch == '\\')
//        {
//            char escapeChar;
//            if (!_reader.LookAheadChar(escapeChar))
//                return false;
//            if (escapeChar == 'b')
//            {
//                // Marks word boundary (word char to non word char or non word char to word char)
////                term.Add(CreateWordBoundaryElement());
//                _reader.Advance();
//                _reader.Advance();
//            } else if (escapeChar == 'B')
//            {
//                // Marks word continuation (word char to word char or non word char to non word char)
////                term.Add(CreateNonWordBoundaryElement());
//                _reader.Advance();
//                _reader.Advance();
//            }
//        } else if (ch == '^')
//        {
//            // Marks beginning of line
////            term.Add(CreateLineBeginElement());
//            _reader.Advance();
//        } else if (ch == '$')
//        {
//            // Marks end of line
////            term.Add(CreateLineEndElement());
//            _reader.Advance();
//        } else if (ch == '(')
//        {
//            _reader.Advance();
//            if (!_reader.GetChar(ch))
//                return false;
//            if (ch == '?')
//            {
//                // (?=<term>) marks disjunction assertion
//                // (?!<term>) marks non disjunction assertion
//                char nextChar;
//                _reader.Advance();
//                if (!_reader.GetChar(nextChar))
//                    return false;
//                if (nextChar == '!')
//                {
//                    _reader.Advance();
////                    term.Add(CreateTermNotDisjunctionAssertionElement());
//                }
//                else if (nextChar == '=')
//                {
//                    _reader.Advance();
////                    term.Add(CreateTermDisjunctionAssertionElement());
//                }
//                else
//                    return false;
//                Term subTerm;
//                if (!ParseTerm(subTerm, ')'))
//                    return false;
//                if (!_reader.Expect(')'))
//                    return false;
//                term.Add(subTerm);
//            }
//            else
//            {
//                // (<term>) marks a normal matched term
//                Term subTerm;
//                if (!ParseTerm(subTerm, ')'))
//                    return false;
//                if (!_reader.Expect(')'))
//                    return false;
//                term.Add(subTerm);
//            }
//        } else
//        {
//            term.Add(CreateLiteralElement(ch));
//            _reader.Advance();
//        }
//    }
//    Term element;
//    while (!_reader.AtEnd())
//    {
//        char ch = pattern[index];
//        if (ch == '\\')
//        {
//            if (!ParseEscapedChar(term, pattern, index))
//                return false;
//        } else if (ch == '$')
//        {
//            element = CreateLineBeginElement();
//            term.Add(element);
//            _reader.Advance();
//        } else if (ch == '^')
//        {
//            element = CreateLineEndElement();
//            term.Add(element);
//            _reader.Advance();
//        } else if (ch == '.')
//        {
//
//        } else if (ch == '*')
//        {
//            Term * lastElement = term.GetLastElement();
//            if (lastElement == nullptr)
//                return false;
//            ChangeRepeat(*lastElement, 0, -1);
//            _reader.Advance();
//        } else if (ch == '+')
//        {
//            Term * lastElement = term.GetLastElement();
//            if (lastElement == nullptr)
//                return false;
//            ChangeRepeat(*lastElement, 1, -1);
//            _reader.Advance();
//        } else if (ch == '?')
//        {
//            Term * lastElement = term.GetLastElement();
//            if (lastElement == nullptr)
//                return false;
//            ChangeRepeat(*lastElement, 0, 1);
//            _reader.Advance();
//        } else if (ch == '{')
//        {
//            _reader.Advance();
//            int minCount {};
//            int maxCount {};
//            if (!ParseMultiplicity(pattern, index, minCount, maxCount, '}'))
//                return false;
//        } else if (ch == '}')
//        {
//
//        } else if (ch == '(')
//        {
//
//        } else if (ch == ')')
//        {
//
//        } else if (ch == '[')
//        {
//            index++;
//            if (!ParseRange(term, pattern, index, ']'))
//                return false;
//        } else if (ch == ']')
//        {
//            return false;
//        } else if (ch == '|')
//        {
//
//        } else
//        {
//            if (!ParseLiteral(term, pattern, index))
//                return false;
//        }
//    }
//    return true;
//}

Term Regex::CreateLiteralElement(char ch)
{
    return Term(Term::Type::Literal, ch);
}

Term Regex::CreateDigitElement()
{
    return Term(Term::Type::Digit, DigitSet);
}

Term Regex::CreateNonDigitElement()
{
    return Term(Term::Type::NotDigit, ~DigitSet);
}

Term Regex::CreateWordCharElement()
{
    return Term(Term::Type::WordChar, WordCharSet);
}

Term Regex::CreateNonWordCharElement()
{
    return Term(Term::Type::NotWordChar, ~WordCharSet);
}

Term Regex::CreateWhitespaceElement()
{
    return Term(Term::Type::Whitespace, WhitespaceSet);
}

Term Regex::CreateNonWhitespaceElement()
{
    return Term(Term::Type::NotWhitespace, ~WhitespaceSet);
}

Term Regex::CreateTabElement()
{
    return Term();
}

Term Regex::CreateCarriageReturnElement()
{
    return Term();
}

Term Regex::CreateLineBreakElement()
{
    return Term();
}

Term Regex::CreateNonLineBreakElement()
{
    return Term();
}

Term Regex::CreateLineFeedElement()
{
    return Term();
}

Term Regex::CreateVerticalTabElement()
{
    return Term();
}

Term Regex::CreateNonVerticalTabElement()
{
    return Term();
}

void Regex::ChangeRepeat(Term & element, int minCount, int maxCount)
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