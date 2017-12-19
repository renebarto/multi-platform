#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "xml/XMLDefs.h"

using namespace std;

namespace XMLParser {
namespace Test {

const int Row = 2;
const int Col = 3;

class XMLCursorTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLCursorTest::SetUp()
{
}

void XMLCursorTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLCursorTest, ConstructDefault)
{
    XMLCursor target;

    EXPECT_EQ(-1, target.Row());
    EXPECT_EQ(-1, target.Col());
}

TEST_FIXTURE(XMLCursorTest, Construct)
{
    XMLCursor target(Row, Col);

    EXPECT_EQ(Row, target.Row());
    EXPECT_EQ(Col, target.Col());
}

TEST_FIXTURE(XMLCursorTest, Set)
{
    XMLCursor target;

    target.Set(Row, Col);
    EXPECT_EQ(Row, target.Row());
    EXPECT_EQ(Col, target.Col());
}

TEST_FIXTURE(XMLCursorTest, Clear)
{
    XMLCursor target;

    target.Set(Row, Col);
    target.Clear();
    EXPECT_EQ(-1, target.Row());
    EXPECT_EQ(-1, target.Col());
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
