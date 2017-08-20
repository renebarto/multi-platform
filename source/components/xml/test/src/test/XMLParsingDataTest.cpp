#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "xml/XMLBase.h"
#include "xml/XMLParsingData.h"

using namespace std;

namespace XMLParser {
namespace Test {

const int Row = 2;
const int Col = 3;
const int Tab = 4;

class XMLParsingDataTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLParsingDataTest::SetUp()
{
    XMLBase::SetTabSize(0);
}

void XMLParsingDataTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLParsingDataTest, Construct)
{
    const char * xml = "Some string";
    XMLParsingData target(xml);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(-1, target.Cursor().Row());
    EXPECT_EQ(-1, target.Cursor().Col());
}

TEST_FIXTURE(XMLParsingDataTest, UpdateCursor)
{
    const char * xml = "Some string";
    XMLParsingData target(xml);
    XMLCursor cursor(Row, Col);

    target.UpdateCursor(cursor);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(Row, target.Cursor().Row());
    EXPECT_EQ(Col, target.Cursor().Col());
}

TEST_FIXTURE(XMLParsingDataTest, Update)
{
    const char * xml = "Some string";
    XMLParsingData target(xml);

    target.Update(xml, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 1, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 1, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(1, target.Cursor().Col());

    target.Update(xml + 5, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 5, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(5, target.Cursor().Col());

    target.Update(xml + 10, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 10, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(10, target.Cursor().Col());

    target.Update(xml + 11, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 11, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(11, target.Cursor().Col());
}

TEST_FIXTURE(XMLParsingDataTest, UpdateMultiLineLF)
{
    const char * xml = "Some\nstring";
    XMLParsingData target(xml);

    target.Update(xml, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 1, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 1, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(1, target.Cursor().Col());

    target.Update(xml + 5, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 5, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 10, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 10, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(5, target.Cursor().Col());

    target.Update(xml + 11, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 11, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(6, target.Cursor().Col());
}

TEST_FIXTURE(XMLParsingDataTest, UpdateMultiLineCRLF)
{
    const char * xml = "Some\r\nstring";
    XMLParsingData target(xml);

    target.Update(xml, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 1, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 1, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(1, target.Cursor().Col());

    target.Update(xml + 5, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 6, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 10, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 10, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(4, target.Cursor().Col());

    target.Update(xml + 11, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 11, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(5, target.Cursor().Col());
}

TEST_FIXTURE(XMLParsingDataTest, UpdateBOMNullChars)
{
    const char * xml = "\xef\xbb\xbf""A\xef\xbf\xbe""B\xef\xbf\xbf""C";
    XMLParsingData target(xml);

    target.Update(xml, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 1, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 3, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 4, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 4, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(1, target.Cursor().Col());

    target.Update(xml + 5, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 7, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(1, target.Cursor().Col());

    target.Update(xml + 8, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 8, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(2, target.Cursor().Col());

    target.Update(xml + 9, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 11, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(2, target.Cursor().Col());

    target.Update(xml + 12, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 12, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(3, target.Cursor().Col());
}

TEST_FIXTURE(XMLParsingDataTest, UpdateComplexWithLinesAndTabs)
{
    const char * xml = "Some\n\tindented\n long\tstring";
    XMLParsingData target(xml);
    XMLBase::SetTabSize(Tab);

    target.Update(xml, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 4, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 4, target.CurrentString());
    EXPECT_EQ(0, target.Cursor().Row());
    EXPECT_EQ(4, target.Cursor().Col());

    target.Update(xml + 5, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 5, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 6, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 6, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 13, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 13, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(7, target.Cursor().Col());

    target.Update(xml + 14, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 14, target.CurrentString());
    EXPECT_EQ(1, target.Cursor().Row());
    EXPECT_EQ(8, target.Cursor().Col());

    target.Update(xml + 15, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 15, target.CurrentString());
    EXPECT_EQ(2, target.Cursor().Row());
    EXPECT_EQ(0, target.Cursor().Col());

    target.Update(xml + 16, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 16, target.CurrentString());
    EXPECT_EQ(2, target.Cursor().Row());
    EXPECT_EQ(1, target.Cursor().Col());

    target.Update(xml + 20, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 20, target.CurrentString());
    EXPECT_EQ(2, target.Cursor().Row());
    EXPECT_EQ(5, target.Cursor().Col());

    target.Update(xml + 21, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 21, target.CurrentString());
    EXPECT_EQ(2, target.Cursor().Row());
    EXPECT_EQ(8, target.Cursor().Col());

    target.Update(xml + 27, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 27, target.CurrentString());
    EXPECT_EQ(2, target.Cursor().Row());
    EXPECT_EQ(14, target.Cursor().Col());

    target.Update(xml + 28, XMLEncoding::UTF8);

    EXPECT_EQ(xml, target.InitialString());
    EXPECT_EQ(xml + 27, target.CurrentString());
    EXPECT_EQ(2, target.Cursor().Row());
    EXPECT_EQ(14, target.Cursor().Col());
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
