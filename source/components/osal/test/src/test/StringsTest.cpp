#include <unit-test-c++/UnitTestC++.h>

#include "osal/strings.h"

namespace OSAL {
namespace Test {

class StringTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void StringTest::SetUp()
{
}

void StringTest::TearDown()
{
}

TEST_FIXTURE(StringTest, Trim)
{
    EXPECT_EQ("abc", Trim("abc", "'\""));
    EXPECT_EQ("abc", Trim("'abc'", "'\""));
    EXPECT_EQ("abc", Trim("\"abc\"", "'\""));
    EXPECT_EQ("a'bc", Trim("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringTest, TrimSpaces)
{
    EXPECT_EQ("abc", TrimSpaces("abc"));
    EXPECT_EQ("abc", TrimSpaces(" abc "));
    EXPECT_EQ("ab c", TrimSpaces(" ab c "));
}

TEST_FIXTURE(StringTest, TrimLeft)
{
    EXPECT_EQ("abc", TrimLeft("abc", "'\""));
    EXPECT_EQ("abc'", TrimLeft("'abc'", "'\""));
    EXPECT_EQ("abc\"", TrimLeft("\"abc\"", "'\""));
    EXPECT_EQ("a'bc\"", TrimLeft("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringTest, TrimSpacesLeft)
{
    EXPECT_EQ("abc", TrimSpacesLeft("abc"));
    EXPECT_EQ("abc ", TrimSpacesLeft(" abc "));
    EXPECT_EQ("ab c ", TrimSpacesLeft(" ab c "));
}

TEST_FIXTURE(StringTest, TrimRight)
{
    EXPECT_EQ("abc", TrimRight("abc", "'\""));
    EXPECT_EQ("'abc", TrimRight("'abc'", "'\""));
    EXPECT_EQ("\"abc", TrimRight("\"abc\"", "'\""));
    EXPECT_EQ("\"a'bc", TrimRight("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringTest, TrimSpaceRight)
{
    EXPECT_EQ("abc", TrimSpacesRight("abc"));
    EXPECT_EQ(" abc", TrimSpacesRight(" abc "));
    EXPECT_EQ(" ab c", TrimSpacesRight(" ab c "));
}

} // namespace Test
} // namespace OSAL