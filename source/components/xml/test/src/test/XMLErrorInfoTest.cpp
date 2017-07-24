#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "xml/XMLErrorInfo.h"

using namespace std;

namespace XMLParser
{

namespace Test
{

const XMLError ErrorCode = XMLError::Error;
const string ErrorDescription = "Error";
const int Row = 2;
const int Col = 3;

class XMLErrorInfoTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLErrorInfoTest::SetUp()
{
}

void XMLErrorInfoTest::TearDown()
{
}

TEST_FIXTURE(XMLErrorInfoTest, Construct)
{
    XMLErrorInfo target(ErrorCode, XMLCursor(Row, Col));

    EXPECT_EQ(ErrorCode, target.ErrorCode());
    EXPECT_EQ(ErrorDescription, target.ErrorDescription());
    EXPECT_EQ(Row + 1, target.ErrorRow());
    EXPECT_EQ(Col + 1, target.ErrorCol());
}

TEST_FIXTURE(XMLErrorInfoTest, Clear)
{
    XMLErrorInfo target(ErrorCode, XMLCursor(Row, Col));

    target.Clear();

    EXPECT_EQ(XMLError::NoError, target.ErrorCode());
    EXPECT_EQ("", target.ErrorDescription());
    EXPECT_EQ(0, target.ErrorRow());
    EXPECT_EQ(0, target.ErrorCol());
}

} // namespace Test

} // namespace XMLParser

