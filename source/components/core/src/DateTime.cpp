#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <sys/time.h>
#include "core/DateTime.h"
#include "core/Core.h"
#include "core/TimeSpan.h"

using namespace std;
namespace Core
{

const int64_t DateTime::MicroSecondsPerSecond = 1000000;
const int64_t DateTime::NanoSecondsPerMicroSecond = 1000;
const int64_t DateTime::NanoSecondsPerSecond = DateTime::NanoSecondsPerMicroSecond * DateTime::MicroSecondsPerSecond;
static const int SecondsPerHour = 3600;

int ConvertMonth(MonthType monthType)
{
    return int(monthType);
}

MonthType ConvertMonth(int month)
{
    return MonthType(month);
}

int ConvertWeekDay(WeekDayType weekDay)
{
    return int(weekDay);
}

WeekDayType ConvertWeekDay(int weekDay)
{
    return WeekDayType(weekDay);
}

DateTime::DateTime()
    : _time()
    , _dateTime()
{
    Assign(time_t(0), false);
}

DateTime::DateTime(const DateTime & other)
    : _time(other._time)
    , _dateTime(other._dateTime)
{
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, double second)
    : _time()
    , _dateTime()
{
    Assign(year, month, day, hour, minute, second, true);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int microSeconds /*= 0*/)
    : _time()
    , _dateTime()
{
    if ((microSeconds < 0) || (microSeconds >= MicroSecondsPerSecond))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "DateTime::DateTime", "microSeconds");
    Assign(year, month, day, hour, minute, second + double(microSeconds) / MicroSecondsPerSecond, true);
}

DateTime::DateTime(int year, MonthType month, int day, int hour, int minute, double second)
    : _time()
    , _dateTime()
{
    Assign(year, ConvertMonth(month), day, hour, minute, second, true);
}

DateTime::DateTime(int year, MonthType month, int day, int hour, int minute, int second, int microSeconds /*= 0*/)
    : _time()
    , _dateTime()
{
    if ((microSeconds < 0) || (microSeconds >= MicroSecondsPerSecond))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "DateTime::DateTime", "microSeconds");
    Assign(year, ConvertMonth(month), day, hour, minute, second + double(microSeconds) / MicroSecondsPerSecond, true);
}

DateTime::DateTime(time_t epochTime, bool localTime /*= false*/)
    : _time()
    , _dateTime()
{
    Assign(epochTime, localTime);
}

DateTime::DateTime(timespec timeSpec, bool localTime /*= false*/)
    : _time()
    , _dateTime()
{
    Assign(timeSpec, localTime);
}

DateTime::DateTime(timeval timeVal, bool localTime /*= false*/)
    : _time()
    , _dateTime()
{
    Assign(timeVal, localTime);
}

DateTime::~DateTime()
{
}

DateTime & DateTime::operator = (const DateTime & other)
{
    if (this != &other)
    {
        _time = other._time;
        _dateTime = other._dateTime;
    }
    return *this;
}

DateTime & DateTime::operator = (time_t epochTime)
{
    Assign(epochTime, false);
    return *this;
}

DateTime & DateTime::operator = (timespec timeSpec)
{
    Assign(timeSpec, false);
    return *this;
}

DateTime & DateTime::operator = (timeval timeVal)
{
    Assign(timeVal, false);
    return *this;
}

DateTime::operator time_t() const
{
    return _time.tv_sec;
}

DateTime::operator timespec() const
{
    return _time;
}

DateTime::operator timeval() const
{
    return timeval {_time.tv_sec, _time.tv_nsec / NanoSecondsPerMicroSecond};
}

DateTime & DateTime::operator += (const TimeSpan & timeSpan)
{
    bool isLocalTime = (_dateTime.tm_gmtoff != 0);
    timespec newTime;
    newTime.tv_nsec = (_time.tv_nsec + timeSpan.NanoSeconds()) % NanoSecondsPerSecond;
    newTime.tv_sec = _time.tv_sec + (_time.tv_nsec + timeSpan.NanoSeconds()) / NanoSecondsPerSecond;
    Assign(newTime, isLocalTime);
    return *this;
}

DateTime & DateTime::operator -= (const TimeSpan & timeSpan)
{
    bool isLocalTime = (_dateTime.tm_gmtoff != 0);
    timespec newTime;
    newTime.tv_nsec = (_time.tv_nsec - (timeSpan.NanoSeconds() % NanoSecondsPerSecond) + NanoSecondsPerSecond) % NanoSecondsPerSecond;
    newTime.tv_sec = _time.tv_sec - timeSpan.NanoSeconds() / NanoSecondsPerSecond - (newTime.tv_nsec > _time.tv_nsec ? 1 : 0);
    Assign(newTime, isLocalTime);
    return *this;
}

bool DateTime::Equals(const DateTime & other) const
{
    if (this != &other)
    {
        return (_time.tv_sec == other._time.tv_sec) &&
               (_time.tv_nsec == other._time.tv_nsec);
    }
    return true;
}

bool DateTime::LessThan(const DateTime & other) const
{
    if (this != &other)
    {
        return (_time.tv_sec < other._time.tv_sec) ||
               ((_time.tv_sec == other._time.tv_sec) && (_time.tv_nsec < other._time.tv_nsec));
    }
    return false;
}

DateTime DateTime::NowUTC()
{
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return DateTime(time);
}

DateTime DateTime::NowLocal()
{
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return DateTime(time, true);
}

DateTime DateTime::CreateUTC(int year, int month, int day, int hour, int minute, double second)
{
    DateTime dateTime;
    dateTime.Assign(year, month, day, hour, minute, second, false);
    return dateTime;
}

DateTime DateTime::CreateUTC(int year, int month, int day, int hour, int minute, int second, int microSeconds /*= 0*/)
{
    if ((microSeconds < 0) || (microSeconds >= MicroSecondsPerSecond))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "DateTime::CreateUTC", "microSeconds");
    DateTime dateTime;
    dateTime.Assign(year, month, day, hour, minute, second + (double)microSeconds / MicroSecondsPerSecond, false);
    return dateTime;
}

DateTime DateTime::CreateUTC(int year, MonthType month, int day, int hour, int minute, double second)
{
    DateTime dateTime;
    dateTime.Assign(year, ConvertMonth(month), day, hour, minute, second, false);
    return dateTime;
}

DateTime DateTime::CreateUTC(int year, MonthType month, int day, int hour, int minute, int second, int microSeconds /*= 0*/)
{
    if ((microSeconds < 0) || (microSeconds >= MicroSecondsPerSecond))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "DateTime::CreateUTC", "microSeconds");
    DateTime dateTime;
    dateTime.Assign(year, ConvertMonth(month), day, hour, minute, second + (double)microSeconds / MicroSecondsPerSecond, false);
    return dateTime;
}

DateTime DateTime::CreateLocal(int year, int month, int day, int hour, int minute, double second)
{
    return DateTime(year, month, day, hour, minute, second);
}

DateTime DateTime::CreateLocal(int year, int month, int day, int hour, int minute, int second, int microSeconds /*= 0*/)
{
    if ((microSeconds < 0) || (microSeconds >= MicroSecondsPerSecond))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "DateTime::CreateLocal", "microSeconds");
    return DateTime(year, month, day, hour, minute, second, microSeconds);
}

DateTime DateTime::CreateLocal(int year, MonthType month, int day, int hour, int minute, double second)
{
    return DateTime(year, month, day, hour, minute, second);
}

DateTime DateTime::CreateLocal(int year, MonthType month, int day, int hour, int minute, int second, int microSeconds /*= 0*/)
{
    if ((microSeconds < 0) || (microSeconds >= MicroSecondsPerSecond))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "DateTime::CreateLocal", "microSeconds");
    return DateTime(year, month, day, hour, minute, second, microSeconds);
}

int DateTime::Hour() const
{
    return _dateTime.tm_hour;
}

int DateTime::Minute() const
{
    return _dateTime.tm_min;
}

int DateTime::Second() const
{
    return _dateTime.tm_sec;
}

int DateTime::MicroSeconds() const
{
    return int(_time.tv_nsec / NanoSecondsPerMicroSecond);
}

int DateTime::Year() const
{
    return _dateTime.tm_year + 1900;
}

int DateTime::Month() const
{
    return _dateTime.tm_mon + 1;
}

MonthType DateTime::MonthName() const
{
    return ConvertMonth(Month());
}

int DateTime::MonthDay() const
{
    return _dateTime.tm_mday;
}

