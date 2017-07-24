#include <unit-test-c++/UnitTestC++.h>

#include "osal/Strings.h"

namespace OSAL {
namespace Test {

class StringTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void StringTest::SetUp()
{
}

void StringTest::TearDown()
{
}

TEST_SUITE(osal)
{

TEST_FIXTURE(StringTest, Trim)
{
    EXPECT_EQ("abc", Trim("abc", "'\""));
    EXPECT_EQ("abc", Trim("'abc'", "'\""));
    EXPECT_EQ("abc", Trim("\"abc\"", "'\""));
    EXPECT_EQ("a'bc", Trim("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringTest, TrimSpaces)
{
    EXPECT_EQ("abc", TrimSpaces("abc"));
    EXPECT_EQ("abc", TrimSpaces(" abc "));
    EXPECT_EQ("ab c", TrimSpaces(" ab c "));
}

TEST_FIXTURE(StringTest, TrimLeft)
{
    EXPECT_EQ("abc", TrimLeft("abc", "'\""));
    EXPECT_EQ("abc'", TrimLeft("'abc'", "'\""));
    EXPECT_EQ("abc\"", TrimLeft("\"abc\"", "'\""));
    EXPECT_EQ("a'bc\"", TrimLeft("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringTest, TrimSpacesLeft)
{
    EXPECT_EQ("abc", TrimSpacesLeft("abc"));
    EXPECT_EQ("abc ", TrimSpacesLeft(" abc "));
    EXPECT_EQ("ab c ", TrimSpacesLeft(" ab c "));
}

TEST_FIXTURE(StringTest, TrimRight)
{
    EXPECT_EQ("abc", TrimRight("abc", "'\""));
    EXPECT_EQ("'abc", TrimRight("'abc'", "'\""));
    EXPECT_EQ("\"abc", TrimRight("\"abc\"", "'\""));
    EXPECT_EQ("\"a'bc", TrimRight("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringTest, TrimSpaceRight)
{
    EXPECT_EQ("abc", TrimSpacesRight("abc"));
    EXPECT_EQ(" abc", TrimSpacesRight(" abc "));
    EXPECT_EQ(" ab c", TrimSpacesRight(" ab c "));
}

TEST_FIXTURE(StringTest, IsEqualString)
{
    String aString("Test");
    String aString2("Test");
    String aString3("TEST");
    String aString4("Test1");
    EXPECT_TRUE(IsEqual(aString, aString));
    EXPECT_TRUE(IsEqual(aString, aString2));
    EXPECT_FALSE(IsEqual(aString, aString3));
    EXPECT_FALSE(IsEqual(aString, aString4));
    EXPECT_TRUE(IsEqual(aString2, aString));
    EXPECT_FALSE(IsEqual(aString2, aString3));
    EXPECT_FALSE(IsEqual(aString2, aString4));
    EXPECT_FALSE(IsEqual(aString3, aString));
    EXPECT_FALSE(IsEqual(aString3, aString2));
    EXPECT_FALSE(IsEqual(aString3, aString4));
}

TEST_FIXTURE(StringTest, IsEqualIgnoreCaseString)
{
    String aString("Test");
    String aString2("Test");
    String aString3("TEST");
    String aString4("Test1");
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(IsEqualIgnoreCase(aString3, aString4));
}

TEST_FIXTURE(StringTest, IsEqualCharPtr)
{
    const char * aString = "Test";
    const char * aString2 = "Test";
    const char * aString3 = "TEST";
    const char * aString4 = "Test1";
    EXPECT_TRUE(IsEqual(aString, aString));
    EXPECT_TRUE(IsEqual(aString, aString2));
    EXPECT_FALSE(IsEqual(aString, aString3));
    EXPECT_FALSE(IsEqual(aString, aString4));
    EXPECT_TRUE(IsEqual(aString2, aString));
    EXPECT_FALSE(IsEqual(aString2, aString3));
    EXPECT_FALSE(IsEqual(aString2, aString4));
    EXPECT_FALSE(IsEqual(aString3, aString));
    EXPECT_FALSE(IsEqual(aString3, aString2));
    EXPECT_FALSE(IsEqual(aString3, aString4));
}

TEST_FIXTURE(StringTest, IsEqualIgnoreCaseCharPtr)
{
    const char * aString = "Test";
    const char * aString2 = "Test";
    const char * aString3 = "TEST";
    const char * aString4 = "Test1";
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(IsEqualIgnoreCase(aString3, aString4));
}

TEST_FIXTURE(StringTest, IsEqualWCharPtr)
{
    const wchar_t * aString = L"Test";
    const wchar_t * aString2 = L"Test";
    const wchar_t * aString3 = L"TEST";
    const wchar_t * aString4 = L"Test1";
    EXPECT_TRUE(IsEqual(aString, aString));
    EXPECT_TRUE(IsEqual(aString, aString2));
    EXPECT_FALSE(IsEqual(aString, aString3));
    EXPECT_FALSE(IsEqual(aString, aString4));
    EXPECT_TRUE(IsEqual(aString2, aString));
    EXPECT_FALSE(IsEqual(aString2, aString3));
    EXPECT_FALSE(IsEqual(aString2, aString4));
    EXPECT_FALSE(IsEqual(aString3, aString));
    EXPECT_FALSE(IsEqual(aString3, aString2));
    EXPECT_FALSE(IsEqual(aString3, aString4));
}

TEST_FIXTURE(StringTest, IsEqualIgnoreCaseWCharPtr)
{
    const wchar_t * aString = L"Test";
    const wchar_t * aString2 = L"Test";
    const wchar_t * aString3 = L"TEST";
    const wchar_t * aString4 = L"Test1";
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(IsEqualIgnoreCase(aString3, aString4));
}
//String ToLower(const String & text);
//String ToUpper(const String & text);
//String Quote(const String & text);
//String UnQuote(const String & text);

TEST_FIXTURE(StringTest, ToLower)
{
    String input("Test");
    String expected("test");
    String actual = ToLower(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, ToUpper)
{
    String input("Test");
    String expected("TEST");
    String actual = ToUpper(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, QuoteUnquoted)
{
    String input("Test");
    String expected("\"Test\"");
    String actual = Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, QuoteQuoted)
{
    String input("\"Test\"");
    String expected("\"Test\"");
    String actual = Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, UnQuoteUnquoted)
{
    String input("Test");
    String expected("Test");
    String actual = UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, UnQuoteQuoted)
{
    String input("\"Test\"");
    String expected("Test");
    String actual = UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, StringToWString)
{
    std::string narrowString("Test");
    std::wstring expected(L"Test");
    std::wstring actual = StringToWString(narrowString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, WStringToString)
{
    std::wstring wideString(L"Test");
    std::string expected("Test");
    std::string actual = WStringToString(wideString);
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL