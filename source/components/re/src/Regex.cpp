#include "re/Regex.h"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

namespace RE {

static const CharSet DigitSet(CharSet::Range('0', '9'));
static const CharSet AlphaCharSet(CharSet::Range('a', 'z') | CharSet::Range('A', 'Z') |
                                  CharSet::Range('_'));
static const CharSet WordCharSet(DigitSet | AlphaCharSet);
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
    , _nfa(StartState, EndState, ErrorState)
{
    if (!ParseRegex())
        throw invalid_argument("Invalid regular expression");
    if (!ConvertASTToNFA())
        throw invalid_argument("Invalid regular expression");
}

bool Regex::Match(const std::string & text)
{
    return _nfa.Match(text);
}

bool Regex::PartialMatch(const std::string & text)
{
    return _nfa.PartialMatch(text);
}

bool Regex::ParseRegex()
{
    if (!_tokenizer.Tokenize())
        return false;
    auto it = _tokenizer.GetTokenIterator();
    if (it.AtEnd())
        return true;
    ASTNode::Ptr rootNode = ParseExpression(it, TokenTypeSet());
    _ast.Root(rootNode);
    return (rootNode != nullptr);
}

bool Regex::Expect(TokenIterator & it, TokenType type)
{
    if (Have(it, type))
    {
        ++it;
        return true;
    }
    return false;
}

bool Regex::Expect(TokenIterator & it, const TokenTypeSet & types)
{
    if (Have(it, types))
    {
        ++it;
        return true;
    }
    return false;
}


bool Regex::Have(const TokenIterator & it, const TokenTypeSet & types)
{
    for (auto type : types)
    {
        if (!it.AtEnd() && (it->type == type))
            return true;
    }
    return false;
}

bool Regex::Have(const TokenIterator & it, TokenType type)
{
    return !it.AtEnd() && (it->type == type);
}

void Regex::AddNodeAndCheckMultiplicity(TokenIterator & it, ASTNode::Ptr & root, const ASTNode::Ptr & node)
{
    root->Insert(node);
    if (Have(it, TokenType::Plus))
    {
        node->UpdateTerm(1, Term::Any);
        ++it;
    } else if (Have(it, TokenType::Asterisk))
    {
        node->UpdateTerm(0, Term::Any);
        ++it;
    } else if (Have(it, TokenType::QuestionMark))
    {
        node->UpdateTerm(0, 1);
        ++it;
    }
}

ASTNode::Ptr Regex::ParseExpression(TokenIterator & it, const TokenTypeSet & terminators)
{
    ASTNode::Ptr rootNode = ASTOrOperation::Create();
    auto extendedTerminators = terminators;
    extendedTerminators.insert(TokenType::Or);
    while (!it.AtEnd())
    {
        auto node = ParseAlternative(it, extendedTerminators);
        rootNode->Insert(node);
        if (Have(it, terminators) || (terminators.empty() && it.AtEnd()))
        {
            Expect(it, terminators);
            return rootNode;
        }
        if (!Expect(it, TokenType::Or))
            return nullptr;
        if (Have(it, terminators) || (terminators.empty() && it.AtEnd()))
        {
            rootNode->Insert(ASTConcatOperation::Create());
            Expect(it, terminators);
            return rootNode;
        }
    }
    return nullptr;
}

ASTNode::Ptr Regex::ParseAlternative(TokenIterator & it, const TokenTypeSet & terminators)
{
    ASTNode::Ptr rootNode = ASTConcatOperation::Create();
    while (!it.AtEnd())
    {
        switch (it->type)
        {
            case TokenType::Digit:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateDigitElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::NotDigit:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateNotDigitElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::WordChar:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateWordCharElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::NotWordChar:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateNotWordCharElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::AlphaChar:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateAlphaCharElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::NotAlphaChar:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateNotAlphaCharElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::WhitespaceChar:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateWhitespaceCharElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::NotWhitespaceChar:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateNotWhitespaceCharElement()));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::Literal:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateLiteralElement(it->value)));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::Dot:
            {
                ASTNode::Ptr node = ASTLeaf::Create(Term(CreateLiteralElement(CharSet::All)));
                ++it;
                AddNodeAndCheckMultiplicity(it, rootNode, node);
            }
                break;
            case TokenType::Space:
                ++it; break;
            case TokenType::Tab:
                ++it; break;
            case TokenType::NewLine:
                ++it; break;
            case TokenType::Return:
                ++it; break;
            case TokenType::SquareBracketOpen:
                {
                    ++it;
                    ASTNode::Ptr node = ParseRange(it);
                    if (!Expect(it, TokenType::SquareBracketClose))
                        return nullptr;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::ParenthesisOpen:
                {
                    ++it;
                    ASTNode::Ptr expression = ParseExpression(it, TokenTypeSet {TokenType::ParenthesisClose});
                    ASTNode::Ptr node = ASTExpression::Create(expression);
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            default:
                if (Have(it, terminators))
                    return rootNode;
                return nullptr;
        }
    }
    return rootNode;
}

ASTNode::Ptr Regex::ParseRange(TokenIterator & it)
{
    bool negatedSet = false;
    CharSet charSet;
    while (!it.AtEnd())
    {
        switch (it->type)
        {
        case TokenType::SquareBracketClose:
            return negatedSet ? ASTLeaf::Create(Term(Term::Type::Set, ~charSet))
                              : ASTLeaf::Create(Term(Term::Type::Set, charSet));
        case TokenType::Caret:
            {
                if (negatedSet)
                    return nullptr;
                negatedSet = true;
                ++it;
            }
            break;
        case TokenType::Digit:
            charSet |= DigitSet;        ++it; break;
        case TokenType::NotDigit:
            charSet |= ~DigitSet;       ++it; break;
        case TokenType::WordChar:
            charSet |= WordCharSet;     ++it; break;
        case TokenType::NotWordChar:
            charSet |= ~WordCharSet;    ++it; break;
        case TokenType::AlphaChar:
            charSet |= AlphaCharSet;    ++it; break;
        case TokenType::NotAlphaChar:
            charSet |= ~AlphaCharSet;   ++it; break;
        case TokenType::WhitespaceChar:
            charSet |= WhitespaceSet;   ++it; break;
        case TokenType::NotWhitespaceChar:
            charSet |= ~WhitespaceSet;  ++it; break;
        case TokenType::Literal:
        case TokenType::Space:
        case TokenType::Tab:
        case TokenType::NewLine:
        case TokenType::Return:
            {
                char charStart = it->value;
                ++it;
                if (Have(it, TokenType::Dash))
                {
                    ++it;
                    if (it->type == TokenType::Literal)
                    {
                        char charEnd = it->value;
                        charSet |= CharSet::Range(charStart, charEnd);
                        ++it;
                    }
                    else
                        return nullptr;
                }
                else
                    charSet |= charStart;
            }
            break;
        case TokenType::Dot:
            return nullptr;
        default:
            return nullptr;
        }
    }
    return nullptr;
}

Term Regex::CreateLiteralElement(char ch)
{
    return Term(Term::Type::Literal, ch);
}

Term Regex::CreateLiteralElement(CharSet charSet)
{
    return Term(Term::Type::Literal, charSet);
}

Term Regex::CreateDigitElement()
{
    return Term(Term::Type::Digit, DigitSet);
}

Term Regex::CreateNotDigitElement()
{
    return Term(Term::Type::NotDigit, ~DigitSet);
}

Term Regex::CreateWordCharElement()
{
    return Term(Term::Type::WordChar, WordCharSet);
}

Term Regex::CreateNotWordCharElement()
{
    return Term(Term::Type::NotWordChar, ~WordCharSet);
}

Term Regex::CreateAlphaCharElement()
{
    return Term(Term::Type::AlphaChar, AlphaCharSet);
}

Term Regex::CreateNotAlphaCharElement()
{
    return Term(Term::Type::NotAlphaChar, ~AlphaCharSet);
}

Term Regex::CreateWhitespaceCharElement()
{
    return Term(Term::Type::Whitespace, WhitespaceSet);
}

Term Regex::CreateNotWhitespaceCharElement()
{
    return Term(Term::Type::NotWhitespace, ~WhitespaceSet);
}

using FSARule = NFARule<StringMatch::State, char, int, CharSet>;

bool Regex::ConvertASTToNFA()
{
    auto root = _ast.Root();
    if (root == nullptr)
    {
        _nfa = StringMatch(StartState, StartState, ErrorState);
        return true;
    }
    assert(root->GetOperation() == ASTNode::Operation::Or);
    State state = StartState;
    for (auto alternative : root->GetNodes())
    {
        assert(alternative->GetOperation() == ASTNode::Operation::Concat);
        _nfa.AddRule(FSARule(CharSet(), StartState, state + 1));
        ++state;
        for (auto term : alternative->GetNodes())
        {
            assert((term->GetOperation() == ASTNode::Operation::Leaf) ||
                   (term->GetOperation() == ASTNode::Operation::Expression));
            if (term->GetOperation() == ASTNode::Operation::Leaf)
            {
                if (term->GetTerm().GetMinCount() == 0)
                {
                    if (term->GetTerm().GetMaxCount() == Term::Any) // *
                    {
                        _nfa.AddRule(FSARule(term->GetTerm().GetCharSet(), state, state));
                    }
                    else // ?
                    {
                        _nfa.AddRule(FSARule(term->GetTerm().GetCharSet(), state, state + 1));
                        _nfa.AddRule(FSARule(CharSet(), state, state + 1));
                        ++state;
                    }
                }
                else
                {
                    _nfa.AddRule(FSARule(term->GetTerm().GetCharSet(), state, state + 1));
                    if (term->GetTerm().GetMaxCount() == Term::Any) // +
                    {
                        _nfa.AddRule(FSARule(term->GetTerm().GetCharSet(), state + 1, state + 1));
                    }
                    ++state;
                }
            }
        }
        _nfa.AddRule(FSARule(CharSet(), state, EndState));
    }
    cout << _nfa;
    return true;
}

} // namespace RE