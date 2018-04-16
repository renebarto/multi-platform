#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "regex/RE.h"
#include "regex/TestData.h"

using namespace std;

namespace Regex {
namespace Test {

class RETest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(regex) {

TEST_FIXTURE(RETest, Empty)
{
    RE regex("");
    auto const & ast = regex.GetAST();
    EXPECT_TRUE(ast.IsEmpty());
    auto const & nfa = regex.GetNFA();
    EXPECT_TRUE(nfa.GetRules().empty());
}

TEST_FIXTURE(RETest, SingleLiteral)
{
    RE regex("a");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());

    auto const & nfa = regex.GetNFA();
    ASSERT_EQ(size_t {3}, nfa.GetRules().size());
    EXPECT_EQ(CharSet(), nfa.GetRules()[0].ExpectedInput());
    EXPECT_EQ(1, nfa.GetRules()[0].ExpectedState());
    EXPECT_EQ(2, nfa.GetRules()[0].NextState());

    EXPECT_EQ(CharSet('a'), nfa.GetRules()[1].ExpectedInput());
    EXPECT_EQ(2, nfa.GetRules()[1].ExpectedState());
    EXPECT_EQ(3, nfa.GetRules()[1].NextState());

    EXPECT_EQ(CharSet(), nfa.GetRules()[2].ExpectedInput());
    EXPECT_EQ(3, nfa.GetRules()[2].ExpectedState());
    EXPECT_EQ(0, nfa.GetRules()[2].NextState());
}

TEST_FIXTURE(RETest, AlternativesOfSingleLiterals)
{
    RE regex("a|b");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{2}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodesA = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesA.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesA[0]->GetOperation());
    ASSERT_TRUE(concatNodesA[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodesA[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodesB = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesB.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesB[0]->GetOperation());
    ASSERT_TRUE(concatNodesB[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodesB[0]->GetTerm());
}

TEST_FIXTURE(RETest, MultipleAlternativesOfSingleLiterals)
{
    RE regex("a|b|c");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{3}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodesA = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesA.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesA[0]->GetOperation());
    ASSERT_TRUE(concatNodesA[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodesA[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodesB = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesB.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesB[0]->GetOperation());
    ASSERT_TRUE(concatNodesB[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodesB[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[2]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[2]->GetTerm());
    auto const & concatNodesC = alternativeNodes[2]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesC.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesC[0]->GetOperation());
    ASSERT_TRUE(concatNodesC[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), concatNodesC[0]->GetTerm());
}

TEST_FIXTURE(RETest, EmptyLeftAlternative)
{
    RE regex("|a");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{2}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodesA = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{0}, concatNodesA.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodesB = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesB.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesB[0]->GetOperation());
    ASSERT_TRUE(concatNodesB[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodesB[0]->GetTerm());
}

TEST_FIXTURE(RETest, EmptyRightAlternative)
{
    RE regex("a|");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{2}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodesA = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesA.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesA[0]->GetOperation());
    ASSERT_TRUE(concatNodesA[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodesA[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodesB = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{0}, concatNodesB.size());
}

TEST_FIXTURE(RETest, Concatenation)
{
    RE regex("ab");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{2}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    ASSERT_TRUE(concatNodes[index]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodes[index]->GetTerm());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    ASSERT_TRUE(concatNodes[index]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodes[index]->GetTerm());
}

TEST_FIXTURE(RETest, MultipleConcatenation)
{
    RE regex("abc");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{3}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    ASSERT_TRUE(concatNodes[index]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodes[index]->GetTerm());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    ASSERT_TRUE(concatNodes[index]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodes[index]->GetTerm());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    ASSERT_TRUE(concatNodes[index]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), concatNodes[index]->GetTerm());
}

TEST_FIXTURE(RETest, OrWithMultipleTerms1)
{
    RE regex("a|bc");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{2}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodesA = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesA.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesA[0]->GetOperation());
    ASSERT_TRUE(concatNodesA[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodesA[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodesB = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{2}, concatNodesB.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesB[0]->GetOperation());
    ASSERT_TRUE(concatNodesB[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodesB[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesB[1]->GetOperation());
    ASSERT_TRUE(concatNodesB[1]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), concatNodesB[1]->GetTerm());
}

TEST_FIXTURE(RETest, OrWithMultipleTerms2)
{
    RE regex("ab|c");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{2}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodesA = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{2}, concatNodesA.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesA[0]->GetOperation());
    ASSERT_TRUE(concatNodesA[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodesA[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesA[1]->GetOperation());
    ASSERT_TRUE(concatNodesA[1]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodesA[1]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodesB = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodesB.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodesB[0]->GetOperation());
    ASSERT_TRUE(concatNodesB[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), concatNodesB[0]->GetTerm());
}

TEST_FIXTURE(RETest, SingleLiteralPlus)
{
    RE regex("a+");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 1, Term::Any), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, SingleLiteralAsterisk)
{
    RE regex("a*");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 0, Term::Any), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, SingleLiteralOptional)
{
    RE regex("a?");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 0, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSet)
{
    RE regex("[abcd]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Set, CharSet(CharSet::Range('a', 'd')), 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetRange)
{
    RE regex("[a-b]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Set, CharSet(CharSet::Range('a', 'b')), 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetMultipleRange)
{
    RE regex("[a-bp-q]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Set, CharSet(CharSet::Range('a', 'b') | CharSet::Range('p', 'q')), 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfNotSetRange)
{
    RE regex("[^a-b]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Set, ~CharSet(CharSet::Range('a', 'b')), 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetDigit)
{
    RE regex("[\\d]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet(CharSet::Range('0', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetNotDigit)
{
    RE regex("[\\D]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet(CharSet::Range('0', '9'));
    EXPECT_EQ(Term(Term::Type::Set, ~charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetAlphaChar)
{
    RE regex("[\\a]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet;
    charSet |= CharSet::Range('A', 'Z');
    charSet |= CharSet::Range('a', 'z');
    charSet |= '_';
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetNotAlphaChar)
{
    RE regex("[\\A]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet;
    charSet |= CharSet::Range('A', 'Z');
    charSet |= CharSet::Range('a', 'z');
    charSet |= '_';
    EXPECT_EQ(Term(Term::Type::Set, ~charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetWordChar)
{
    RE regex("[\\w]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet;
    charSet |= CharSet::Range('0', '9');
    charSet |= CharSet::Range('A', 'Z');
    charSet |= CharSet::Range('a', 'z');
    charSet |= '_';
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetNotWordChar)
{
    RE regex("[\\W]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet;
    charSet |= CharSet::Range('0', '9');
    charSet |= CharSet::Range('A', 'Z');
    charSet |= CharSet::Range('a', 'z');
    charSet |= '_';
    EXPECT_EQ(Term(Term::Type::Set, ~charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetWhitespaceChar)
{
    RE regex("[\\s]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet;
    charSet |= ' ';
    charSet |= '\t';
    charSet |= '\r';
    charSet |= '\n';
    charSet |= '\f';
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, OneOfSetNotWhitespaceChar)
{
    RE regex("[\\S]");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet;
    charSet |= ' ';
    charSet |= '\t';
    charSet |= '\r';
    charSet |= '\n';
    charSet |= '\f';
    EXPECT_EQ(Term(Term::Type::Set, ~charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RETest, Dot)
{
    RE regex(".");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet = CharSet::All;
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());

}

TEST_FIXTURE(RETest, ComplexLiteralsAndSets)
{
    RE regex("ab[c-e][f-h]ijk");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{7}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet('a');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('b');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('c', 'e'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('f', 'h'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('i');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('j');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('k');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, ComplexWithMultiplicities)
{
    RE regex("a*b+[c-e][f-h]ij?k.*");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{8}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet('a');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 0, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('b');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('c', 'e'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('f', 'h'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('i');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('j');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 0, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('k');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet::All;
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 0, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, StringLiteralWithEscapes)
{
    RE regex("\\.\\*\\+\\?\\$\\^\\/\\\\\\[\\]\\{\\}\\(\\)\\a\\b");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{16}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet('.');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('*');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('+');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('?');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('$');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('^');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('/');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('\\');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('[');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(']');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('{');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('}');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('(');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(')');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('a', 'z')) | CharSet(CharSet::Range('A', 'Z')) | CharSet('_');
    EXPECT_EQ(Term(Term::Type::AlphaChar, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('b');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, DecimalNumber)
{
    RE regex("[1-9][0-9]*");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{2}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet(CharSet::Range('1', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 0, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, OctalNumber)
{
    RE regex("[0-7]*");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet(CharSet::Range('0', '7'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 0, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, HexaDecimalNumber)
{
    RE regex("0[xX][0-9a-fA-F]+");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{3}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet('0');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet('x') | CharSet('X');
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '9')) | CharSet(CharSet::Range('a', 'f')) | CharSet(CharSet::Range('A', 'F'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, AnyNumber)
{
    RE regex("[1-9][0-9]*|[0-7]*|0[xX][0-9a-fA-F]+");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{3}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes0 = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{2}, concatNodes0.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes0[index]->GetOperation());
    CharSet charSet(CharSet::Range('1', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes0[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes0[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes0[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 0, Term::Any), concatNodes0[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes0[index]->GetNodes().size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodes1 = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes1.size());

    index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes1[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '7'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 0, Term::Any), concatNodes1[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes1[index]->GetNodes().size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[2]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[2]->GetTerm());
    auto const & concatNodes2 = alternativeNodes[2]->GetNodes();
    ASSERT_EQ(size_t{3}, concatNodes2.size());

    index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes2[index]->GetOperation());
    charSet = CharSet('0');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes2[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes2[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes2[index]->GetOperation());
    charSet = CharSet('x') | CharSet('X');
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes2[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes2[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes2[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '9')) | CharSet(CharSet::Range('a', 'f')) | CharSet(CharSet::Range('A', 'F'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, Term::Any), concatNodes2[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes2[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, AnyNumberShort)
{
    RE regex("[1-9]\\d*|[0-7]*|0[xX][\\da-fA-F]+");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{3}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes0 = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{2}, concatNodes0.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes0[index]->GetOperation());
    CharSet charSet(CharSet::Range('1', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes0[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes0[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes0[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '9'));
    EXPECT_EQ(Term(Term::Type::Digit, charSet, 0, Term::Any), concatNodes0[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes0[index]->GetNodes().size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[1]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[1]->GetTerm());
    auto const & concatNodes1 = alternativeNodes[1]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes1.size());

    index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes1[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '7'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 0, Term::Any), concatNodes1[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes1[index]->GetNodes().size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[2]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[2]->GetTerm());
    auto const & concatNodes2 = alternativeNodes[2]->GetNodes();
    ASSERT_EQ(size_t{3}, concatNodes2.size());

    index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes2[index]->GetOperation());
    charSet = CharSet('0');
    EXPECT_EQ(Term(Term::Type::Literal, charSet, 1, 1), concatNodes2[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes2[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes2[index]->GetOperation());
    charSet = CharSet('x') | CharSet('X');
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes2[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes2[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes2[index]->GetOperation());
    charSet = CharSet(CharSet::Range('0', '9')) | CharSet(CharSet::Range('a', 'f')) | CharSet(CharSet::Range('A', 'F'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, Term::Any), concatNodes2[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes2[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, Word)
{
    RE regex("[a-zA-Z]+");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    CharSet charSet(CharSet::Range('a', 'z') | CharSet::Range('A', 'Z'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, SubExpression)
{
    RE regex("a(bb)+a");
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeNodes[0]->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{3}, concatNodes.size());

    size_t index = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Expression, concatNodes[index]->GetOperation());
    EXPECT_EQ(Term(Term::Type::None, CharSet(), 1, Term::Any), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{1}, concatNodes[index]->GetNodes().size());
    auto node = concatNodes[index]->GetNodes()[0];

    EXPECT_EQ(ASTNode::Operation::Or, node->GetOperation());
    EXPECT_EQ(Term(), node->GetTerm());

    auto const & alternativeSubNodes = node->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeSubNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeSubNodes[0]->GetOperation());
    EXPECT_EQ(Term(), alternativeSubNodes[0]->GetTerm());
    auto const & concatSubNodes = alternativeSubNodes[0]->GetNodes();
    ASSERT_EQ(size_t{2}, concatSubNodes.size());

    size_t subIndex = 0;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatSubNodes[subIndex]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'b', 1, 1), concatSubNodes[subIndex]->GetTerm());
    ASSERT_EQ(size_t{0}, concatSubNodes[subIndex]->GetNodes().size());

    ++subIndex;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatSubNodes[subIndex]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'b', 1, 1), concatSubNodes[subIndex]->GetTerm());
    ASSERT_EQ(size_t{0}, concatSubNodes[subIndex]->GetNodes().size());

    ++index;
    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[index]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 1, 1), concatNodes[index]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[index]->GetNodes().size());
}

TEST_FIXTURE(RETest, Match)
{
    RE regex("a(bb)+a");
    EXPECT_TRUE(regex.Match("abba"));
    EXPECT_TRUE(regex.Match("abbbba"));
    EXPECT_TRUE(regex.Match("abbbbbba"));
    EXPECT_FALSE(regex.Match("a"));
    EXPECT_FALSE(regex.Match("b"));
    EXPECT_FALSE(regex.Match("aa"));
    EXPECT_FALSE(regex.Match("aba"));
    EXPECT_FALSE(regex.Match("abbba"));
    EXPECT_FALSE(regex.Match("abbbbba"));
    EXPECT_FALSE(regex.Match("abbbbbbba"));
    EXPECT_TRUE(regex.PartialMatch("abba"));
    EXPECT_TRUE(regex.PartialMatch(""));
    EXPECT_TRUE(regex.PartialMatch("a"));
    EXPECT_TRUE(regex.PartialMatch("ab"));
    EXPECT_TRUE(regex.PartialMatch("abb"));
    EXPECT_TRUE(regex.PartialMatch("abbb"));
}

TEST_FIXTURE(RETest, MatchHexadecimalNumber)
{
    RE regex("0[xX][0-9a-fA-F]+");
    EXPECT_FALSE(regex.Match(""));
    EXPECT_FALSE(regex.Match("0"));
    EXPECT_FALSE(regex.Match("0x"));
    EXPECT_FALSE(regex.Match("0X"));
    EXPECT_TRUE(regex.Match("0x0"));
    EXPECT_TRUE(regex.Match("0X0"));
    EXPECT_TRUE(regex.Match("0x01"));
    EXPECT_TRUE(regex.Match("0x0123456789aBcDeF"));
    EXPECT_FALSE(regex.Match("0x0123456789aBcDeFg"));
    EXPECT_FALSE(regex.Match("0xx0123456789aBcDeF"));
    EXPECT_TRUE(regex.PartialMatch(""));
    EXPECT_TRUE(regex.PartialMatch("0"));
    EXPECT_TRUE(regex.PartialMatch("0x"));
    EXPECT_TRUE(regex.PartialMatch("0X"));
    EXPECT_TRUE(regex.PartialMatch("0x0"));
    EXPECT_TRUE(regex.PartialMatch("0X0"));
    EXPECT_TRUE(regex.PartialMatch("0x0f"));
    EXPECT_FALSE(regex.PartialMatch("0x0g"));
    EXPECT_FALSE(regex.PartialMatch("0xx"));
}

TEST_FIXTURE(RETest, MatchDecimalNumber)
{
    RE regex("[1-9][0-9]*|0");
    EXPECT_FALSE(regex.Match(""));
    EXPECT_TRUE(regex.Match("0"));
    EXPECT_TRUE(regex.Match("1"));
    EXPECT_FALSE(regex.Match("01"));
    EXPECT_TRUE(regex.Match("10"));
    EXPECT_TRUE(regex.Match("1234567890"));
    EXPECT_FALSE(regex.Match("0x0123456789aBcDeF"));
    EXPECT_FALSE(regex.Match("1234567890A"));
    EXPECT_TRUE(regex.PartialMatch(""));
    EXPECT_TRUE(regex.PartialMatch("0"));
    EXPECT_TRUE(regex.PartialMatch("1"));
    EXPECT_FALSE(regex.PartialMatch("01"));
    EXPECT_TRUE(regex.PartialMatch("10"));
    EXPECT_TRUE(regex.PartialMatch("12"));
    EXPECT_TRUE(regex.PartialMatch("123"));
    EXPECT_TRUE(regex.PartialMatch("1234"));
    EXPECT_TRUE(regex.PartialMatch("12345"));
    EXPECT_TRUE(regex.PartialMatch("123456"));
    EXPECT_TRUE(regex.PartialMatch("1234567"));
    EXPECT_FALSE(regex.PartialMatch("0x"));
    EXPECT_FALSE(regex.PartialMatch("0A"));
}

TEST_FIXTURE(RETest, LineComment)
{
    RE regex("\\/\\/.*");

    EXPECT_FALSE(regex.Match(""));
    EXPECT_FALSE(regex.Match("/"));
    EXPECT_TRUE(regex.Match("//"));
    EXPECT_TRUE(regex.Match("//abcd"));
}

TEST_FIXTURE(RETest, BlockComment)
{
    RE regex("\\/\\*[^/\\*]*\\*\\/");

    EXPECT_FALSE(regex.Match(""));
    EXPECT_FALSE(regex.Match("/*"));
    EXPECT_TRUE(regex.Match("/**/"));
    EXPECT_TRUE(regex.Match("/*abcd*/"));
}

TEST_FIXTURE(RETest, NewLine)
{
    RE regex("\n");

    EXPECT_FALSE(regex.Match(""));
    EXPECT_TRUE(regex.Match("\n"));
}

TEST_FIXTURE(RETest, String)
{
    RE regex("\\\"[^\\\"]*\\\"");

    EXPECT_FALSE(regex.Match(""));
    EXPECT_FALSE(regex.Match("\""));
    EXPECT_TRUE(regex.PartialMatch("\""));
    EXPECT_TRUE(regex.Match("\"\""));
    EXPECT_TRUE(regex.Match("\"a\""));
    EXPECT_TRUE(regex.PartialMatch("\"a\""));
    EXPECT_FALSE(regex.Match("\"a\"a"));
    EXPECT_FALSE(regex.PartialMatch("\"a\"a"));
}

} // TEST_SUITE(regex)

} // namespace Test
} // namespace Regex
