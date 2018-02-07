#include <unittest-cpp/UnitTestC++.h>

#include "osal/Time.h"
//#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Test {

class TimeTest : public UnitTestCpp::TestFixture
{
public:
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
    void VerifyTimeZoneInfo(const Time::tm & time);
};

//bool TimeTest::_hadAssertion = false;
//bool TimeTest::_expression = false;
//const char * TimeTest::_expressionText = nullptr;
//const char * TimeTest::_file = nullptr;
//int TimeTest::_line = 0;
//const char * TimeTest::_func = nullptr;

void TimeTest::VerifyTimeZoneInfo(const Time::tm & time)
{
    if (time.GetTime().tm_isdst)
    {
        if (Strings::IsEqualIgnoreCase(time.NonDaylightSavingsTimeZoneName(), "CET"))
        {
            EXPECT_EQ("CEST", time.ActiveTimeZoneName());
            EXPECT_EQ("CET", time.NonDaylightSavingsTimeZoneName());
            EXPECT_EQ("CEST", time.DaylightSavingsActiveTimeZoneName());
            EXPECT_EQ(-7200, time.ActiveLocalTimeOffsetSeconds());
            EXPECT_EQ(-3600, time.LocalTimeOffsetSecondsNonDaylightSavings());
            EXPECT_EQ(-7200, time.LocalTimeOffsetSecondsDaylightSavings());
        }
    } else
    {
        if (Strings::IsEqualIgnoreCase(time.NonDaylightSavingsTimeZoneName(), "CET"))
        {
            EXPECT_EQ("CET", time.ActiveTimeZoneName());
            EXPECT_EQ("CET", time.NonDaylightSavingsTimeZoneName());
            EXPECT_EQ("CEST", time.DaylightSavingsActiveTimeZoneName());
            EXPECT_EQ(-3600, time.ActiveLocalTimeOffsetSeconds());
            EXPECT_EQ(-3600, time.LocalTimeOffsetSecondsNonDaylightSavings());
            EXPECT_EQ(-7200, time.LocalTimeOffsetSecondsDaylightSavings());
        }
    }
}

