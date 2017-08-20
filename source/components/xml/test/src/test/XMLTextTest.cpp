#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "xml/TestData.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLTestVisitor.h"
#include "xml/XMLText.h"

using namespace std;

namespace XMLParser {
namespace Test {

const string Value = "value";
const string Value2 = "Value";
const string NoValue = "none";
const int TabSize = 4;

class XMLTextTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLTextTest::SetUp()
{
    XMLBase::SetTabSize(TabSize);
}

void XMLTextTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLTextTest, Construct)
{
    XMLText target;

    EXPECT_EQ("", target.Value());
    EXPECT_FALSE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, ConstructValue)
{
    XMLText target(Value);

    EXPECT_EQ(Value, target.Value());
    EXPECT_FALSE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, ConstructValueCDATA)
{
    XMLText target(Value, true);

    EXPECT_EQ(Value, target.Value());
    EXPECT_TRUE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, ConstructElement)
{
    XMLElement::Ptr element = make_shared<XMLElement>(Value);
    XMLText target(element);

    EXPECT_EQ(Value, target.Value());
    EXPECT_FALSE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, ConstructCopy)
{
    XMLText other(Value, true);
    XMLText target(other);

    EXPECT_EQ(Value, target.Value());
    EXPECT_TRUE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, Assign)
{
    XMLText other(Value, true);
    XMLText target;

    target = other;

    EXPECT_EQ(Value, target.Value());
    EXPECT_TRUE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, GetSetCDATA)
{
    XMLText target;

    EXPECT_FALSE(target.CDATA());

    target.SetCDATA(true);

    EXPECT_TRUE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, Match)
{
    XMLText target(Value);

    EXPECT_TRUE(target.Match(Value));
    EXPECT_TRUE(target.Match(Value, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(Value2));
    EXPECT_TRUE(target.Match(Value2, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(NoValue));
    EXPECT_FALSE(target.Match(NoValue, XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLTextTest, ParseString)
{
    XMLText target(Value);
    std::string xmlString = Value;
    target.ParseString(xmlString);
    EXPECT_EQ(Value, target.Value());
    EXPECT_FALSE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, ParseStringCDATA)
{
    XMLText target(NoValue);
    std::string xmlString = "<![CDATA[" + Value + "]]>";
    target.ParseString(xmlString);
    EXPECT_TRUE(target.CDATA());
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLTextTest, ParseStringEmpty)
{
    XMLText target(NoValue);
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
        EXPECT_FALSE(target.CDATA());
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLTextTest, ParseStringInvalidCDATA)
{
    XMLText target(NoValue);
    std::string xmlString = "<![CDATA[" + Value;
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("", target.Value());
        EXPECT_FALSE(target.CDATA());
        EXPECT_EQ(XMLError::ErrorParsingCDATA, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(15, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLTextTest, ParseStringTextEndedByTagEmpty)
{
    XMLText target(NoValue);
    std::string xmlString = "<";
    target.ParseString(xmlString);
    EXPECT_FALSE(target.CDATA());
    EXPECT_EQ("", target.Value());
}

TEST_FIXTURE(XMLTextTest, ParseStringTextEndedByTag)
{
    XMLText target(NoValue);
    std::string xmlString = Value + "<";
    target.ParseString(xmlString);
    EXPECT_FALSE(target.CDATA());
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLTextTest, ParseStringWithSpacesValue)
{
    XMLText target(NoValue);
    std::string xmlString = " " + Value + " \t";
    target.ParseString(xmlString);
    EXPECT_EQ(" " + Value + " \t", target.Value());
    EXPECT_FALSE(target.CDATA());
}

TEST_FIXTURE(XMLTextTest, GenerateString)
{
    XMLText target(Value);

    std::string expected = Value;
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);

    expected = Value;
    actual = target.GenerateString(1);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLTextTest, GenerateStringCDATA)
{
    XMLText target(Value, true);

    std::string expected = "\n<![CDATA[" + Value + "]]>\n";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);

    expected = "\n" + std::string(XMLBase::TabSize(), ' ') + "<![CDATA[" + Value + "]]>\n";
    actual = target.GenerateString(1);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLTextTest, Accept)
{
    XMLTestVisitor visitor;
    XMLText target(Value);

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
    EXPECT_TRUE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
