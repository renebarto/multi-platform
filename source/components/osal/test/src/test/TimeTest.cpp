#include <unittest-cpp/UnitTestC++.h>

#include "osal/Time.h"
//#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Test {

class TimeTest : public UnitTestCpp::TestFixture
{
//public:
//    static void AssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
//    {
//        _hadAssertion = true;
//        _expression = expression;
//        _expressionText = expressionText;
//        _file = file;
//        _line = line;
//        _func = func;
//    }
//    static bool _hadAssertion;
//    static bool _expression;
//    static const char * _expressionText;
//    static const char * _file;
//    static int _line;
//    static const char * _func;
//
//    void SetUp() override
//    {
//        _hadAssertion = false;
//        _expression = false;
//        _expressionText = nullptr;
//        _file = nullptr;
//        _line = 0;
//        _func = nullptr;
//        SetAssertionHandler(AssertionHandler);
//    }
};

//bool TimeTest::_hadAssertion = false;
//bool TimeTest::_expression = false;
//const char * TimeTest::_expressionText = nullptr;
//const char * TimeTest::_file = nullptr;
//int TimeTest::_line = 0;
//const char * TimeTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(TimeTest, timespec_Constructor)
{
    Time::timespec time;

    EXPECT_EQ(long {0}, time.tv_sec);
    EXPECT_EQ(long {0}, time.tv_nsec);
}

TEST_FIXTURE(TimeTest, timespec_EqualsOperator)
{
    const long Seconds = 1234;
    const long NanoSeconds = 123456789;
    Time::timespec time1(Seconds, NanoSeconds);
    Time::timespec time2;
    Time::timespec time3(Seconds, 0);
    Time::timespec time4(0, NanoSeconds);
    Time::timespec time5(Seconds, NanoSeconds);

    EXPECT_FALSE(time1 == time2);
    EXPECT_FALSE(time1 == time3);
    EXPECT_FALSE(time1 == time4);
    EXPECT_TRUE(time1 == time5);
}

TEST_FIXTURE(TimeTest, timespec_NotEqualsOperator)
{
    const long Seconds = 1234;
    const long NanoSeconds = 123456789;
    Time::timespec time1(Seconds, NanoSeconds);
    Time::timespec time2;
    Time::timespec time3(Seconds, 0);
    Time::timespec time4(0, NanoSeconds);
    Time::timespec time5(Seconds, NanoSeconds);

    EXPECT_TRUE(time1 != time2);
    EXPECT_TRUE(time1 != time3);
    EXPECT_TRUE(time1 != time4);
    EXPECT_FALSE(time1 != time5);
}

TEST_FIXTURE(TimeTest, timespec_GreaterOperator)
{
    const long Seconds = 1234;
    const long SecondsLess = 1233;
    const long SecondsMore = 1235;
    const long NanoSeconds = 123456789;
    const long NanoSecondsLess = 123456788;
    const long NanoSecondsMore = 123456790;
    Time::timespec time1(Seconds, NanoSeconds);
    Time::timespec time2;
    Time::timespec time3(Seconds, NanoSecondsLess);
    Time::timespec time4(Seconds, NanoSecondsMore);
    Time::timespec time5(SecondsLess, NanoSeconds);
    Time::timespec time6(SecondsMore, NanoSeconds);
    Time::timespec time7(Seconds, NanoSeconds);

    EXPECT_TRUE(time1 > time2);
    EXPECT_TRUE(time1 > time3);
    EXPECT_FALSE(time1 > time4);
    EXPECT_TRUE(time1 > time5);
    EXPECT_FALSE(time1 > time6);
    EXPECT_FALSE(time1 > time7);
}

TEST_FIXTURE(TimeTest, timespec_GreaterEqualsOperator)
{
    const long Seconds = 1234;
    const long SecondsLess = 1233;
    const long SecondsMore = 1235;
    const long NanoSeconds = 123456789;
    const long NanoSecondsLess = 123456788;
    const long NanoSecondsMore = 123456790;
    Time::timespec time1(Seconds, NanoSeconds);
    Time::timespec time2;
    Time::timespec time3(Seconds, NanoSecondsLess);
    Time::timespec time4(Seconds, NanoSecondsMore);
    Time::timespec time5(SecondsLess, NanoSeconds);
    Time::timespec time6(SecondsMore, NanoSeconds);
    Time::timespec time7(Seconds, NanoSeconds);

    EXPECT_TRUE(time1 >= time2);
    EXPECT_TRUE(time1 >= time3);
    EXPECT_FALSE(time1 >= time4);
    EXPECT_TRUE(time1 >= time5);
    EXPECT_FALSE(time1 >= time6);
    EXPECT_TRUE(time1 >= time7);
}

TEST_FIXTURE(TimeTest, timespec_SmallerOperator)
{
    const long Seconds = 1234;
    const long SecondsLess = 1233;
    const long SecondsMore = 1235;
    const long NanoSeconds = 123456789;
    const long NanoSecondsLess = 123456788;
    const long NanoSecondsMore = 123456790;
    Time::timespec time1(Seconds, NanoSeconds);
    Time::timespec time2;
    Time::timespec time3(Seconds, NanoSecondsLess);
    Time::timespec time4(Seconds, NanoSecondsMore);
    Time::timespec time5(SecondsLess, NanoSeconds);
    Time::timespec time6(SecondsMore, NanoSeconds);
    Time::timespec time7(Seconds, NanoSeconds);

    EXPECT_FALSE(time1 < time2);
    EXPECT_FALSE(time1 < time3);
    EXPECT_TRUE(time1 < time4);
    EXPECT_FALSE(time1 < time5);
    EXPECT_TRUE(time1 < time6);
    EXPECT_FALSE(time1 < time7);
}

TEST_FIXTURE(TimeTest, timespec_SmallerEqualsOperator)
{
    const long Seconds = 1234;
    const long SecondsLess = 1233;
    const long SecondsMore = 1235;
    const long NanoSeconds = 123456789;
    const long NanoSecondsLess = 123456788;
    const long NanoSecondsMore = 123456790;
    Time::timespec time1(Seconds, NanoSeconds);
    Time::timespec time2;
    Time::timespec time3(Seconds, NanoSecondsLess);
    Time::timespec time4(Seconds, NanoSecondsMore);
    Time::timespec time5(SecondsLess, NanoSeconds);
    Time::timespec time6(SecondsMore, NanoSeconds);
    Time::timespec time7(Seconds, NanoSeconds);

    EXPECT_FALSE(time1 <= time2);
    EXPECT_FALSE(time1 <= time3);
    EXPECT_TRUE(time1 <= time4);
    EXPECT_FALSE(time1 <= time5);
    EXPECT_TRUE(time1 <= time6);
    EXPECT_TRUE(time1 <= time7);
}

TEST_FIXTURE(TimeTest, timespec_PrintTo)
{
    const long Seconds = 1234;
    const long NanoSeconds = 123456789;
    Time::timespec time(Seconds, NanoSeconds);
    ostringstream stream;
    time.PrintTo(stream);
    string expected = "tv_sec=1234 tv_nsec=123456789";
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
