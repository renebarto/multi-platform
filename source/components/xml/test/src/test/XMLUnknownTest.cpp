#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "xml/TestData.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLTestVisitor.h"
#include "xml/XMLUnknown.h"

using namespace std;

namespace XMLParser {
namespace Test {

const string Value = "value";
const string Value2 = "Value";
const string NoValue = "none";
const int TabSize = 4;

class XMLUnknownTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLUnknownTest::SetUp()
{
    XMLBase::SetTabSize(TabSize);
}

void XMLUnknownTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLUnknownTest, Construct)
{
    XMLUnknown target;

    EXPECT_EQ("", target.Value());
}

TEST_FIXTURE(XMLUnknownTest, ConstructValue)
{
    XMLUnknown target(Value);

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLUnknownTest, ConstructElement)
{
    XMLElement::Ptr element = make_shared<XMLElement>(Value);
    XMLUnknown target(element);

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLUnknownTest, ConstructCopy)
{
    XMLUnknown other(Value);
    XMLUnknown target(other);

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLUnknownTest, Assign)
{
    XMLUnknown other(Value);
    XMLUnknown target;

    target = other;

    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLUnknownTest, Match)
{
    XMLUnknown target(Value);

    EXPECT_TRUE(target.Match(Value));
    EXPECT_TRUE(target.Match(Value, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(Value2));
    EXPECT_TRUE(target.Match(Value2, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(NoValue));
    EXPECT_FALSE(target.Match(NoValue, XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLUnknownTest, ParseString)
{
    XMLUnknown target(Value);
    std::string xmlString = "<" + Value + ">";
    target.ParseString(xmlString);
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLUnknownTest, ParseStringEmpty)
{
    XMLUnknown target(NoValue);
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

TEST_FIXTURE(XMLUnknownTest, ParseStringInvalidTag)
{
    XMLUnknown target(NoValue);
    std::string xmlString = Value + ">";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(XMLError::ErrorParsingUnknown, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLUnknownTest, ParseStringNoTag)
{
    XMLUnknown target(NoValue);
    std::string xmlString = "<>";
    target.ParseString(xmlString);
    EXPECT_EQ("", target.Value());
}

TEST_FIXTURE(XMLUnknownTest, ParseStringInvalidTagEnd)
{
    XMLUnknown target(NoValue);
    std::string xmlString = "<" + Value + "/>";
    target.ParseString(xmlString);
    EXPECT_EQ(Value + "/", target.Value());
}

TEST_FIXTURE(XMLUnknownTest, ParseStringWithSpacesValue)
{
    XMLUnknown target(NoValue);
    std::string xmlString = " <" + Value + ">";
    target.ParseString(xmlString);
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLUnknownTest, GenerateString)
{
    XMLUnknown target(Value);

    std::string expected = "<" + Value + ">";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);

    expected = std::string(XMLBase::TabSize(), ' ') + "<" + Value + ">";
    actual = target.GenerateString(1);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLUnknownTest, Accept)
{
    XMLTestVisitor visitor;
    XMLUnknown target(Value);

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
    EXPECT_FALSE(visitor.visitedByComment);
    EXPECT_FALSE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_TRUE(visitor.visitedByUnknown);
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