TEST_SUITE(osal)
{

TEST_FIXTURE(TimeTest, timespec_ConstructorDefault)
{
    Time::timespec time;

    EXPECT_EQ(long {0}, time.tv_sec);
    EXPECT_EQ(long {0}, time.tv_nsec);
}

TEST_FIXTURE(TimeTest, timespec_Constructor)
{
    const long Seconds = 1234;
    const long NanoSeconds = 123456789;
    Time::timespec time(Seconds, NanoSeconds);

    EXPECT_EQ(Seconds, time.tv_sec);
    EXPECT_EQ(NanoSeconds, time.tv_nsec);
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

TEST_FIXTURE(TimeTest, tm_ConstructorDefault)
{
    Time::tm time;

    EXPECT_EQ(long {0}, time.GetTime().tm_year);
    EXPECT_EQ(long {0}, time.GetTime().tm_mon);
    EXPECT_EQ(long {0}, time.GetTime().tm_mday);
    EXPECT_EQ(long {0}, time.GetTime().tm_hour);
    EXPECT_EQ(long {0}, time.GetTime().tm_min);
    EXPECT_EQ(long {0}, time.GetTime().tm_sec);
    EXPECT_EQ(long {0}, time.GetTime().tm_yday);
    EXPECT_EQ(long {0}, time.GetTime().tm_wday);
    EXPECT_EQ(long {0}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, tm_Constructor_tm)
{
    ::tm tm {};
    Time::tm time(tm, true);

    EXPECT_EQ(long {0}, time.GetTime().tm_year);
    EXPECT_EQ(long {0}, time.GetTime().tm_mon);
    EXPECT_EQ(long {0}, time.GetTime().tm_mday);
    EXPECT_EQ(long {0}, time.GetTime().tm_hour);
    EXPECT_EQ(long {0}, time.GetTime().tm_min);
    EXPECT_EQ(long {0}, time.GetTime().tm_sec);
    EXPECT_EQ(long {0}, time.GetTime().tm_yday);
    EXPECT_EQ(long {0}, time.GetTime().tm_wday);
    EXPECT_EQ(long {0}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, tm_Constructor_tmInitialized)
{
    const int Second = 56;
    const int Minute = 34;
    const int Hour = 21;
    const int Day = 12;
    const int Month = 7;
    const int Year = 2013;
    const int YearDay = 31 + 28 + 31 + 30 + 31 + 30 + Day - 1;
    const int Weekday = 5; // Friday
    ::tm tm {};
    tm.tm_year = Year - 1900;
    tm.tm_mon = Month - 1;
    tm.tm_mday = Day;
    tm.tm_hour = Hour;
    tm.tm_min = Minute;
    tm.tm_sec = Second;
    tm.tm_yday = YearDay;
    tm.tm_wday = Weekday;
    tm.tm_isdst = 1;
    Time::tm time(tm, true);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour, time.GetTime().tm_hour);
    EXPECT_EQ(Minute, time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {1}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, tm_ConstructorInitializer)
{
    const int Second = 56;
    const int Minute = 34;
    const int Hour = 21;
    const int Day = 12;
    const int Month = 7;
    const int Year = 2013;
    const int YearDay = 31 + 28 + 31 + 30 + 31 + 30 + Day - 1;
    const int Weekday = 5; // Friday
    Time::tm time(Second, Minute, Hour, Day, Month, Year, true);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour, time.GetTime().tm_hour);
    EXPECT_EQ(Minute, time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {1}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, tm_CopyConstructor)
{
    const int Second = 6;
    const int Minute = 44;
    const int Hour = 21;
    const int Day = 7;
    const int Month = 2;
    const int Year = 2018;
    const int YearDay = 31 + Day - 1;
    const int Weekday = 3; // Wednesday
    Time::tm other(Second, Minute, Hour, Day, Month, Year, true);
    Time::tm time(other);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour, time.GetTime().tm_hour);
    EXPECT_EQ(Minute, time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {0}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, localtime_summer)
{
    const int Second = 56;
    const int Minute = 34;
    const int Hour = 21;
    const int Day = 12;
    const int Month = 7;
    const int Year = 2013;
    const int YearDay = 31 + 28 + 31 + 30 + 31 + 30 + Day - 1;
    const int Weekday = 5; // Friday
    time_t epochTime = 1373657696;
    Time::tm time = *Time::localtime(&epochTime);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour, time.GetTime().tm_hour);
    EXPECT_EQ(Minute, time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {1}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, localtime_winter)
{
    const int Second = 6;
    const int Minute = 44;
    const int Hour = 21;
    const int Day = 7;
    const int Month = 2;
    const int Year = 2018;
    const int YearDay = 31 + Day - 1;
    const int Weekday = 3; // Wednesday
    time_t epochTime = 1518036246;
    Time::tm time = *Time::localtime(&epochTime);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour, time.GetTime().tm_hour);
    EXPECT_EQ(Minute, time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {0}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, gmtime_summer)
{
    const int Second = 56;
    const int Minute = 34;
    const int Hour = 21;
    const int Day = 12;
    const int Month = 7;
    const int Year = 2013;
    const int YearDay = 31 + 28 + 31 + 30 + 31 + 30 + Day - 1;
    const int Weekday = 5; // Friday
    time_t epochTime = 1373657696;
    Time::tm time = *Time::gmtime(&epochTime);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour + (time.LocalTimeOffsetSecondsDaylightSavings() / 3600), time.GetTime().tm_hour);
    EXPECT_EQ(Minute + ((time.LocalTimeOffsetSecondsDaylightSavings() % 3600) / 60), time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {0}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, gmtime_winter)
{
    const int Second = 6;
    const int Minute = 44;
    const int Hour = 21;
    const int Day = 7;
    const int Month = 2;
    const int Year = 2018;
    const int YearDay = 31 + Day - 1;
    const int Weekday = 3; // Wednesday
    time_t epochTime = 1518036246;
    Time::tm time = *Time::gmtime(&epochTime);

    EXPECT_EQ(Year - 1900, time.GetTime().tm_year);
    EXPECT_EQ(Month - 1, time.GetTime().tm_mon);
    EXPECT_EQ(Day, time.GetTime().tm_mday);
    EXPECT_EQ(Hour + (time.LocalTimeOffsetSecondsNonDaylightSavings() / 3600), time.GetTime().tm_hour);
    EXPECT_EQ(Minute + ((time.LocalTimeOffsetSecondsNonDaylightSavings() % 3600) / 60), time.GetTime().tm_min);
    EXPECT_EQ(Second, time.GetTime().tm_sec);
    EXPECT_EQ(YearDay, time.GetTime().tm_yday);
    EXPECT_EQ(Weekday, time.GetTime().tm_wday);
    EXPECT_EQ(long {0}, time.GetTime().tm_isdst);
    VerifyTimeZoneInfo(time);
}

TEST_FIXTURE(TimeTest, mktime_summer)
{
    const int Second = 56;
    const int Minute = 34;
    const int Hour = 21;
    const int Day = 12;
    const int Month = 7;
    const int Year = 2013;
    Time::tm time(Second, Minute, Hour, Day, Month, Year, true);
    time_t epochTimeRef = 1373657696;
    time_t epochTime = Time::mktime(&time);

    EXPECT_EQ(epochTimeRef, epochTime);
}

TEST_FIXTURE(TimeTest, mktime_winter)
{
    const int Second = 6;
    const int Minute = 44;
    const int Hour = 21;
    const int Day = 7;
    const int Month = 2;
    const int Year = 2018;
    Time::tm time(Second, Minute, Hour, Day, Month, Year, true);
    time_t epochTimeRef = 1518036246;
    time_t epochTime = Time::mktime(&time);

    EXPECT_EQ(epochTimeRef, epochTime);
}

TEST_FIXTURE(TimeTest, strftime_local)
{
    const int Second = 6;
    const int Minute = 44;
    const int Hour = 21;
    const int Day = 7;
    const int Month = 2;
    const int Year = 2018;
    Time::tm time(Second, Minute, Hour, Day, Month, Year, true);

    string expected = "2018-02-07 21:44:06";
    string actual = Time::strftime("%F %T", &time);
    EXPECT_EQ(expected, actual);

    expected += " " + time.ActiveTimeZoneName();
    actual = Time::strftime("%F %T %Z", &time);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TimeTest, strftime_gmt)
{
    const int Second = 6;
    const int Minute = 44;
    const int Hour = 21;
    const int Day = 7;
    const int Month = 2;
    const int Year = 2018;
    Time::tm time(Second, Minute, Hour, Day, Month, Year, false);

    ostringstream stream;
    stream << setw(4) << setfill('0') << Year << "-"
           << setw(2) << setfill('0') << Month << "-"
           << setw(2) << setfill('0') << Day << " "
           << setw(2) << setfill('0') << Hour + (time.LocalTimeOffsetSecondsNonDaylightSavings() / 3600) << ":"
           << setw(2) << setfill('0') << Minute + ((time.LocalTimeOffsetSecondsNonDaylightSavings() % 3600) / 60) << ":"
           << setw(2) << setfill('0') << Second;
    string expected = stream.str();
    string actual = Time::strftime("%F %T", &time);
    EXPECT_EQ(expected, actual);

    expected += " GMT";
    actual = Time::strftime("%F %T %Z", &time);
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
