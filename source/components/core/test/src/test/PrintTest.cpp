#include <unittest-cpp/UnitTestC++.h>

#include "core/Print.h"
#include "core/Trace.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class PrintTest : public UnitTestCpp::TestFixture
{
public:
    void SetUp() override
    {
        InitializeTrace(&_traceStream);
    }
    void TearDown() override
    {
        DeinitializeTrace();
    }
    std::ostringstream _traceStream;
};

TEST_FIXTURE(PrintTest, PrintCharPtrNarrow)
{
    const char * text = "Hello";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, text);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintCharPtrWide)
{
    const char * text = "Hello";
    const wchar_t * wtext = L"Hello";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, wtext);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintStringNarrow)
{
    std::string text = "Hello";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, text);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintStringWide)
{
    const char * text = "Hello";
    std::wstring wtext = L"Hello";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, wtext);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintNoFormat)
{
    const std::string text = "";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "", 123);

    EXPECT_EQ(text, stream.str());
    ASSERT_NE("", _traceStream.str());
    std::string message = _traceStream.str().substr(_traceStream.str().find('|'));
    EXPECT_EQ("|Warn |Not all parameters have been used. Parameters not used: 0\n", message);
}

TEST_FIXTURE(PrintTest, PrintSimple)
{
    const char * text = "123 456";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{} {}", 123, 456);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintIndexes)
{
    const char * text = "123 456 World Hello";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{} {} {3} {2}", 123, 456, "Hello", L"World");

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintWithWidth)
{
    const char * text = "       123 456           World Hello   ";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{,10} {,-10} {3,8} {2,-8}", 123, 456, "Hello", L"World");

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintIntegralWithFormat)
{
    const char * text = " 305419896    305419896 00010010001101000101011001111000  02215053170   12345678";
    std::ostringstream stream;
    OSAL::Console console(stream);
    int value = 0x12345678;
    Print(console, "{0,10:D} {0,12:D10} {0,32:B32} {0,12:O11} {0,10:X8}", value);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintFloatingPointWithFormat)
{
    // Apparently there is a difference between MinGW and Windows in how they print floating point numbers
#if defined(WIN_MINGW)
    const char * text = " 1234.5678  1234.5678  1234.5678   1234.568  01234.56780 1.2346e+003  1234.5678";
#else
    const char * text = " 1234.5678  1234.5678  1234.5678   1234.568  01234.56780 1.2346e+03  1234.5678";
#endif
    std::ostringstream stream;
    OSAL::Console console(stream);
    double value = 1234.5678;
    Print(console, "{0,10} {0,10:F4.4} {0,10:F3.4} {0,10:F3.3} {0,12:F5.5} {0,10:E4.4} {0,10:G5.4}", value);

    EXPECT_EQ(text, stream.str().c_str());
}

TEST_FIXTURE(PrintTest, PrintInvalidIndex)
{
    const std::string text = "";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{a}", 123);

    EXPECT_EQ(text, stream.str());
    ASSERT_NE("", _traceStream.str());
    std::string message = _traceStream.str().substr(_traceStream.str().find('|'));
    EXPECT_EQ("|Error|Invalid index specified: a\n", message);
}

TEST_FIXTURE(PrintTest, PrintIndexOutOfRangeLow)
{
    const std::string text = "";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{-1}", 123);

    EXPECT_EQ(text, stream.str());
    ASSERT_NE("", _traceStream.str());
    std::string message = _traceStream.str().substr(_traceStream.str().find('|'));
    EXPECT_EQ("|Error|Invalid index specified: -1\n", message);
}

TEST_FIXTURE(PrintTest, PrintIndexOutOfRangeHigh)
{
    const std::string text = "";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{1}", 123);

    EXPECT_EQ(text, stream.str());
    ASSERT_NE("", _traceStream.str());
    std::string message = _traceStream.str().substr(_traceStream.str().find('|'));
    EXPECT_EQ("|Error|Invalid index specified: 1 should be between 0 and 0\n", message);
}

TEST_FIXTURE(PrintTest, PrintInvalidWidth)
{
    const std::string text = "";
    std::ostringstream stream;
    OSAL::Console console(stream);
    Print(console, "{0,a}", 123);

    EXPECT_EQ(text, stream.str());
    ASSERT_NE("", _traceStream.str());
    std::string message = _traceStream.str().substr(_traceStream.str().find('|'));
    EXPECT_EQ("|Error|Invalid width specified: a\n", message);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
