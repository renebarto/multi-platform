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
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_TRUE(ast.IsEmpty());
}

TEST_FIXTURE(RegexTest, SingleLiteral)
{
    Regex regex("a");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, AlternativesOfSingleLiterals)
{
    Regex regex("a|b");
    cout << regex;
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
    cout << regex;
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
    cout << regex;
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
    cout << regex;
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
    cout << regex;
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

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    ASSERT_TRUE(concatNodes[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodes[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[1]->GetOperation());
    ASSERT_TRUE(concatNodes[1]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodes[1]->GetTerm());
}

TEST_FIXTURE(RegexTest, MultipleConcatenation)
{
    Regex regex("abc");
    cout << regex;
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

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    ASSERT_TRUE(concatNodes[0]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), concatNodes[0]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[1]->GetOperation());
    ASSERT_TRUE(concatNodes[1]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), concatNodes[1]->GetTerm());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[2]->GetOperation());
    ASSERT_TRUE(concatNodes[2]->GetNodes().empty());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), concatNodes[2]->GetTerm());
}

TEST_FIXTURE(RegexTest, OrWithMultipleTerms1)
{
    Regex regex("a|bc");
    cout << regex;
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
    cout << regex;
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
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 1, Term::Any), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, SingleLiteralAsterisk)
{
    Regex regex("a*");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 0, Term::Any), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, SingleLiteralOptional)
{
    Regex regex("a?");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Literal, 'a', 0, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, OneOfSet)
{
    Regex regex("[abcd]");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Set, CharSet(CharSet::Range('a', 'd')), 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, OneOfSetRange)
{
    Regex regex("[a-b]");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    EXPECT_EQ(Term(Term::Type::Set, CharSet(CharSet::Range('a', 'b')), 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, OneOfSetDigit)
{
    Regex regex("[\\d]");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
    auto const & concatNodes = alternativeNodes[0]->GetNodes();
    ASSERT_EQ(size_t{1}, concatNodes.size());

    EXPECT_EQ(ASTNode::Operation::Leaf, concatNodes[0]->GetOperation());
    CharSet charSet(CharSet::Range('0', '9'));
    EXPECT_EQ(Term(Term::Type::Set, charSet, 1, 1), concatNodes[0]->GetTerm());
    ASSERT_EQ(size_t{0}, concatNodes[0]->GetNodes().size());
}

TEST_FIXTURE(RegexTest, OneOfSetAlphaChar)
{
    Regex regex("[\\a]");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
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

TEST_FIXTURE(RegexTest, OneOfSetWordChar)
{
    Regex regex("[\\w]");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
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

TEST_FIXTURE(RegexTest, OneOfSetWhitespaceChar)
{
//    Regex regex("[\\d\\w\\s\\D\\W\\S]");
    // Whitespace is [' ', '\t', '\f', '\r', '\n']
    Regex regex("[\\s]");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    ASSERT_NOT_NULL(rootNode);
    EXPECT_EQ(ASTNode::Operation::Or, rootNode->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());

    auto const & alternativeNodes = rootNode->GetNodes();
    ASSERT_EQ(size_t{1}, alternativeNodes.size());

    EXPECT_EQ(ASTNode::Operation::Concat, alternativeNodes[0]->GetOperation());
    EXPECT_EQ(Term(), rootNode->GetTerm());
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

//TEST_FIXTURE(RegexTest, ConstructDefault)
//{
//    Regex regex;
//    EXPECT_FALSE(regex.Match(""));
//    EXPECT_TRUE(regex.PartialMatch(""));
//    EXPECT_FALSE(regex.Match("abc"));
//    EXPECT_TRUE(regex.PartialMatch("abc"));
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteral)
//{
//    Regex regex("abc");
//    cout << regex;
//    ASSERT_EQ(size_t{3}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 1, 1), elements[2]);
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteralWithRange)
//{
//    Regex regex("abc[de]");
//    cout << regex;
//    ASSERT_EQ(size_t{4}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 1, 1), elements[2]);
//    EXPECT_EQ(TermElement(TermElement::Type::Set, CharSet(CharSet::Range('d', 'e')), 1, 1), elements[3]);
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteralWithEscapes)
//{
//    Regex regex("\\.\\*\\+\\?\\$\\^\\/\\\\\\[\\]\\{\\}\\(\\)\\a");
//    cout << regex;
//    ASSERT_EQ(size_t{15}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements0 = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '.', 1, 1), elements0[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '*', 1, 1), elements0[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '+', 1, 1), elements0[2]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '?', 1, 1), elements0[3]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '$', 1, 1), elements0[4]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '^', 1, 1), elements0[5]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '/', 1, 1), elements0[6]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '\\', 1, 1), elements0[7]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '[', 1, 1), elements0[8]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, ']', 1, 1), elements0[9]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '{', 1, 1), elements0[10]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '}', 1, 1), elements0[11]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, '(', 1, 1), elements0[12]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, ')', 1, 1), elements0[13]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements0[14]);
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteralWithStartEnd)
//{
//    Regex regex("$abc^");
//    cout << regex;
//    ASSERT_EQ(size_t{5}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements0 = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::LineBegin, CharSet(), 1, 1), elements0[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements0[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements0[2]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 1, 1), elements0[3]);
//    EXPECT_EQ(TermElement(TermElement::Type::LineEnd, CharSet(), 1, 1), elements0[4]);
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteralQuestionMark)
//{
//    Regex regex("abc?");
//    cout << regex;
//    ASSERT_EQ(size_t{3}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements0 = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements0[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements0[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 0, 1), elements0[2]);
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteralAsterisk)
//{
//    Regex regex("abc*");
//    cout << regex;
//    ASSERT_EQ(size_t{3}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements0 = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements0[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements0[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 0, -1), elements0[2]);
//}
//
//TEST_FIXTURE(RegexTest, ConstructStringLiteralPlus)
//{
//    Regex regex("abc+");
//    cout << regex;
//    ASSERT_EQ(size_t{3}, regex.GetElements().size());
//    ASSERT_EQ(size_t{0}, regex.GetTerms().size());
//    auto const & elements0 = regex.GetElements();
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements0[0]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements0[1]);
//    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 1, -1), elements0[2]);
//}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
