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
    EXPECT_EQ(_("abc"), Trim(_("abc"), _("'\"")));
    EXPECT_EQ(_("abc"), Trim(_("'abc'"), _("'\"")));
    EXPECT_EQ(_("abc"), Trim(_("\"abc\""), _("'\"")));
    EXPECT_EQ(_("a'bc"), Trim(_("\"a'bc\""), _("'\"")));
}

TEST_FIXTURE(StringTest, TrimSpaces)
{
    EXPECT_EQ(_("abc"), TrimSpaces(_("abc")));
    EXPECT_EQ(_("abc"), TrimSpaces(_(" abc ")));
    EXPECT_EQ(_("ab c"), TrimSpaces(_(" ab c ")));
}

TEST_FIXTURE(StringTest, TrimLeft)
{
    EXPECT_EQ(_("abc"), TrimLeft(_("abc"), _("'\"")));
    EXPECT_EQ(_("abc'"), TrimLeft(_("'abc'"), _("'\"")));
    EXPECT_EQ(_("abc\""), TrimLeft(_("\"abc\""), _("'\"")));
    EXPECT_EQ(_("a'bc\""), TrimLeft(_("\"a'bc\""), _("'\"")));
}

TEST_FIXTURE(StringTest, TrimSpacesLeft)
{
    EXPECT_EQ(_("abc"), TrimSpacesLeft(_("abc")));
    EXPECT_EQ(_("abc "), TrimSpacesLeft(_(" abc ")));
    EXPECT_EQ(_("ab c "), TrimSpacesLeft(_(" ab c ")));
}

TEST_FIXTURE(StringTest, TrimRight)
{
    EXPECT_EQ(_("abc"), TrimRight(_("abc"), _("'\"")));
    EXPECT_EQ(_("'abc"), TrimRight(_("'abc'"), _("'\"")));
    EXPECT_EQ(_("\"abc"), TrimRight(_("\"abc\""), _("'\"")));
    EXPECT_EQ(_("\"a'bc"), TrimRight(_("\"a'bc\""), _("'\"")));
}

TEST_FIXTURE(StringTest, TrimSpaceRight)
{
    EXPECT_EQ(_("abc"), TrimSpacesRight(_("abc")));
    EXPECT_EQ(_(" abc"), TrimSpacesRight(_(" abc ")));
    EXPECT_EQ(_(" ab c"), TrimSpacesRight(_(" ab c ")));
}

TEST_FIXTURE(StringTest, IsEqualString)
{
    String aString(_("Test"));
    String aString2(_("Test"));
    String aString3(_("TEST"));
    String aString4(_("Test1"));
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
    String aString(_("Test"));
    String aString2(_("Test"));
    String aString3(_("TEST"));
    String aString4(_("Test1"));
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

TEST_FIXTURE(StringTest, ToLower)
{
    String input(_("Test"));
    String expected(_("test"));
    String actual = ToLower(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, ToUpper)
{
    String input(_("Test"));
    String expected(_("TEST"));
    String actual = ToUpper(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, QuoteUnquoted)
{
    String input(_("Test"));
    String expected(_("\"Test\""));
    String actual = Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, QuoteQuoted)
{
    String input(_("\"Test\""));
    String expected(_("\"Test\""));
    String actual = Quote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, UnQuoteUnquoted)
{
    String input(_("Test"));
    String expected(_("Test"));
    String actual = UnQuote(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, UnQuoteQuoted)
{
    String input(_("\"Test\""));
    String expected(_("Test"));
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

TEST_FIXTURE(StringTest, ToStringNarrow)
{
    std::string narrowString("Test");
    OSAL::String expected = _("Test");
    OSAL::String actual = ToString(narrowString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, ToStringWide)
{
    std::wstring wideString(L"Test");
    OSAL::String expected = _("Test");
    OSAL::String actual = ToString(wideString);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, ToNarrowString)
{
    OSAL::String input(_("Test"));
    std::string expected = "Test";
    std::string actual = ToNarrowString(input);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(StringTest, ToWideString)
{
    OSAL::String input(_("Test"));
    std::wstring expected = L"Test";
    std::wstring actual = ToWideString(input);
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL