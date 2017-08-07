#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "xml/TestData.h"
#include "xml/XMLComment.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLDocument.h"
#include "xml/XMLElement.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLTestVisitor.h"
#include "xml/XMLText.h"
#include "xml/XMLUnknown.h"

using namespace std;

namespace XMLParser {
namespace Test {

const int Tab = 4;
const string Document = "doc";
const string Tag = "tag";
const string Name = "name";
const string Value = "value";
const string Value2 = "Value";
const string NoValue = "none";
const string ValueChild = "valuechild";
const string ValueChild2 = "ValueChild";
const string AttributeName = "attrname";
const string AttributeName2 = "AttrName";
const string AttributeValue = "attrvalue";
const string AttributeValue2 = "attrvalue2";

class XMLElementTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLElementTest::SetUp()
{
    XMLBase::SetTabSize(Tab);
}

void XMLElementTest::TearDown()
{
}

TEST_FIXTURE(XMLElementTest, ConstructDefault)
{
    XMLElement target;
    EXPECT_EQ("", target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(0), target.Children().size());
    EXPECT_FALSE(target.HasChildren());
}

TEST_FIXTURE(XMLElementTest, ConstructValue)
{
    XMLElement target(Value);

    EXPECT_EQ(Value, target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(0), target.Children().size());
}

TEST_FIXTURE(XMLElementTest, ConstructCopy)
{
    XMLElement other(Value);
    other.AddAttribute(AttributeName, AttributeValue);
    XMLElement::Ptr child = make_shared<XMLElement>(ValueChild);
    other.AddChild(child);

    XMLElement target(other);

    EXPECT_EQ(Value, target.Value());
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_EQ(ValueChild, target.Children()[0]->Value());
    EXPECT_TRUE(target.HasChildren());
}

TEST_FIXTURE(XMLElementTest, Assign)
{
    XMLElement other(Value);
    other.AddAttribute(AttributeName, AttributeValue);
    XMLElement::Ptr child = make_shared<XMLElement>(ValueChild);
    other.AddChild(child);

    XMLElement target;
    target = other;

    EXPECT_EQ(Value, target.Value());
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_EQ(ValueChild, target.Children()[0]->Value());
    EXPECT_TRUE(target.HasChildren());
}

TEST_FIXTURE(XMLElementTest, AddOrModifyAttribute)
{
    XMLElement target(Value);

    EXPECT_EQ(size_t(0), target.Attributes().size());

    target.AddOrModifyAttribute(AttributeName, AttributeValue);
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());

    target.AddOrModifyAttribute(AttributeName, AttributeValue2);
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue2, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());

    target.AddOrModifyAttribute(AttributeName2, AttributeValue, XMLIgnoreCase::IgnoreCase);
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());

    target.AddOrModifyAttribute(AttributeName2, AttributeValue2, XMLIgnoreCase::MatchCase);
    EXPECT_EQ(size_t(2), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());
    EXPECT_EQ(AttributeValue2, target.Attributes()[1]->Value());
    EXPECT_EQ(AttributeName2, target.Attributes()[1]->Name());
}

TEST_FIXTURE(XMLElementTest, AddAttribute)
{
    XMLElement target(Value);

    EXPECT_EQ(size_t(0), target.Attributes().size());

    target.AddAttribute(AttributeName, AttributeValue);
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());

    EXPECT_THROW(target.AddAttribute(AttributeName, AttributeValue2), XMLException);

    EXPECT_THROW(target.AddAttribute(AttributeName2, AttributeValue2, XMLIgnoreCase::IgnoreCase), XMLException);

    target.AddAttribute(AttributeName2, AttributeValue2, XMLIgnoreCase::MatchCase);
    EXPECT_EQ(size_t(2), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());
    EXPECT_EQ(AttributeValue2, target.Attributes()[1]->Value());
    EXPECT_EQ(AttributeName2, target.Attributes()[1]->Name());
}

TEST_FIXTURE(XMLElementTest, ModifyAttribute)
{
    XMLElement target(Value);

    EXPECT_EQ(size_t(0), target.Attributes().size());

    EXPECT_THROW(target.ModifyAttribute(AttributeName, AttributeValue2), XMLException);
    EXPECT_THROW(target.ModifyAttribute(AttributeName2, AttributeValue2), XMLException);
    target.AddAttribute(AttributeName, AttributeValue);

    target.ModifyAttribute(AttributeName, AttributeValue2);
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue2, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());

    target.ModifyAttribute(AttributeName2, AttributeValue, XMLIgnoreCase::IgnoreCase);
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());

    EXPECT_THROW(target.ModifyAttribute(AttributeName2, AttributeValue2, XMLIgnoreCase::MatchCase), XMLException);

    target.AddAttribute(AttributeName2, AttributeValue2);

    target.ModifyAttribute(AttributeName2, AttributeValue, XMLIgnoreCase::IgnoreCase);
    EXPECT_EQ(size_t(2), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());
    EXPECT_EQ(AttributeValue2, target.Attributes()[1]->Value());
    EXPECT_EQ(AttributeName2, target.Attributes()[1]->Name());

    target.ModifyAttribute(AttributeName2, AttributeValue, XMLIgnoreCase::MatchCase);
    EXPECT_EQ(size_t(2), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.Attributes()[0]->Value());
    EXPECT_EQ(AttributeName, target.Attributes()[0]->Name());
    EXPECT_EQ(AttributeValue, target.Attributes()[1]->Value());
    EXPECT_EQ(AttributeName2, target.Attributes()[1]->Name());
}