int DateTime::YearDay() const
{
    return _dateTime.tm_yday + 1;
}

WeekDayType DateTime::WeekDay() const
{
    return (_dateTime.tm_wday == 0) ? WeekDayType::Sunday : ConvertWeekDay(_dateTime.tm_wday);
}

int DateTime::WeekNumberingYear() const
{
    int year = Year();
    int month = Month();
    int day = MonthDay();
    WeekDayType weekday = WeekDay();

    // January 1, 2, 3 might be in the previous year if they occur after Thursday.
    //
    // Jan 1:  Friday, Saturday, Sunday    =>  day 1:  weekday 5, 6, 7
    // Jan 2:  Saturday, Sunday            =>  day 2:  weekday 6, 7
    // Jan 3:  Sunday                      =>  day 3:  weekday 7
    //
    // So we have a special case if (day - weekday) <= -4
    if (month == 1 && (day - ConvertWeekDay(weekday)) <= -4)
        return year - 1;

    // December 29, 30, 31 might be in the next year if they occur before Thursday.
    //
    // Dec 31: Monday, Tuesday, Wednesday  =>  day 31: weekday 1, 2, 3
    // Dec 30: Monday, Tuesday             =>  day 30: weekday 1, 2
    // Dec 29: Monday                      =>  day 29: weekday 1
    //
    // So we have a special case if (day - weekday) >= 28
    else if (month == 12 && (day - ConvertWeekDay(weekday)) >= 28)
        return year + 1;

    return year;
}

int DateTime::WeekOfYear() const
{
    int calcLeapYearsReference;
    int numLeapYearsSinceAD;
    int numLeapYearsSinceADPrev;
    int d;
    int leapDays;
    int dayOfYear;
    int g;
    int n;
    int numLeapYearsDiff;
    int month = Month();
    int day = MonthDay();
    int year = Year();

    if (month <= 2)
    {
        calcLeapYearsReference = year - 1;
        numLeapYearsSinceAD = (calcLeapYearsReference / 4) -
                              (calcLeapYearsReference / 100) +
                              (calcLeapYearsReference / 400);
        numLeapYearsSinceADPrev = ((calcLeapYearsReference - 1) / 4) -
                                  ((calcLeapYearsReference - 1) / 100) +
                                  ((calcLeapYearsReference - 1) / 400);
        numLeapYearsDiff = numLeapYearsSinceAD - numLeapYearsSinceADPrev;
        leapDays = 0;
        // January:  month==1, dayOfYear = day - 1
        // February: month==2, dayOfYear = 31 + day - 1
        dayOfYear = day - 1 + (31 * (month - 1));
    }
    else
    {
        calcLeapYearsReference = year;
        numLeapYearsSinceAD = (calcLeapYearsReference / 4) - (calcLeapYearsReference / 100) + (calcLeapYearsReference / 400);
        numLeapYearsSinceADPrev = ((calcLeapYearsReference - 1) / 4) - ((calcLeapYearsReference - 1) / 100) + ((calcLeapYearsReference - 1) / 400);
        numLeapYearsDiff = numLeapYearsSinceAD - numLeapYearsSinceADPrev;
        leapDays = numLeapYearsDiff + 1;
        // March: month==3, dayOfYear = 31 + 28 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + numLeapYearsDiff
        // April: month==4, dayOfYear = 31 + 28 + 31 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + 155 / 5 (31) + numLeapYearsDiff
        // May:   month==5, dayOfYear = 31 + 28 + 31 + 30 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + 308 / 5 (61) + numLeapYearsDiff
        // June:  month==6, dayOfYear = 31 + 28 + 31 + 30 + 31 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + 461 / 5 (92) + numLeapYearsDiff
        // July:  month==7, dayOfYear = 31 + 28 + 31 + 30 + 31 + 30 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + 614 / 5 (122) + numLeapYearsDiff
        // Aug:   month==8, dayOfYear = 31 + 28 + 31 + 30 + 31 + 30 + 31 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + 767 / 5 (153) + numLeapYearsDiff
        // Sep:   month==9, dayOfYear = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + day - 1 + leapYear ? 1 : 0 =
        //                              day + 58 + 920 / 5 (184) + numLeapYearsDiff
        // Oct:   month==10, dayOfYear = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + day - 1 + leapYear ? 1 : 0 =
        //                               day + 58 + 1073 / 5 (214) + numLeapYearsDiff
        // Nov:   month==11, dayOfYear = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + day - 1 + leapYear ? 1 : 0 =
        //                               day + 58 + 1226 / 5 (245) + numLeapYearsDiff
        // Dec:   month==12, dayOfYear = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + day - 1 + leapYear ? 1 : 0 =
        //                               day + 58 + 1379 / 5 (275) + numLeapYearsDiff
        dayOfYear = day + (((153 * (month - 3)) + 2) / 5) + 58 + numLeapYearsDiff;
    }

    g = (calcLeapYearsReference + numLeapYearsSinceAD) % 7;
    d = (dayOfYear + g - leapDays) % 7;
    n = dayOfYear + 3 - d;

    if (n < 0)
        return 53 - ((g - numLeapYearsDiff) / 5);
    else if (n > 364 + numLeapYearsDiff)
        return 1;
    return (n / 7) + 1;
}

