#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "xml/TestData.h"
#include "xml/XMLComment.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLTestVisitor.h"

using namespace std;

namespace XMLParser {
namespace Test {

const string Value = "value";
const string Value2 = "Value";
const string NoValue = "none";
const int TabSize = 4;
//const int Row = 2;
//const int Col = 3;

class XMLCommentTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLCommentTest::SetUp()
{
    XMLBase::SetTabSize(TabSize);
}

void XMLCommentTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLCommentTest, Construct)
{
    XMLComment target;

    EXPECT_EQ("", target.Value());
}

TEST_FIXTURE(XMLCommentTest, ConstructValue)
{
    XMLComment target(Value);

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLCommentTest, ConstructCopy)
{
    XMLComment other(Value);
    XMLComment target(other);

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLCommentTest, Assign)
{
    XMLComment other(Value);
    XMLComment target;

    target = other;

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLCommentTest, Match)
{
    XMLComment target(Value);

    EXPECT_TRUE(target.Match(Value));
    EXPECT_TRUE(target.Match(Value, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(Value2));
    EXPECT_TRUE(target.Match(Value2, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(NoValue));
    EXPECT_FALSE(target.Match(NoValue, XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLCommentTest, ParseString)
{
    XMLComment target(Value);
    std::string xmlString = "<!--" + Value + "-->";
    target.ParseString(xmlString);
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLCommentTest, ParseStringEmpty)
{
    XMLComment target(NoValue);
    std::string xmlString = "";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLCommentTest, ParseStringNoEndTag)
{
    XMLComment target(NoValue);
    std::string xmlString = "<!--" + Value;
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("", target.Value());
        EXPECT_EQ(XMLError::ErrorParsingComment, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(10, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLCommentTest, ParseStringWithSpecialChars)
{
    XMLComment target(NoValue);
    std::string xmlString = "<!--" + Value + " && &amp; \x01 -->";
    target.ParseString(xmlString);
    EXPECT_EQ(Value + " && &amp; \x01 ", target.Value());
}

TEST_FIXTURE(XMLCommentTest, ParseStringWithSpacesValue)
{
    XMLComment target(NoValue);
    std::string xmlString = "<!-- " + Value + " \t-->";
    target.ParseString(xmlString);
    EXPECT_EQ(" " + Value + " \t", target.Value());
}

TEST_FIXTURE(XMLCommentTest, GenerateString)
{
    XMLComment target(Value);

    std::string expected = "<!--" + Value + "-->";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);

    expected = "    <!--" + Value + "-->";
    actual = target.GenerateString(1);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLCommentTest, Accept)
{
    XMLTestVisitor visitor;
    XMLComment target(Value);

    EXPECT_FALSE(visitor.visitedByDocumentEnter);
    EXPECT_FALSE(visitor.visitedByDocumentExit);
    EXPECT_EQ(0, visitor.visitedByElementEnter);
    EXPECT_EQ(0, visitor.visitedByElementExit);
    EXPECT_FALSE(visitor.visitedByComment);
    EXPECT_FALSE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);

    EXPECT_TRUE(target.Accept(&visitor));

    EXPECT_FALSE(visitor.visitedByDocumentEnter);
    EXPECT_FALSE(visitor.visitedByDocumentExit);
    EXPECT_EQ(0, visitor.visitedByElementEnter);
    EXPECT_EQ(0, visitor.visitedByElementExit);
    EXPECT_TRUE(visitor.visitedByComment);
    EXPECT_FALSE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
