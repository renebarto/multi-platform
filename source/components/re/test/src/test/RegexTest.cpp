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

TEST_FIXTURE(RegexTest, ConstructDefault)
{
    Regex regex;
    EXPECT_FALSE(regex.Match(""));
    EXPECT_TRUE(regex.PartialMatch(""));
    EXPECT_FALSE(regex.Match("abc"));
    EXPECT_TRUE(regex.PartialMatch("abc"));
}

TEST_FIXTURE(RegexTest, ConstructStringLiteral)
{
    Regex regex("abc");
    cout << regex;
    ASSERT_EQ(size_t{0}, regex.GetElements().size());
    ASSERT_EQ(size_t{1}, regex.GetTerms().size());
    ASSERT_EQ(size_t{3}, regex.GetTerms()[0].GetElements().size());
    ASSERT_EQ(size_t{0}, regex.GetTerms()[0].GetTerms().size());
    auto const & elements = regex.GetTerms()[0].GetElements();
    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements[0]);
    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements[1]);
    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 1, 1), elements[2]);
}

TEST_FIXTURE(RegexTest, ConstructStringLiteralWithRange)
{
    Regex regex("abc[de]");
    cout << regex;
    ASSERT_EQ(size_t{0}, regex.GetElements().size());
    ASSERT_EQ(size_t{2}, regex.GetTerms().size());
    ASSERT_EQ(size_t{3}, regex.GetTerms()[0].GetElements().size());
    ASSERT_EQ(size_t{0}, regex.GetTerms()[0].GetTerms().size());
    ASSERT_EQ(size_t{1}, regex.GetTerms()[1].GetElements().size());
    ASSERT_EQ(size_t{0}, regex.GetTerms()[1].GetTerms().size());
    auto const & elements0 = regex.GetTerms()[0].GetElements();
    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'a', 1, 1), elements0[0]);
    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'b', 1, 1), elements0[1]);
    EXPECT_EQ(TermElement(TermElement::Type::Literal, 'c', 1, 1), elements0[2]);
    auto const & elements1 = regex.GetTerms()[1].GetElements();
    EXPECT_EQ(TermElement(TermElement::Type::Set, CharSet(CharSet::Range('d', 'e')), 1, 1), elements1[0]);
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