TimeSpan DateTime::OffsetFromUTC() const
{
    return TimeSpan(_dateTime.tm_gmtoff * NanoSecondsPerSecond);
}

std::string DateTime::TimeZoneName() const
{
    return _dateTime.tm_zone;
}

bool DateTime::IsDaylightSavings() const
{
    return _dateTime.tm_isdst;
}

DateTime DateTime::ConvertToLocalTime() const
{
    DateTime localTime;
    localTime.Assign(_time, true);
    return localTime;
}

DateTime DateTime::ConvertToUTCTime() const
{
    DateTime utcTime;
    utcTime.Assign(_time, false);
    return utcTime;
}

string DateTime::ToString() const
{
    ostringstream stream;

    stream << ToString("%Y-%m-%d %H:%M:%S") << "." << setfill('0') << setw(6) << MicroSeconds();
    return stream.str();
}

string DateTime::ToString(const string & formatString) const
{
    char buffer[1000];
    if (0 == strftime(buffer, sizeof(buffer), formatString.c_str(), &_dateTime))
        throw Core::Exception(__func__, __FILE__, __LINE__, "Invalid format string.");
    return string(buffer);
}

void DateTime::Assign(timespec value, bool localTime)
{
    _time = value;
    time_t rawtime = _time.tv_sec;
    if (localTime)
        _dateTime = *localtime(&rawtime);
    else
        _dateTime = *gmtime(&rawtime);
}

void DateTime::Assign(timeval value, bool localTime)
{
    _time.tv_sec = value.tv_sec;
    _time.tv_nsec = int64_t(value.tv_usec) * NanoSecondsPerMicroSecond;
    time_t rawtime = _time.tv_sec;
    if (localTime)
        _dateTime = *localtime(&rawtime);
    else
        _dateTime = *gmtime(&rawtime);
}

void DateTime::Assign(time_t value, bool localTime)
{
    _time.tv_sec = value;
    _time.tv_nsec = 0;
    time_t rawtime = _time.tv_sec;
    if (localTime)
        _dateTime = *localtime(&rawtime);
    else
        _dateTime = *gmtime(&rawtime);
}

void DateTime::Assign(int year, int month, int day, int hour, int minute, double second, bool localTime)
{
    tm local { int(second), minute, hour, day, month - 1, year - 1900, 0, 0, 0, 0, 0 };
    time_t rawtime = mktime(&local);
    if (localTime)
    {
        if (local.tm_isdst)
        {
            rawtime -= SecondsPerHour;
        }
        _dateTime = *localtime(&rawtime);
    }
    else
    {
        rawtime += local.tm_gmtoff;
        _dateTime = *gmtime(&rawtime);
    }
    _time.tv_sec = rawtime;
    _time.tv_nsec = int64_t((second - int(second)) * NanoSecondsPerSecond + 0.5);
}

void DateTime::Assign(const tm & value)
{
    _dateTime = value;
    _time.tv_sec = mktime(&_dateTime);
}

DateTime operator + (const TimeSpan & lhs, const DateTime & rhs)
{
    DateTime datetime = rhs;
    datetime += lhs;
    return datetime;
}

DateTime operator + (const DateTime & lhs, const TimeSpan & rhs)
{
    DateTime datetime = lhs;
    datetime += rhs;
    return datetime;
}

DateTime operator - (const DateTime & lhs, const TimeSpan & rhs)
{
    DateTime datetime = lhs;
    datetime -= rhs;
    return datetime;
}

TimeSpan operator - (const DateTime & lhs, const DateTime & rhs)
{
    int64_t diffNanoSeconds = (lhs._time.tv_sec * DateTime::NanoSecondsPerSecond + lhs._time.tv_nsec) -
                              (rhs._time.tv_sec * DateTime::NanoSecondsPerSecond + rhs._time.tv_nsec);
    return TimeSpan(diffNanoSeconds);
}

bool operator == (const DateTime & lhs, const DateTime & rhs)
{
    return lhs.Equals(rhs);
}

bool operator == (timespec lhs, const DateTime & rhs)
{
    DateTime date(lhs);
    return rhs.Equals(date);
}

bool operator == (timeval lhs, const DateTime & rhs)
{
    DateTime date(lhs);
    return rhs.Equals(date);
}

bool operator == (const DateTime & lhs, timespec rhs)
{
    DateTime date(rhs);
    return lhs.Equals(date);
}

bool operator == (const DateTime & lhs, timeval rhs)
{
    DateTime date(rhs);
    return lhs.Equals(date);
}

bool operator != (const DateTime & lhs, const DateTime & rhs)
{
    return !(lhs == rhs);
}

bool operator != (timespec lhs, const DateTime & rhs)
{
    return !(lhs == rhs);
}

bool operator != (timeval lhs, const DateTime & rhs)
{
    return !(lhs == rhs);
}

bool operator != (const DateTime & lhs, timespec rhs)
{
    return !(lhs == rhs);
}

bool operator != (const DateTime & lhs, timeval rhs)
{
    return !(lhs == rhs);
}

bool operator < (const DateTime & lhs, const DateTime & rhs)
{
    return lhs.LessThan(rhs);
}

bool operator < (timespec lhs, const DateTime & rhs)
{
    DateTime time(lhs);
    return time.LessThan(rhs);
}

bool operator < (timeval lhs, const DateTime & rhs)
{
    DateTime time(lhs);
    return time.LessThan(rhs);
}

bool operator < (const DateTime & lhs, timespec rhs)
{
    DateTime time(rhs);
    return lhs.LessThan(time);
}

bool operator < (const DateTime & lhs, timeval rhs)
{
    DateTime time(rhs);
    return lhs.LessThan(time);
}

bool operator > (const DateTime & lhs, const DateTime & rhs)
{
    return !(lhs.LessThan(rhs) || lhs.Equals(rhs));
}

bool operator > (timespec lhs, const DateTime & rhs)
{
    DateTime time(lhs);
    return !(time.LessThan(rhs) || time.Equals(rhs));
}

bool operator > (timeval lhs, const DateTime & rhs)
{
    DateTime time(lhs);
    return !(time.LessThan(rhs) || time.Equals(rhs));
}

bool operator > (const DateTime & lhs, timespec rhs)
{
    DateTime time(rhs);
    return !(lhs.LessThan(time) || lhs.Equals(time));
}

bool operator > (const DateTime & lhs, timeval rhs)
{
    DateTime time(rhs);
    return !(lhs.LessThan(time) || lhs.Equals(time));
}

bool operator <= (const DateTime & lhs, const DateTime & rhs)
{
    return !(lhs > rhs);
}

bool operator <= (timespec lhs, const DateTime & rhs)
{
    return !(lhs > rhs);
}

bool operator <= (timeval lhs, const DateTime & rhs)
{
    return !(lhs > rhs);
}

bool operator <= (const DateTime & lhs, timespec rhs)
{
    return !(lhs > rhs);
}

bool operator <= (const DateTime & lhs, timeval rhs)
{
    return !(lhs > rhs);
}

bool operator >= (const DateTime & lhs, const DateTime & rhs)
{
    return !(lhs < rhs);
}

bool operator >= (timespec lhs, const DateTime & rhs)
{
    return !(lhs < rhs);
}

bool operator >= (timeval lhs, const DateTime & rhs)
{
    return !(lhs < rhs);
}

bool operator >= (const DateTime & lhs, timespec rhs)
{
    return !(lhs < rhs);
}

bool operator >= (const DateTime & lhs, timeval rhs)
{
    return !(lhs < rhs);
}

} // namespace Core