TEST_FIXTURE(XMLElementTest, HaveAttribute)
{
    XMLElement target(Value);

    EXPECT_FALSE(target.HaveAttribute(AttributeName));
    EXPECT_FALSE(target.HaveAttribute(AttributeName2));

    target.AddAttribute(AttributeName, AttributeValue);

    EXPECT_TRUE(target.HaveAttribute(AttributeName));
    EXPECT_FALSE(target.HaveAttribute(AttributeName2));

    EXPECT_TRUE(target.HaveAttribute(AttributeName, XMLIgnoreCase::IgnoreCase));
    EXPECT_TRUE(target.HaveAttribute(AttributeName2, XMLIgnoreCase::IgnoreCase));

    target.AddAttribute(AttributeName2, AttributeValue2);

    EXPECT_TRUE(target.HaveAttribute(AttributeName, XMLIgnoreCase::MatchCase));
    EXPECT_TRUE(target.HaveAttribute(AttributeName2, XMLIgnoreCase::MatchCase));
}

TEST_FIXTURE(XMLElementTest, CheckAttribute)
{
    XMLElement target(Value);

    std::string value;
    EXPECT_FALSE(target.CheckAttribute(AttributeName, value));
    EXPECT_FALSE(target.CheckAttribute(AttributeName2, value));

    target.AddAttribute(AttributeName, AttributeValue);

    EXPECT_TRUE(target.CheckAttribute(AttributeName, value));
    EXPECT_EQ(AttributeValue, value);

    value = "";
    EXPECT_FALSE(target.CheckAttribute(AttributeName2, value));
    EXPECT_EQ("", value);

    value = "";
    EXPECT_TRUE(target.CheckAttribute(AttributeName, value, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(AttributeValue, value);

    value = "";
    EXPECT_TRUE(target.CheckAttribute(AttributeName2, value, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(AttributeValue, value);

    int intValue;
    EXPECT_FALSE(target.CheckAttribute(AttributeName2, intValue, XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLElementTest, ExpectAttribute)
{
    XMLElement target(Value);

    std::string value;
    EXPECT_THROW(target.ExpectAttribute<std::string>(AttributeName), XMLException);
    EXPECT_THROW(target.ExpectAttribute<std::string>(AttributeName2), XMLException);

    target.AddAttribute(AttributeName, AttributeValue);

    value = target.ExpectAttribute<std::string>(AttributeName);
    EXPECT_EQ(AttributeValue, value);

    EXPECT_THROW(target.ExpectAttribute<std::string>(AttributeName2), XMLException);

    value = target.ExpectAttribute<std::string>(AttributeName, XMLIgnoreCase::IgnoreCase);
    EXPECT_EQ(AttributeValue, value);

    value = target.ExpectAttribute<std::string>(AttributeName2, XMLIgnoreCase::IgnoreCase);
    EXPECT_EQ(AttributeValue, value);

    EXPECT_THROW(target.ExpectAttribute<int>(AttributeName2, XMLIgnoreCase::IgnoreCase), XMLValueParseException);
}

TEST_FIXTURE(XMLElementTest, FindAttribute)
{
    XMLElement target(Value);

    EXPECT_EQ(size_t(0), target.Attributes().size());

    EXPECT_TRUE(target.FindAttribute(AttributeName) == nullptr);
    EXPECT_TRUE(target.FindAttribute(AttributeName2) == nullptr);
    target.AddAttribute(AttributeName, AttributeValue);

    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.FindAttribute(AttributeName)->Value());
    EXPECT_TRUE(target.FindAttribute(AttributeName2) == nullptr);

    EXPECT_EQ(AttributeValue, target.FindAttribute(AttributeName, XMLIgnoreCase::IgnoreCase)->Value());
    EXPECT_EQ(AttributeValue, target.FindAttribute(AttributeName2, XMLIgnoreCase::IgnoreCase)->Value());

    target.AddAttribute(AttributeName2, AttributeValue2);

    EXPECT_EQ(size_t(2), target.Attributes().size());
    EXPECT_EQ(AttributeValue, target.FindAttribute(AttributeName, XMLIgnoreCase::MatchCase)->Value());
    EXPECT_EQ(AttributeValue2, target.FindAttribute(AttributeName2, XMLIgnoreCase::MatchCase)->Value());
}

TEST_FIXTURE(XMLElementTest, ParseStringEmpty)
{
    XMLElement target(NoValue);
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
        EXPECT_EQ(size_t(0), target.Attributes().size());
        EXPECT_EQ(size_t(0), target.Children().size());
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLElementTest, ParseStringInvalidTag)
{
    XMLElement target(NoValue);
    std::string xmlString = "tag/>";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(NoValue, target.Value());
        EXPECT_EQ(size_t(0), target.Attributes().size());
        EXPECT_EQ(size_t(0), target.Children().size());
        EXPECT_EQ(XMLError::ErrorParsingElement, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLElementTest, ParseStringInvalidTag2)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag/";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(Tag, target.Value());
        EXPECT_EQ(size_t(0), target.Attributes().size());
        EXPECT_EQ(size_t(0), target.Children().size());
        EXPECT_EQ(XMLError::ErrorParsingElement, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(5, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLElementTest, ParseStringNoEndTag)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag>abc";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(Tag, target.Value());
        EXPECT_EQ(size_t(0), target.Attributes().size());
        EXPECT_EQ(size_t(1), target.Children().size());
        EXPECT_TRUE(dynamic_pointer_cast<XMLText>(target.Children()[0]) != nullptr);
        EXPECT_EQ("abc", target.Children()[0]->Value());
        EXPECT_EQ(XMLError::ErrorReadingEndTag, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(6, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLElementTest, ParseStringEmptyClosedTag)
{
    XMLElement target(NoValue);
    std::string xmlString = "< />";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("", target.Value());
        EXPECT_EQ(size_t(0), target.Attributes().size());
        EXPECT_EQ(size_t(0), target.Children().size());
        EXPECT_EQ(XMLError::ErrorReadingElementValue, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(2, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLElementTest, ParseStringClosedTag)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag />";
    target.ParseString(xmlString);
    EXPECT_EQ(Tag, target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(0), target.Children().size());
}

TEST_FIXTURE(XMLElementTest, ParseStringWithAttribute)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag attrname=\"attrvalue\" />";
    target.ParseString(xmlString);
    EXPECT_EQ(Tag, target.Value());
    EXPECT_EQ(size_t(1), target.Attributes().size());
    EXPECT_EQ(size_t(0), target.Children().size());
    EXPECT_EQ("attrname", target.Attributes()[0]->Name());
    EXPECT_EQ("attrvalue", target.Attributes()[0]->Value());
}

TEST_FIXTURE(XMLElementTest, ParseStringDuplicateAttribute)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag attrname=\"attrvalue\" attrname=\"\" />";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(Tag, target.Value());
        EXPECT_EQ(size_t(1), target.Attributes().size());
        EXPECT_EQ(size_t(0), target.Children().size());
        EXPECT_EQ("attrname", target.Attributes()[0]->Name());
        EXPECT_EQ("attrvalue", target.Attributes()[0]->Value());
        EXPECT_EQ(XMLError::ErrorDuplicateAttribute, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(27, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLElementTest, ParseStringWithDeclaration)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag>\n"
                              "<?xml version=\"1.0\" standalone=\"no\" ?>\n"
                              "</tag>";
    target.ParseString(xmlString);
    EXPECT_EQ(Tag, target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_TRUE(dynamic_pointer_cast<XMLDeclaration>(target.Children()[0]) != nullptr);
    EXPECT_EQ("1.0", dynamic_pointer_cast<XMLDeclaration>(target.Children()[0])->Version());
    EXPECT_EQ("", dynamic_pointer_cast<XMLDeclaration>(target.Children()[0])->Encoding());
    EXPECT_EQ("no", dynamic_pointer_cast<XMLDeclaration>(target.Children()[0])->Standalone());
}

TEST_FIXTURE(XMLElementTest, ParseStringWithComment)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag>\n"
                              "    <!-- Some comment -->\n"
                              "</tag>";
    target.ParseString(xmlString);
    EXPECT_EQ(Tag, target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_TRUE(dynamic_pointer_cast<XMLComment>(target.Children()[0]) != nullptr);
    EXPECT_EQ(" Some comment ", target.Children()[0]->Value());
}

TEST_FIXTURE(XMLElementTest, ParseStringWithText)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag>\n"
                              "Some Text\n"
                              "</tag>";
    target.ParseString(xmlString);
    EXPECT_EQ(Tag, target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_TRUE(dynamic_pointer_cast<XMLText>(target.Children()[0]) != nullptr);
    EXPECT_EQ("\nSome Text\n", target.Children()[0]->Value());
}

TEST_FIXTURE(XMLElementTest, ParseStringWithUnknown)
{
    XMLElement target(NoValue);
    std::string xmlString = "<tag>\n"
                              "<$Some Text>\n"
                              "</tag>";
    target.ParseString(xmlString);
    EXPECT_EQ(Tag, target.Value());
    EXPECT_EQ(size_t(0), target.Attributes().size());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_TRUE(dynamic_pointer_cast<XMLUnknown>(target.Children()[0]) != nullptr);
    EXPECT_EQ("$Some Text", target.Children()[0]->Value());
}

TEST_FIXTURE(XMLElementTest, GenerateStringEmptyClosedTag)
{
    XMLElement target;

    std::string expected = "< />";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLElementTest, GenerateStringSingleAttribute)
{
    XMLElement target(Tag);
    target.AddAttribute(AttributeName, AttributeValue);

    std::string expected = "<tag " + AttributeName + "=\"" + AttributeValue + "\" />";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLElementTest, GenerateStringRootOnly)
{
    XMLElement target(Tag);

    std::string expected = "<" + Tag + " />";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLElementTest, GenerateStringVerySimple)
{
    XMLElement target(Tag);

    XMLElement::Ptr element1 = make_shared<XMLElement>("element");
    element1->AddAttribute("name", "element1");
    target.AddChild(element1);
    XMLElement::Ptr element2 = make_shared<XMLElement>("element");
    element2->AddAttribute("name", "element2");
    target.AddChild(element2);

    std::string expected = "<tag>\n"
                             "    <element name=\"element1\" />\n"
                             "    <element name=\"element2\" />\n"
                             "</tag>";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLElementTest, GenerateStringSimple)
{
    XMLElement target(Tag);

    XMLElement::Ptr element1 = make_shared<XMLElement>("element");
    element1->AddAttribute("name", "element1");
    XMLElement::Ptr subElement11 = make_shared<XMLElement>("subelement");
    subElement11->AddAttribute("name", "sub1");
    element1->AddChild(subElement11);
    XMLElement::Ptr subElement12 = make_shared<XMLElement>("subelement");
    subElement12->AddAttribute("name", "sub2");
    element1->AddChild(subElement12);
    target.AddChild(element1);
    XMLElement::Ptr element2 = make_shared<XMLElement>("element");
    element2->AddAttribute("name", "element2");
    XMLElement::Ptr subElement21 = make_shared<XMLElement>("subelement");
    subElement21->AddAttribute("name", "sub1");
    element2->AddChild(subElement21);
    XMLElement::Ptr subElement22 = make_shared<XMLElement>("subelement");
    subElement22->AddAttribute("name", "sub2");
    element2->AddChild(subElement22);
    XMLElement::Ptr subElement23 = make_shared<XMLElement>("subelement");
    subElement23->AddAttribute("name", "sub3");
    element2->AddChild(subElement23);
    target.AddChild(element2);

    std::string expected = "<tag>\n"
                             "    <element name=\"element1\">\n"
                             "        <subelement name=\"sub1\" />\n"
                             "        <subelement name=\"sub2\" />\n"
                             "    </element>\n"
                             "    <element name=\"element2\">\n"
                             "        <subelement name=\"sub1\" />\n"
                             "        <subelement name=\"sub2\" />\n"
                             "        <subelement name=\"sub3\" />\n"
                             "    </element>\n"
                             "</tag>";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLElementTest, Accept)
{
    XMLTestVisitor visitor;
    XMLElement target;

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
    EXPECT_EQ(1, visitor.visitedByElementEnter);
    EXPECT_EQ(1, visitor.visitedByElementExit);
    EXPECT_FALSE(visitor.visitedByComment);
    EXPECT_FALSE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);
}

TEST_FIXTURE(XMLElementTest, AcceptWithChildrenNoElements)
{
    XMLTestVisitor visitor;
    XMLElement target;
    XMLNode::Ptr child = make_shared<XMLComment>();
    target.AddChild(child);

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
    EXPECT_EQ(1, visitor.visitedByElementEnter);
    EXPECT_EQ(1, visitor.visitedByElementExit);
    EXPECT_TRUE(visitor.visitedByComment);
    EXPECT_FALSE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);
}

TEST_FIXTURE(XMLElementTest, AcceptWithElements)
{
    XMLTestVisitor visitor;
    XMLElement target;
    XMLNode::Ptr child = make_shared<XMLElement>();
    target.AddChild(child);

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
    EXPECT_EQ(2, visitor.visitedByElementEnter);
    EXPECT_EQ(2, visitor.visitedByElementExit);
    EXPECT_FALSE(visitor.visitedByComment);
    EXPECT_FALSE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);
}

} // namespace Test
} // namespace XMLParser
