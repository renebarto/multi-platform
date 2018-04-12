#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "regex/InputReader.h"
#include "regex/TestData.h"

using namespace std;

namespace Regex {
namespace Test {

class InputReaderTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(regex) {

TEST_FIXTURE(InputReaderTest, ConstructDefault)
{
    InputReader reader;
    EXPECT_TRUE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Expect('a'));
    char ch {};
    EXPECT_FALSE(reader.GetChar(ch));
    EXPECT_FALSE(reader.LookAheadChar(ch));
}

TEST_FIXTURE(InputReaderTest, Empty)
{
    InputReader reader("");
    EXPECT_TRUE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Expect('a'));
    char ch {};
    EXPECT_FALSE(reader.GetChar(ch));
    EXPECT_FALSE(reader.LookAheadChar(ch));
}

TEST_FIXTURE(InputReaderTest, GetChar)
{
    InputReader reader("abc");
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_TRUE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    char ch {};

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('a', ch);
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_TRUE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('b', ch);
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_TRUE(reader.Have('c'));

    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('c', ch);
    EXPECT_TRUE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));
}

TEST_FIXTURE(InputReaderTest, LookAheadChar)
{
    InputReader reader("abc");
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_TRUE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    char ch {};

    EXPECT_TRUE(reader.LookAheadChar(ch));
    EXPECT_EQ('b', ch);
    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('a', ch);
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_TRUE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    EXPECT_TRUE(reader.LookAheadChar(ch));
    EXPECT_EQ('c', ch);
    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('b', ch);
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_TRUE(reader.Have('c'));

    EXPECT_FALSE(reader.LookAheadChar(ch));
    EXPECT_TRUE(reader.GetChar(ch));
    EXPECT_EQ('c', ch);
    EXPECT_TRUE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));
}

TEST_FIXTURE(InputReaderTest, Expect)
{
    InputReader reader("abc");
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_TRUE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    EXPECT_TRUE(reader.Expect('a'));
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_TRUE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    EXPECT_TRUE(reader.Expect('b'));
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_TRUE(reader.Have('c'));

    EXPECT_TRUE(reader.Expect('c'));
    EXPECT_TRUE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));
    EXPECT_FALSE(reader.Expect('c'));
}

TEST_FIXTURE(InputReaderTest, SetInput)
{
    InputReader reader;
    reader.SetInput("abc");
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_TRUE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    EXPECT_TRUE(reader.Expect('a'));
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_TRUE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));

    EXPECT_TRUE(reader.Expect('b'));
    EXPECT_FALSE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_TRUE(reader.Have('c'));

    EXPECT_TRUE(reader.Expect('c'));
    EXPECT_TRUE(reader.AtEnd());
    EXPECT_FALSE(reader.Have('a'));
    EXPECT_FALSE(reader.Have('b'));
    EXPECT_FALSE(reader.Have('c'));
    EXPECT_FALSE(reader.Expect('c'));
}

} // TEST_SUITE(regex)

} // namespace Test
} // namespace Regex
