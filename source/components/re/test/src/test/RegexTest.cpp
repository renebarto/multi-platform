#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/Regex.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class RegexTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(re) {

TEST_FIXTURE(RegexTest, Empty)
{
    Regex regex("");
    auto const & ast = regex.GetAST();
    EXPECT_TRUE(ast.IsEmpty());
    auto const & nfa = regex.GetNFA();
    EXPECT_TRUE(nfa.GetRules().empty());
    auto const & dfa = regex.GetDFA();
    EXPECT_TRUE(dfa.GetRules().empty());
}

TEST_FIXTURE(RegexTest, SingleLiteral)
{
    Regex regex("a");
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

//    auto const & dfa = regex.GetDFA();
//    ASSERT_EQ(size_t {1}, dfa.GetRules().size());
//
//    EXPECT_EQ(CharSet('a'), dfa.GetRules()[0].ExpectedInput());
//    EXPECT_EQ(1, dfa.GetRules()[0].ExpectedState());
//    EXPECT_EQ(0, dfa.GetRules()[0].NextState());
}

TEST_FIXTURE(RegexTest, AlternativesOfSingleLiterals)
{
    Regex regex("a|b");
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

TEST_FIXTURE(RegexTest, MultipleAlternativesOfSingleLiterals)
{
    Regex regex("a|b|c");
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

TEST_FIXTURE(RegexTest, EmptyLeftAlternative)
{
    Regex regex("|a");
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

TEST_FIXTURE(RegexTest, EmptyRightAlternative)
{
    Regex regex("a|");
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

TEST_FIXTURE(RegexTest, Concatenation)
{
    Regex regex("ab");
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

TEST_FIXTURE(RegexTest, MultipleConcatenation)
{
    Regex regex("abc");
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

TEST_FIXTURE(RegexTest, OrWithMultipleTerms1)
{
    Regex regex("a|bc");
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

TEST_FIXTURE(RegexTest, OrWithMultipleTerms2)
{
    Regex regex("ab|c");
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

TEST_FIXTURE(RegexTest, SingleLiteralPlus)
{
    Regex regex("a+");
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

TEST_FIXTURE(RegexTest, SingleLiteralAsterisk)
{
    Regex regex("a*");
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

TEST_FIXTURE(RegexTest, SingleLiteralOptional)
{
    Regex regex("a?");
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

TEST_FIXTURE(RegexTest, OneOfSet)
{
    Regex regex("[abcd]");
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

TEST_FIXTURE(RegexTest, OneOfSetRange)
{
    Regex regex("[a-b]");
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

TEST_FIXTURE(RegexTest, OneOfSetMultipleRange)
{
    Regex regex("[a-bp-q]");
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

TEST_FIXTURE(RegexTest, OneOfNotSetRange)
{
    Regex regex("[^a-b]");
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

TEST_FIXTURE(RegexTest, OneOfSetDigit)
{
    Regex regex("[\\d]");
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

TEST_FIXTURE(RegexTest, OneOfSetNotDigit)
{
    Regex regex("[\\D]");
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

TEST_FIXTURE(RegexTest, OneOfSetAlphaChar)
{
    Regex regex("[\\a]");
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

TEST_FIXTURE(RegexTest, OneOfSetNotAlphaChar)
{
    Regex regex("[\\A]");
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

TEST_FIXTURE(RegexTest, OneOfSetWordChar)
{
    Regex regex("[\\w]");
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

TEST_FIXTURE(RegexTest, OneOfSetNotWordChar)
{
    Regex regex("[\\W]");
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

TEST_FIXTURE(RegexTest, OneOfSetWhitespaceChar)
{
    Regex regex("[\\s]");
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

TEST_FIXTURE(RegexTest, OneOfSetNotWhitespaceChar)
{
    Regex regex("[\\S]");
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

TEST_FIXTURE(RegexTest, Dot)
{
    Regex regex(".");
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

TEST_FIXTURE(RegexTest, ComplexLiteralsAndSets)
{
    Regex regex("ab[c-e][f-h]ijk");
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

TEST_FIXTURE(RegexTest, ComplexWithMultiplicities)
{
    Regex regex("a*b+[c-e][f-h]ij?k.*");
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

TEST_FIXTURE(RegexTest, StringLiteralWithEscapes)
{
    Regex regex("\\.\\*\\+\\?\\$\\^\\/\\\\\\[\\]\\{\\}\\(\\)\\a\\b");
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

TEST_FIXTURE(RegexTest, DecimalNumber)
{
    Regex regex("[1-9][0-9]*");
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

TEST_FIXTURE(RegexTest, OctalNumber)
{
    Regex regex("[0-7]*");
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

TEST_FIXTURE(RegexTest, HexaDecimalNumber)
{
    Regex regex("0[xX][0-9a-fA-F]+");
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

TEST_FIXTURE(RegexTest, AnyNumber)
{
    Regex regex("[1-9][0-9]*|[0-7]*|0[xX][0-9a-fA-F]+");
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

TEST_FIXTURE(RegexTest, AnyNumberShort)
{
    Regex regex("[1-9]\\d*|[0-7]*|0[xX][\\da-fA-F]+");
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

TEST_FIXTURE(RegexTest, Word)
{
    Regex regex("[a-zA-Z]+");
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

TEST_FIXTURE(RegexTest, SubExpression)
{
    Regex regex("a(bb)+a");
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

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
