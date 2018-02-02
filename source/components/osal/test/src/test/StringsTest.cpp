#include <unittest-c++/UnitTestC++.h>

#include "osal/Strings.h"

namespace OSAL {
namespace Test {

class StringsTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void StringsTest::SetUp()
{
}

void StringsTest::TearDown()
{
}

TEST_SUITE(osal)
{

TEST_FIXTURE(StringsTest, Trim)
{
    EXPECT_EQ("abc", Strings::Trim("abc", "'\""));
    EXPECT_EQ("abc", Strings::Trim("'abc'", "'\""));
    EXPECT_EQ("abc", Strings::Trim("\"abc\"", "'\""));
    EXPECT_EQ("a'bc", Strings::Trim("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringsTest, TrimSpaces)
{
    EXPECT_EQ("abc", Strings::TrimSpaces("abc"));
    EXPECT_EQ("abc", Strings::TrimSpaces(" abc "));
    EXPECT_EQ("ab c", Strings::TrimSpaces(" ab c "));
}

TEST_FIXTURE(StringsTest, TrimLeft)
{
    EXPECT_EQ("abc", Strings::TrimLeft("abc", "'\""));
    EXPECT_EQ("abc'", Strings::TrimLeft("'abc'", "'\""));
    EXPECT_EQ("abc\"", Strings::TrimLeft("\"abc\"", "'\""));
    EXPECT_EQ("a'bc\"", Strings::TrimLeft("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringsTest, TrimSpacesLeft)
{
    EXPECT_EQ("abc", Strings::TrimSpacesLeft("abc"));
    EXPECT_EQ("abc ", Strings::TrimSpacesLeft(" abc "));
    EXPECT_EQ("ab c ", Strings::TrimSpacesLeft(" ab c "));
}

TEST_FIXTURE(StringsTest, TrimRight)
{
    EXPECT_EQ("abc", Strings::TrimRight("abc", "'\""));
    EXPECT_EQ("'abc", Strings::TrimRight("'abc'", "'\""));
    EXPECT_EQ("\"abc", Strings::TrimRight("\"abc\"", "'\""));
    EXPECT_EQ("\"a'bc", Strings::TrimRight("\"a'bc\"", "'\""));
}

TEST_FIXTURE(StringsTest, TrimSpaceRight)
{
    EXPECT_EQ("abc", Strings::TrimSpacesRight("abc"));
    EXPECT_EQ(" abc", Strings::TrimSpacesRight(" abc "));
    EXPECT_EQ(" ab c", Strings::TrimSpacesRight(" ab c "));
}

TEST_FIXTURE(StringsTest, IsEqualString)
{
    std::string aString("Test");
    std::string aString2("Test");
    std::string aString3("TEST");
    std::string aString4("Test1");
    EXPECT_TRUE(Strings::IsEqual(aString, aString));
    EXPECT_TRUE(Strings::IsEqual(aString, aString2));
    EXPECT_FALSE(Strings::IsEqual(aString, aString3));
    EXPECT_FALSE(Strings::IsEqual(aString, aString4));
    EXPECT_TRUE(Strings::IsEqual(aString2, aString));
    EXPECT_FALSE(Strings::IsEqual(aString2, aString3));
    EXPECT_FALSE(Strings::IsEqual(aString2, aString4));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString2));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString4));
}

TEST_FIXTURE(StringsTest, IsEqualIgnoreCaseString)
{
    std::string aString("Test");
    std::string aString2("Test");
    std::string aString3("TEST");
    std::string aString4("Test1");
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString3, aString4));
}

TEST_FIXTURE(StringsTest, IsEqualCharPtr)
{
    const char * aString = "Test";
    const char * aString2 = "Test";
    const char * aString3 = "TEST";
    const char * aString4 = "Test1";
    EXPECT_TRUE(Strings::IsEqual(aString, aString));
    EXPECT_TRUE(Strings::IsEqual(aString, aString2));
    EXPECT_FALSE(Strings::IsEqual(aString, aString3));
    EXPECT_FALSE(Strings::IsEqual(aString, aString4));
    EXPECT_TRUE(Strings::IsEqual(aString2, aString));
    EXPECT_FALSE(Strings::IsEqual(aString2, aString3));
    EXPECT_FALSE(Strings::IsEqual(aString2, aString4));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString2));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString4));
}

