#include <unittest-c++/UnitTestC++.h>

#include <string>
#include "xml/TestData.h"
#include "xml/XMLDeclaration.h"
#include "xml/XMLDocument.h"
#include "xml/XMLParsingData.h"
#include "xml/XMLTestVisitor.h"

using namespace std;

namespace XMLParser {
namespace Test {

const string Version = "Version";
const string Encoding = "Encoding";
const string Standalone = "Standalone";
const string NoVersion = "none";
const string NoEncoding = "none";
const string NoStandalone = "none";
const int TabSize = 4;

class XMLDeclarationTest: public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void XMLDeclarationTest::SetUp()
{
    XMLBase::SetTabSize(TabSize);
}

void XMLDeclarationTest::TearDown()
{
}

TEST_SUITE(xml)
{

TEST_FIXTURE(XMLDeclarationTest, Construct)
{
    XMLDeclaration target;

    EXPECT_EQ("", target.Value());
    EXPECT_EQ("", target.Version());
    EXPECT_EQ("", target.Encoding());
    EXPECT_EQ("", target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, ConstructVersionEncodingStandalone)
{
    XMLDeclaration target(Version, Encoding, Standalone);

    EXPECT_EQ("", target.Value());
    EXPECT_EQ(Version, target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, ConstructCopy)
{
    XMLDeclaration other(Version, Encoding, Standalone);
    XMLDeclaration target(other);

    EXPECT_EQ("", target.Value());
    EXPECT_EQ(Version, target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, Assign)
{
    XMLDeclaration other(Version, Encoding, Standalone);
    XMLDeclaration target;

    target = other;

    EXPECT_EQ("", target.Value());
    EXPECT_EQ(Version, target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, ParseString)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "<?xml version=\"" + Version + "\""
        " encoding=\"" + Encoding + "\""
                                " standalone=\"" + Standalone + "\" ?>";
    target.ParseString(xmlString);
    EXPECT_EQ(Version, target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, ParseStringEmpty)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ(NoVersion, target.Version());
        EXPECT_EQ(NoEncoding, target.Encoding());
        EXPECT_EQ(NoStandalone, target.Standalone());
        EXPECT_EQ(XMLError::ErrorParsingEmpty, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDeclarationTest, ParseStringUnknownAttribute)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "<?xml someversion=\"" + Version + "\""
        " encoding=\"" + Encoding + "\""
                                " standalone=\"" + Standalone + "\" ?>";
    target.ParseString(xmlString);
    EXPECT_EQ("", target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, ParseStringMissingAttribute)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "<?xml encoding=\"" + Encoding + "\""
        " standalone=\"" + Standalone + "\" ?>";
    target.ParseString(xmlString);
    EXPECT_EQ("", target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, ParseStringIncorrectTag)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "<xml version=\"" + Version + "\""
        " encoding=\"" + Encoding + "\""
                                " standalone=\"" + Standalone + "\"";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("", target.Version());
        EXPECT_EQ("", target.Encoding());
        EXPECT_EQ("", target.Standalone());
        EXPECT_EQ(XMLError::ErrorParsingDeclaration, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(1, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDeclarationTest, ParseStringNoTagEnd)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "<?xml version=\"" + Version + "\""
        " encoding=\"" + Encoding + "\""
                                " standalone=\"" + Standalone + "\"";
    bool hadException = false;
    try
    {
        target.ParseString(xmlString);
    }
    catch (const XMLParseException & e)
    {
        hadException = true;
        EXPECT_EQ("", target.Version());
        EXPECT_EQ("", target.Encoding());
        EXPECT_EQ("", target.Standalone());
        EXPECT_EQ(XMLError::ErrorParsingDeclaration, e.ErrorInfo().ErrorCode());
        EXPECT_EQ(1, e.ErrorInfo().ErrorRow());
        EXPECT_EQ(68, e.ErrorInfo().ErrorCol());
    }
    EXPECT_TRUE(hadException);
}

TEST_FIXTURE(XMLDeclarationTest, ParseStringWithSpacesValue)
{
    XMLDeclaration target(NoVersion, NoEncoding, NoStandalone);
    std::string xmlString = "<?xml version=\" " + Version + "\t\""
        " encoding=\"" + Encoding + "\""
                                " standalone=\"" + Standalone + "\" ?>";
    target.ParseString(xmlString);
    EXPECT_EQ(" " + Version + "\t", target.Version());
    EXPECT_EQ(Encoding, target.Encoding());
    EXPECT_EQ(Standalone, target.Standalone());
}

TEST_FIXTURE(XMLDeclarationTest, GenerateString)
{
    XMLDeclaration target(Version, Encoding, Standalone);

    std::string expected = "<?xml version=\"" + Version + "\""
        " encoding=\"" + Encoding + "\""
                               " standalone=\"" + Standalone + "\" ?>";
    std::string actual = target.GenerateString();
    EXPECT_EQ(expected, actual);

    actual = target.GenerateString(1);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(XMLDeclarationTest, Accept)
{
    XMLTestVisitor visitor;
    XMLDeclaration target(Version, Encoding, Standalone);

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
    EXPECT_TRUE(visitor.visitedByDeclaration);
    EXPECT_FALSE(visitor.visitedByText);
    EXPECT_FALSE(visitor.visitedByUnknown);
}

} // TEST_SUITE(xml)
} // namespace Test
} // namespace XMLParser
