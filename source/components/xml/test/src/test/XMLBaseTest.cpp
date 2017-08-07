#include "unit-test-c++/UnitTestC++.h"

#include <string>
#include "xml/TestData.h"
#include "xml/XMLBase.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"

using namespace std;

namespace XMLParser {
namespace Test {

const XMLError ErrorCode = XMLError::Error;
const string ErrorDescription = "Error";
const int Row = 2;
const int Col = 3;

class XMLBaseImplementation : public XMLBase
{
    virtual void ParseString(const std::string &,
                             XMLEncoding)
    {
    }
    virtual const char * Parse(const char *,
                               XMLEncoding)
    {
        return nullptr;
    }
    virtual std::string GenerateString(int) const { return ""; }
};

class XMLBaseTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLBaseTest::SetUp()
{
    XMLBase::SetCondenseWhiteSpace(false);
    XMLBase::SetTabSize(0);
}

void XMLBaseTest::TearDown()
{
}

TEST_FIXTURE(XMLBaseTest, Construct)
{
    XMLBaseImplementation target;

    EXPECT_NULL(target.GetUserData());
    EXPECT_EQ(0, target.Row());
    EXPECT_EQ(0, target.Column());
    EXPECT_NULL(target.OwningDocument());
    EXPECT_FALSE(target.IsWhiteSpaceCondensed());
    EXPECT_EQ(0, target.TabSize());
}

TEST_FIXTURE(XMLBaseTest, GetSetUserData)
{
    XMLBaseImplementation target;

    void * userData = target.GetUserData();
    const void * userDataConst = target.GetUserData();

    EXPECT_NULL(userData);
    EXPECT_NULL(userDataConst);

    int x = 3;
    target.SetUserData(&x);

    userData = target.GetUserData();
    userDataConst = target.GetUserData();
    EXPECT_EQ(&x, userData);
    EXPECT_EQ(&x, userDataConst);
}

//TEST_FIXTURE(XMLBaseTest, GetSetLocation)
//{
//    XMLBaseImplementation target;
//
//    EXPECT_EQ(-1, target.Row());
//    EXPECT_EQ(-1, target.Column());
//
//    XMLCursor location(2, 3);
//    target.SetLocation(location);
//
//    EXPECT_EQ(2, target.Row());
//    EXPECT_EQ(3, target.Column());
//}

TEST_FIXTURE(XMLBaseTest, GetSetCondenseWhiteSpace)
{
    XMLBaseImplementation target;

    EXPECT_FALSE(target.IsWhiteSpaceCondensed());

    target.SetCondenseWhiteSpace(true);

    EXPECT_TRUE(target.IsWhiteSpaceCondensed());
}

TEST_FIXTURE(XMLBaseTest, GetSetTabSize)
{
    XMLBaseImplementation target;

    EXPECT_EQ(0, target.TabSize());

    target.SetTabSize(4);

    EXPECT_EQ(4, target.TabSize());
}

TEST_FIXTURE(XMLBaseTest, GetSetOwningDocument)
{
    XMLDocument doc;
    XMLBaseImplementation target;

    EXPECT_NULL(target.OwningDocument());

    target.SetOwningDocument(&doc);

    EXPECT_EQ(&doc, target.OwningDocument());
}

TEST_FIXTURE(XMLBaseTest, EncodeStringNormal)
{
    std::string source = "Test";
    std::string expected = "Test";
    std::string actual;
    XMLBase::EncodeString(source, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLBaseTest, EncodeStringSpecialCharacters)
{
    std::string source = "&<>'\"";
    std::string expected = "&amp;&lt;&gt;&apos;&quot;";
    std::string actual;
    XMLBase::EncodeString(source, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLBaseTest, EncodeStringPrespecifiedHexCharacters)
{
    std::string source = "&#xA9;";
    std::string expected = "&#xA9;";
    std::string actual;
    XMLBase::EncodeString(source, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLBaseTest, EncodeStringPrespecifiedHexCharactersIncomplete)
{
    std::string source = "&#xA9";
    std::string expected = "";
    std::string actual;
    XMLBase::EncodeString(source, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLBaseTest, EncodeStringControlCharacters)
{
    std::string source = "\x01\x02\x03\x04\x05\x06\x07"
                           "\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
                           "\x10\x11\x12\x13\x14\x15\x16\x17"
                           "\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F";
    std::string expected = "&#x01;&#x02;&#x03;&#x04;&#x05;&#x06;&#x07;"
                             "&#x08;&#x09;&#x0A;&#x0B;&#x0C;&#x0D;&#x0E;&#x0F;"
                             "&#x10;&#x11;&#x12;&#x13;&#x14;&#x15;&#x16;&#x17;"
                             "&#x18;&#x19;&#x1A;&#x1B;&#x1C;&#x1D;&#x1E;&#x1F;";
    std::string actual;
    XMLBase::EncodeString(source, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLBaseTest, EncodeStringCombined)
{
    std::string source = "A copyright &#xA9; symbol and a CR \"\x0D\"";
    std::string expected = "A copyright &#xA9; symbol and a CR &quot;&#x0D;&quot;";
    std::string actual;
    XMLBase::EncodeString(source, actual);
    EXPECT_EQ(expected, actual);
}

} // namespace Test
} // namespace XMLParser
