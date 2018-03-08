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
//    Regex regex("abc");
//    EXPECT_FALSE(regex.Match(""));
//    EXPECT_TRUE(regex.PartialMatch(""));
//    EXPECT_FALSE(regex.Match("abc"));
//    EXPECT_TRUE(regex.PartialMatch("abc"));
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
