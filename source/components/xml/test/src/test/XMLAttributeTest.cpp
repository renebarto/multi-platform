#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "xml/TestData.h"
#include "xml/XMLAttribute.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"

using namespace std;

namespace XMLParser {
namespace Test {

const string Name = "name";
const string NameUppercase = "Name";
const string NoName = "none";
const string Value = "value";
const string Value2 = "value2";
const string NoValue = "none";
const int TabSize = 4;
const int Row = 2;
const int Col = 3;

class XMLAttributeTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLAttributeTest::SetUp()
{
    XMLBase::SetTabSize(TabSize);
}

void XMLAttributeTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLAttributeTest, Construct)
{
    XMLAttribute target;

    EXPECT_EQ("", target.Name());
    EXPECT_EQ("", target.Value());
	EXPECT_NULL(target.GetUserData());
	EXPECT_EQ(0, target.Row());
	EXPECT_EQ(0, target.Column());
    EXPECT_NULL(target.OwningDocument());
}

TEST_FIXTURE(XMLAttributeTest, ConstructName)
{
    XMLAttribute target(Name);

    EXPECT_EQ(Name, target.Name());
    EXPECT_EQ("", target.Value());
}

TEST_FIXTURE(XMLAttributeTest, ConstructNameValue)
{
    XMLAttribute target(Name, Value);

    EXPECT_EQ(Name, target.Name());
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLAttributeTest, ConstructCopy)
{
    XMLAttribute attribute(Name, Value);
    XMLAttribute target(attribute);

    EXPECT_EQ(Name, target.Name());
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLAttributeTest, Assign)
{
    XMLAttribute attribute(Name, Value);
    XMLAttribute target;

    target = attribute;

    EXPECT_EQ(Name, target.Name());
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLAttributeTest, Match)
{
    XMLAttribute target(Name, Value);

    EXPECT_TRUE(target.Match(Name));
    EXPECT_TRUE(target.Match(Name, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(NameUppercase));
    EXPECT_TRUE(target.Match(NameUppercase, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(Value));
    EXPECT_FALSE(target.Match(Value, XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLAttributeTest, ValidIntValue)
{
    XMLAttribute target(Name, "1");
    EXPECT_EQ(1, target.Value<int>());
    EXPECT_EQ(1.00, target.Value<double>());
}

TEST_FIXTURE(XMLAttributeTest, InvalidIntValue)
{
    XMLAttribute target(Name, "abc");
    EXPECT_THROW(target.Value<int>(), XMLValueParseException);
}

TEST_FIXTURE(XMLAttributeTest, ValidDoubleValue)
{
    XMLAttribute target(Name, "1.23");
    EXPECT_EQ(1.23, target.Value<double>());
    EXPECT_THROW(target.Value<int>(), XMLValueParseException);
}

TEST_FIXTURE(XMLAttributeTest, GetSetValue)
{
    XMLAttribute target(Name, Value);

    EXPECT_EQ(Value, target.Value());
    target.SetValue(Value2);
    EXPECT_EQ(Value2, target.Value());
}

TEST_FIXTURE(XMLAttributeTest, ParseString)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = Name + "=\"" + Value + "\"";
    target.ParseString(xmlString);
    EXPECT_EQ(Name, target.Name());
    EXPECT_EQ(Value, target.Value());
}

TEST_FIXTURE(XMLAttributeTest, ParseStringEmpty)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = "";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(NoName, target.Name());
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(0, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(0, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLAttributeTest, ParseStringInvalidName)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = ";name=\"value\"";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("", target.Name());
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(XMLError::ErrorReadingAttributes, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(0, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(0, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLAttributeTest, ParseStringNoValue)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = "name";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("name", target.Name());
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(XMLError::ErrorReadingAttributes, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(5, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLAttributeTest, ParseStringNoValue2)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = "name=";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("name", target.Name());
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(XMLError::ErrorReadingAttributes, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(6, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLAttributeTest, ParseStringUnquotedValue)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = "name=value";
    target.ParseString(xmlString);
    EXPECT_EQ("name", target.Name());
    EXPECT_EQ("value", target.Value());
}

TEST_FIXTURE(XMLAttributeTest, ParseStringWithSpacesValue)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = "name =\t\rvalue";
    target.ParseString(xmlString);
    EXPECT_EQ("name", target.Name());
    EXPECT_EQ("value", target.Value());
}

TEST_FIXTURE(XMLAttributeTest, ParseStringIncorrectlyQuotedValue)
{
    XMLAttribute target(NoName, NoValue);
    std::string xmlString = "name=value\"";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("name", target.Name());
        EXPECT_EQ("", target.Value());
        EXPECT_EQ(XMLError::ErrorReadingAttributes, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(11, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLAttributeTest, GenerateString)
{
    XMLAttribute target(Name, Value);

    std::string expected = Name + "=\"" + Value + "\"";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser

