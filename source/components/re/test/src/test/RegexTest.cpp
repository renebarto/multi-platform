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

TEST_FIXTURE(RegexTest, ConstructEmpty)
{
    Regex regex("");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_TRUE(ast.IsEmpty());
}

TEST_FIXTURE(RegexTest, ConstructAlternatives)
{
    Regex regex("a|b");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, ast.Root()->GetOperation());
    auto leftNode = rootNode->GetLeftNode();
    auto rightNode = rootNode->GetRightNode();
    ASSERT_NOT_NULL(leftNode);
    ASSERT_NOT_NULL(rightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftNode->GetOperation());
    ASSERT_NULL(leftNode->GetLeftNode());
    ASSERT_NULL(leftNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), leftNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Leaf, rightNode->GetOperation());
    ASSERT_NULL(rightNode->GetLeftNode());
    ASSERT_NULL(rightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), rightNode->GetTerm());
}

TEST_FIXTURE(RegexTest, ConstructMultipleAlternatives)
{
    Regex regex("a|b|c");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, ast.Root()->GetOperation());
    auto leftNode = rootNode->GetLeftNode();
    auto rightNode = rootNode->GetRightNode();
    ASSERT_NOT_NULL(leftNode);
    ASSERT_NOT_NULL(rightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftNode->GetOperation());
    ASSERT_NULL(leftNode->GetLeftNode());
    ASSERT_NULL(leftNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), leftNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Or, rightNode->GetOperation());
    auto leftRightNode = rightNode->GetLeftNode();
    auto rightRightNode = rightNode->GetRightNode();
    ASSERT_NOT_NULL(leftRightNode);
    ASSERT_NOT_NULL(rightRightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftRightNode->GetOperation());
    ASSERT_NULL(leftRightNode->GetLeftNode());
    ASSERT_NULL(leftRightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), leftRightNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Leaf, rightRightNode->GetOperation());
    ASSERT_NULL(rightRightNode->GetLeftNode());
    ASSERT_NULL(rightRightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), rightRightNode->GetTerm());
}

TEST_FIXTURE(RegexTest, ConstructEmptyLeftAlternative)
{
    Regex regex("|a");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, ast.Root()->GetOperation());
    auto leftNode = rootNode->GetLeftNode();
    auto rightNode = rootNode->GetRightNode();
    ASSERT_NOT_NULL(leftNode);
    ASSERT_NOT_NULL(rightNode);
    EXPECT_EQ(ASTNode::Operation::Null, leftNode->GetOperation());
    ASSERT_NULL(leftNode->GetLeftNode());
    ASSERT_NULL(leftNode->GetRightNode());
    EXPECT_EQ(ASTNode::Operation::Leaf, rightNode->GetOperation());
    ASSERT_NULL(rightNode->GetLeftNode());
    ASSERT_NULL(rightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), rightNode->GetTerm());
}

TEST_FIXTURE(RegexTest, ConstructEmptyRightAlternative)
{
    Regex regex("a|");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Or, ast.Root()->GetOperation());
    auto leftNode = rootNode->GetLeftNode();
    auto rightNode = rootNode->GetRightNode();
    ASSERT_NOT_NULL(leftNode);
    ASSERT_NOT_NULL(rightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftNode->GetOperation());
    ASSERT_NULL(leftNode->GetLeftNode());
    ASSERT_NULL(leftNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), leftNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Null, rightNode->GetOperation());
    ASSERT_NULL(rightNode->GetLeftNode());
    ASSERT_NULL(rightNode->GetRightNode());
}

TEST_FIXTURE(RegexTest, ConstructConcatenation)
{
    Regex regex("ab");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Concat, ast.Root()->GetOperation());
    auto leftNode = rootNode->GetLeftNode();
    auto rightNode = rootNode->GetRightNode();
    ASSERT_NOT_NULL(leftNode);
    ASSERT_NOT_NULL(rightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftNode->GetOperation());
    ASSERT_NULL(leftNode->GetLeftNode());
    ASSERT_NULL(leftNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), leftNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Leaf, rightNode->GetOperation());
    ASSERT_NULL(rightNode->GetLeftNode());
    ASSERT_NULL(rightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), rightNode->GetTerm());
}

TEST_FIXTURE(RegexTest, ConstructMultipleConcatenation)
{
    Regex regex("abc");
    cout << regex;
    auto const & ast = regex.GetAST();
    EXPECT_FALSE(ast.IsEmpty());
    auto rootNode = ast.Root();
    EXPECT_EQ(ASTNode::Operation::Concat, ast.Root()->GetOperation());
    auto leftNode = rootNode->GetLeftNode();
    auto rightNode = rootNode->GetRightNode();
    ASSERT_NOT_NULL(leftNode);
    ASSERT_NOT_NULL(rightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftNode->GetOperation());
    ASSERT_NULL(leftNode->GetLeftNode());
    ASSERT_NULL(leftNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'a'), leftNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Concat, rightNode->GetOperation());
    auto leftRightNode = rightNode->GetLeftNode();
    auto rightRightNode = rightNode->GetRightNode();
    ASSERT_NOT_NULL(leftRightNode);
    ASSERT_NOT_NULL(rightRightNode);
    EXPECT_EQ(ASTNode::Operation::Leaf, leftRightNode->GetOperation());
    ASSERT_NULL(leftRightNode->GetLeftNode());
    ASSERT_NULL(leftRightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'b'), leftRightNode->GetTerm());
    EXPECT_EQ(ASTNode::Operation::Leaf, rightRightNode->GetOperation());
    ASSERT_NULL(rightRightNode->GetLeftNode());
    ASSERT_NULL(rightRightNode->GetRightNode());
    EXPECT_EQ(Term(Term::Type::Literal, 'c'), rightRightNode->GetTerm());
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
