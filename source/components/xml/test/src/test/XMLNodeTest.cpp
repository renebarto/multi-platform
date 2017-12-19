#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "xml/TestData.h"
#include "xml/XMLComment.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLDocument.h"
#include "xml/XMLNode.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLTestVisitor.h"
#include "xml/XMLText.h"
#include "xml/XMLUnknown.h"

using namespace std;

namespace XMLParser
{
namespace Test
{

const int Tab = 4;
const string Document = "doc";
const string Tag = "tag";
const string Name = "name";
const string Value = "value";
const string Value2 = "Value";
const string NoValue = "none";
const string ValueChild = "valuechild";
const string ValueChild2 = "ValueChild";
const string ValueChildOther = "valuechild2";
const string ValueChildOther2 = "ValueChild2";
const string AttributeName = "attrname";
const string AttributeName2 = "AttrName";
const string AttributeValue = "attrvalue";
const string AttributeValue2 = "attrvalue2";

class XMLNodeImpl
    : public XMLNode
{
public:
    XMLNodeImpl()
        : XMLNode()
    {
    }

    explicit XMLNodeImpl(const std::string & value)
        : XMLNode(value)
    {
    }

    XMLNodeImpl(const XMLNodeImpl & other)
        : XMLNode(other)
    {
    }

    XMLNodeImpl & operator=(const XMLNodeImpl & other)
    {
        XMLNode::operator=(other);
        return *this;
    }

    virtual void ParseString(const std::string & /*xml*/,
                             XMLEncoding /*encoding*/)
    {
    }

    virtual std::string GenerateString(int /*depth*/) const
    {
        return "";
    }

    virtual const char * Parse(const char * /*xml*/, XMLEncoding /*encoding*/)
    {
        return nullptr;
    }

    virtual XMLNode::Ptr Clone() const
    {
        return XMLNode::Ptr();
    }

    virtual bool Accept(XMLVisitor * /*content*/) const
    {
        return false;
    }
};

class XMLNodeTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLNodeTest::SetUp()
{
    XMLBase::SetTabSize(Tab);
}

void XMLNodeTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLNodeTest, ConstructDefault)
{
    XMLNodeImpl target;
    EXPECT_EQ("", target.Value());
    EXPECT_EQ(size_t(0), target.Children().size());
    EXPECT_EQ(size_t(0), target.Elements().size());
    EXPECT_FALSE(target.HasChildren());
}

TEST_FIXTURE(XMLNodeTest, ConstructValue)
{
    XMLNodeImpl target(Value);

    EXPECT_EQ(Value, target.Value());
    EXPECT_EQ(size_t(0), target.Children().size());
}

TEST_FIXTURE(XMLNodeTest, ConstructCopy)
{
    XMLNodeImpl other(Value);
    XMLNode::Ptr child = make_shared<XMLElement>(ValueChild);
    other.AddChild(child);

    XMLNodeImpl target(other);

    EXPECT_EQ(Value, target.Value());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_EQ(size_t(1), target.Elements().size());
    EXPECT_EQ(ValueChild, target.Children()[0]->Value());
    EXPECT_TRUE(target.HasChildren());
}

TEST_FIXTURE(XMLNodeTest, Assign)
{
    XMLNodeImpl other(Value);
    XMLNode::Ptr child = make_shared<XMLElement>(ValueChild);
    other.AddChild(child);

    XMLNodeImpl target;
    target = other;

    EXPECT_EQ(Value, target.Value());
    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_EQ(size_t(1), target.Elements().size());
    EXPECT_EQ(ValueChild, target.Children()[0]->Value());
    EXPECT_TRUE(target.HasChildren());
}

TEST_FIXTURE(XMLNodeTest, Match)
{
    XMLNodeImpl target(Value);

    EXPECT_TRUE(target.Match(Value));
    EXPECT_TRUE(target.Match(Value, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(Value2));
    EXPECT_TRUE(target.Match(Value2, XMLIgnoreCase::IgnoreCase));
    EXPECT_FALSE(target.Match(ValueChild));
    EXPECT_FALSE(target.Match(ValueChild, XMLIgnoreCase::IgnoreCase));
}

TEST_FIXTURE(XMLNodeTest, AddChild)
{
    XMLNodeImpl target(Value);

    EXPECT_EQ(size_t(0), target.Children().size());
    EXPECT_FALSE(target.HasChildren());

    XMLComment::Ptr child = make_shared<XMLComment>(ValueChild);
    target.AddChild(child);

    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_EQ(size_t(0), target.Elements().size());
    EXPECT_EQ(child, target.Children()[0]);
    EXPECT_TRUE(target.HasChildren());

    XMLElement::Ptr childElement = make_shared<XMLElement>(ValueChild);
    target.AddChild(childElement);

    EXPECT_EQ(size_t(2), target.Children().size());
    EXPECT_EQ(size_t(1), target.Elements().size());
    EXPECT_EQ(childElement, target.Children()[1]);
    EXPECT_TRUE(target.HasChildren());
}

TEST_FIXTURE(XMLNodeTest, FindChild)
{
    XMLNodeImpl target(Value);

    EXPECT_EQ(size_t(0), target.Children().size());

    EXPECT_TRUE(target.FindChild(ValueChild) == nullptr);
    EXPECT_TRUE(target.FindChild(ValueChild, XMLIgnoreCase::IgnoreCase) == nullptr);
    EXPECT_TRUE(target.FindChild(ValueChild, XMLIgnoreCase::MatchCase) == nullptr);

    XMLNode::Ptr child = make_shared<XMLComment>(ValueChild);
    target.AddChild(child);

    EXPECT_EQ(size_t(1), target.Children().size());

    EXPECT_EQ(child, target.FindChild(ValueChild));
    EXPECT_TRUE(target.FindChild(ValueChild2) == nullptr);
    EXPECT_EQ(child, target.FindChild(ValueChild, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(child, target.FindChild(ValueChild2, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(child, target.FindChild(ValueChild, XMLIgnoreCase::MatchCase));
    EXPECT_TRUE(target.FindChild(ValueChild2, XMLIgnoreCase::MatchCase) == nullptr);

    XMLNode::Ptr childElement = make_shared<XMLElement>(ValueChildOther);
    target.AddChild(childElement);

    EXPECT_EQ(size_t(2), target.Children().size());

    EXPECT_EQ(child, target.FindChild(ValueChild));
    EXPECT_TRUE(target.FindChild(ValueChild2) == nullptr);
    EXPECT_EQ(child, target.FindChild(ValueChild, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(child, target.FindChild(ValueChild2, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(child, target.FindChild(ValueChild, XMLIgnoreCase::MatchCase));
    EXPECT_TRUE(target.FindChild(ValueChild2, XMLIgnoreCase::MatchCase) == nullptr);

    EXPECT_EQ(childElement, target.FindChild(ValueChildOther));
    EXPECT_TRUE(target.FindChild(ValueChildOther2) == nullptr);
    EXPECT_EQ(childElement, target.FindChild(ValueChildOther, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(childElement, target.FindChild(ValueChildOther2, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(childElement, target.FindChild(ValueChildOther, XMLIgnoreCase::MatchCase));
    EXPECT_TRUE(target.FindChild(ValueChildOther2, XMLIgnoreCase::MatchCase) == nullptr);
}

TEST_FIXTURE(XMLNodeTest, FindElement)
{
    XMLNodeImpl target(Value);

    EXPECT_EQ(size_t(0), target.Children().size());

    EXPECT_TRUE(target.FindElement(ValueChild) == nullptr);
    EXPECT_TRUE(target.FindElement(ValueChild, XMLIgnoreCase::IgnoreCase) == nullptr);
    EXPECT_TRUE(target.FindElement(ValueChild, XMLIgnoreCase::MatchCase) == nullptr);

    XMLNode::Ptr child = make_shared<XMLComment>(ValueChild);
    target.AddChild(child);

    EXPECT_EQ(size_t(1), target.Children().size());
    EXPECT_EQ(size_t(0), target.Elements().size());

    EXPECT_TRUE(target.FindElement(ValueChild) == nullptr);
    EXPECT_TRUE(target.FindElement(ValueChild, XMLIgnoreCase::IgnoreCase) == nullptr);
    EXPECT_TRUE(target.FindElement(ValueChild, XMLIgnoreCase::MatchCase) == nullptr);

    XMLNode::Ptr childElement = make_shared<XMLElement>(ValueChildOther);
    target.AddChild(childElement);

    EXPECT_EQ(size_t(2), target.Children().size());
    EXPECT_EQ(size_t(1), target.Elements().size());

    EXPECT_EQ(childElement, target.FindElement(ValueChildOther));
    EXPECT_TRUE(target.FindElement(ValueChildOther2) == nullptr);
    EXPECT_EQ(childElement, target.FindElement(ValueChildOther, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(childElement, target.FindElement(ValueChildOther2, XMLIgnoreCase::IgnoreCase));
    EXPECT_EQ(childElement, target.FindElement(ValueChildOther, XMLIgnoreCase::MatchCase));
    EXPECT_TRUE(target.FindElement(ValueChildOther2, XMLIgnoreCase::MatchCase) == nullptr);
}

TEST_FIXTURE(XMLNodeTest, GetText)
{
    XMLNodeImpl target(Value);

    EXPECT_EQ("", target.GetText());

    XMLText::Ptr child1 = make_shared<XMLText>(ValueChild);
    target.AddChild(child1);

    EXPECT_EQ(ValueChild, target.GetText());

    XMLText::Ptr child2 = make_shared<XMLText>(ValueChild2);
    target.AddChild(child2);

    EXPECT_EQ(ValueChild, target.GetText());
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