TEST_FIXTURE(StringsTest, IsEqualIgnoreCaseCharPtr)
{
    const char * aString = "Test";
    const char * aString2 = "Test";
    const char * aString3 = "TEST";
    const char * aString4 = "Test1";
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString3, aString4));
}

TEST_FIXTURE(StringsTest, IsEqualWCharPtr)
{
    const wchar_t * aString = L"Test";
    const wchar_t * aString2 = L"Test";
    const wchar_t * aString3 = L"TEST";
    const wchar_t * aString4 = L"Test1";
    EXPECT_TRUE(Strings::IsEqual(aString, aString));
    EXPECT_TRUE(Strings::IsEqual(aString, aString2));
    EXPECT_FALSE(Strings::IsEqual(aString, aString3));
    EXPECT_FALSE(Strings::IsEqual(aString, aString4));
    EXPECT_TRUE(Strings::IsEqual(aString2, aString));
    EXPECT_FALSE(Strings::IsEqual(aString2, aString3));
    EXPECT_FALSE(Strings::IsEqual(aString2, aString4));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString2));
    EXPECT_FALSE(Strings::IsEqual(aString3, aString4));
}

TEST_FIXTURE(StringsTest, IsEqualIgnoreCaseWCharPtr)
{
    const wchar_t * aString = L"Test";
    const wchar_t * aString2 = L"Test";
    const wchar_t * aString3 = L"TEST";
    const wchar_t * aString4 = L"Test1";
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString2));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString, aString3));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString, aString4));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString2, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString2, aString3));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString2, aString4));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString3, aString));
    EXPECT_TRUE(Strings::IsEqualIgnoreCase(aString3, aString2));
    EXPECT_FALSE(Strings::IsEqualIgnoreCase(aString3, aString4));
}

TEST_FIXTURE(StringsTest, ToLower)
{
    std::string input("Test");
    std::string expected("test");
    std::string actual = Strings::ToLower(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, ToUpper)
{
    std::string input("Test");
    std::string expected("TEST");
    std::string actual = Strings::ToUpper(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, QuoteUnquoted)
{
    std::string input("Test");
    std::string expected("\"Test\"");
    std::string actual = Strings::Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, QuoteQuoted)
{
    std::string input("\"Test\"");
    std::string expected("\"Test\"");
    std::string actual = Strings::Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, UnQuoteUnquoted)
{
    std::string input("Test");
    std::string expected("Test");
    std::string actual = Strings::UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, UnQuoteQuoted)
{
    std::string input("\"Test\"");
    std::string expected("Test");
    std::string actual = Strings::UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, StringToWString)
{
    std::string narrowString("Test");
    std::wstring expected(L"Test");
    std::wstring actual = Strings::StringToWString(narrowString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, WStringToString)
{
    std::wstring wideString(L"Test");
    std::string expected("Test");
    std::string actual = Strings::WStringToString(wideString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, ToStringNarrow)
{
    std::string narrowString("Test");
    std::string expected = "Test";
    std::string actual = Strings::ToString(narrowString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, ToStringWide)
{
    std::wstring wideString(L"Test");
    std::string expected = "Test";
    std::string actual = Strings::ToString(wideString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, ToNarrowString)
{
    std::string input("Test");
    std::string expected = "Test";
    std::string actual = Strings::ToNarrowString(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringsTest, ToWideString)
{
    std::string input("Test");
    std::wstring expected = L"Test";
    std::wstring actual = Strings::ToWideString(input);
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL