#include "regex/RE.h"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

namespace Regex {

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

RE::RE(const std::string & pattern)
    : _pattern(pattern)
    , _reader(_pattern)
    , _tokenizer(_reader)
    , _ast()
    , _nfa(StartState, EndState, ErrorState)
{
    if (!ParseRegex())
        throw invalid_argument("Invalid regular expression");
    if (!ConvertASTToNFA())
        throw invalid_argument("Invalid regular expression");
}

RE::RE(const RE & other)
    : _pattern(other._pattern)
    , _reader(_pattern)
    , _tokenizer(_reader)
    , _ast()
    , _nfa(StartState, EndState, ErrorState)
{
    if (!ParseRegex())
        throw invalid_argument("Invalid regular expression");
    if (!ConvertASTToNFA())
        throw invalid_argument("Invalid regular expression");
}

//RE::RE(RE && other)
//    : _reader(std::move(other._reader))
//    , _tokenizer(std::move(other._tokenizer))
//    , _ast(std::move(other._ast))
//    , _nfa(std::move(other._nfa))
//{
//}

RE & RE::operator = (const RE & other)
{
    if (this != &other)
    {
        _pattern = other._pattern;
        _reader = InputReader(_pattern);
        _tokenizer = Tokenizer(_reader);
        _ast = AST();
        _nfa = StringMatch(StartState, EndState, ErrorState);
    }
    return *this;
}

//RE & RE::operator = (RE && other)
//{
//    if (this != &other)
//    {
//        _reader = std::move(other._reader);
//        _tokenizer = std::move(other._tokenizer);
//        _ast = std::move(other._ast);
//        _nfa = std::move(other._nfa);
//    }
//    return *this;
//}

bool RE::Match(const std::string & text) const
{
    return _nfa.Match(text);
}

bool RE::PartialMatch(const std::string & text) const
{
    return _nfa.PartialMatch(text);
}

bool RE::ParseRegex()
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

bool RE::Expect(TokenIterator & it, TokenType type)
{
    if (Have(it, type))
    {
        ++it;
        return true;
    }
    return false;
}

bool RE::Expect(TokenIterator & it, const TokenTypeSet & types)
{
    if (Have(it, types))
    {
        ++it;
        return true;
    }
    return false;
}


bool RE::Have(const TokenIterator & it, const TokenTypeSet & types)
{
    for (auto type : types)
    {
        if (!it.AtEnd() && (it->type == type))
            return true;
    }
    return false;
}

bool RE::Have(const TokenIterator & it, TokenType type)
{
    return !it.AtEnd() && (it->type == type);
}

void RE::AddNodeAndCheckMultiplicity(TokenIterator & it, ASTNode::Ptr & root, const ASTNode::Ptr & node)
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

ASTNode::Ptr RE::ParseExpression(TokenIterator & it, const TokenTypeSet & terminators)
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

ASTNode::Ptr RE::ParseAlternative(TokenIterator & it, const TokenTypeSet & terminators)
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
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateLiteralElement(it->value)));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::Tab:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateLiteralElement(it->value)));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
            case TokenType::NewLine:
                {
                    ASTNode::Ptr node = ASTLeaf::Create(Term(CreateLiteralElement(it->value)));
                    ++it;
                    AddNodeAndCheckMultiplicity(it, rootNode, node);
                }
                break;
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

ASTNode::Ptr RE::ParseRange(TokenIterator & it)
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
                    if ((it->type == TokenType::Literal))
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

Term RE::CreateLiteralElement(char ch)
{
    return Term(Term::Type::Literal, ch);
}

Term RE::CreateLiteralElement(CharSet charSet)
{
    return Term(Term::Type::Literal, charSet);
}

Term RE::CreateDigitElement()
{
    return Term(Term::Type::Digit, DigitSet);
}

Term RE::CreateNotDigitElement()
{
    return Term(Term::Type::NotDigit, ~DigitSet);
}

Term RE::CreateWordCharElement()
{
    return Term(Term::Type::WordChar, WordCharSet);
}

Term RE::CreateNotWordCharElement()
{
    return Term(Term::Type::NotWordChar, ~WordCharSet);
}

Term RE::CreateAlphaCharElement()
{
    return Term(Term::Type::AlphaChar, AlphaCharSet);
}

Term RE::CreateNotAlphaCharElement()
{
    return Term(Term::Type::NotAlphaChar, ~AlphaCharSet);
}

Term RE::CreateWhitespaceCharElement()
{
    return Term(Term::Type::Whitespace, WhitespaceSet);
}

Term RE::CreateNotWhitespaceCharElement()
{
    return Term(Term::Type::NotWhitespace, ~WhitespaceSet);
}

using FSARule = NFARule<StringMatch::State, char, int, CharSet>;

bool RE::ConvertASTToNFA()
{
    auto root = _ast.Root();
    if (root == nullptr)
    {
        _nfa = StringMatch(StartState, StartState, ErrorState);
        return true;
    }
    State lastState;
    if (!ConvertASTToNFA(root, StartState, TempEndState, lastState))
        return false;
    for (auto rule : _nfa.FindRulesToState(TempEndState))
    {
        rule->NextState(EndState);
    }
    return true;
}

bool RE::ConvertASTToNFA(ASTNode::Ptr base, State startState, State endState, State & actualEndState)
{
    if (base == nullptr)
    {
        actualEndState = startState;
        return true;
    }
    assert(base->GetOperation() == ASTNode::Operation::Or);
    State state = startState;
    for (auto alternative : base->GetNodes())
    {
        assert(alternative->GetOperation() == ASTNode::Operation::Concat);
        _nfa.AddRule(FSARule(CharSet(), startState, state + 1));
        ++state;
        for (auto term : alternative->GetNodes())
        {
            assert((term->GetOperation() == ASTNode::Operation::Leaf) ||
                   (term->GetOperation() == ASTNode::Operation::Expression));
            State startSubState = state;
            State endSubState {};
            if (term->GetOperation() == ASTNode::Operation::Leaf)
            {
                endSubState = state + 1;
                _nfa.AddRule(FSARule(term->GetTerm().GetCharSet(), startSubState, endSubState));
            } else
            {
                if (!ConvertASTToNFA(term->GetNodes()[0], startSubState, endState + 1, endSubState))
                    return false;
                for (auto rule : _nfa.FindRulesToState(endState + 1))
                {
                    rule->NextState(endSubState);
                }
            }
            if (term->GetTerm().GetMinCount() == 0)
            {
                if (term->GetTerm().GetMaxCount() == Term::Any) // *
                {
                    _nfa.AddRule(FSARule(CharSet(), endSubState, startSubState));
                    _nfa.AddRule(FSARule(CharSet(), startSubState, endSubState + 1));
                    state = endSubState + 1;
                }
                else // ?
                {
                    _nfa.AddRule(FSARule(CharSet(), startSubState, endSubState));
                }
            }
            else
            {
                if (term->GetTerm().GetMaxCount() == Term::Any) // +
                {
                    _nfa.AddRule(FSARule(CharSet(), endSubState, startSubState));
                }
                state = endSubState;
            }
        }
        _nfa.AddRule(FSARule(CharSet(), state, endState));
    }
    actualEndState = state + 1;
    return true;
}

} // namespace Regex