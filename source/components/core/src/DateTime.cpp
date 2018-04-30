#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <time.h>
#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/Exception.h"
#include "core/serialization/Serialization.h"
#include "core/DateTime.h"
#include "core/TimeSpan.h"

using namespace std;
namespace Core {

static constexpr int SecondsPerHour = 3600;
static const string UTCTimeZoneName = "GMT";

// Invariant: _dateTime contains actual time (local or UTC)
//            _time contains timespec (epoch and nanoseconds), irrespective of local or UTC time.
// So: for example if the date is
//   Monday, May 26, 2014 12:02:03 AM UTC
//   _datetime will contain that date, _isLocalTime is false, and _time will hold the epoch time 1401062523
// If the date is Monday, May 26, 2014 2:02:03 AM CEST (i.e. the same date in local time)
//   _datetime will contain that date, _isLocalTime is true, and _time will hold the same epoch time 1401062523

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
    , _isLocalTime()
{
    Assign(time_t(0), false);
}

DateTime::DateTime(const DateTime & other)
    : _time(other._time)
    , _dateTime(other._dateTime)
    , _isLocalTime(other._isLocalTime)
{
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, double second)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    Assign(year, month, day, hour, minute, second, true);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second,
                   int microSeconds /*= 0*/)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    ASSERT((microSeconds >= 0) || (microSeconds < MicroSecondsPerSecond));
    Assign(year, month, day, hour, minute, second + double(microSeconds) / MicroSecondsPerSecond, true);
}

DateTime::DateTime(int year, MonthType month, int day, int hour, int minute, double second)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    Assign(year, ConvertMonth(month), day, hour, minute, second, true);
}

DateTime::DateTime(int year, MonthType month, int day, int hour, int minute, int second,
                   int microSeconds /*= 0*/)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    ASSERT((microSeconds >= 0) || (microSeconds < MicroSecondsPerSecond));
    Assign(year, ConvertMonth(month), day, hour, minute,
           second + double(microSeconds) / MicroSecondsPerSecond, true);
}

DateTime::DateTime(time_t epochTime, bool localTime /*= false*/)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    Assign(epochTime, localTime);
}

DateTime::DateTime(timespec timeSpec, bool localTime /*= false*/)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    Assign(timeSpec, localTime);
}

DateTime::DateTime(timeval timeVal, bool localTime /*= false*/)
    : _time()
    , _dateTime()
    , _isLocalTime()
{
    Assign(timeVal, localTime);
}

DateTime::~DateTime()
{
}

DateTime & DateTime::operator =(const DateTime & other)
{
    if (this != &other)
    {
        _time = other._time;
        _dateTime = other._dateTime;
    }
    return *this;
}

DateTime & DateTime::operator =(time_t epochTime)
{
    Assign(epochTime, false);
    return *this;
}

DateTime & DateTime::operator =(timespec timeSpec)
{
    Assign(timeSpec, false);
    return *this;
}

DateTime & DateTime::operator =(timeval timeVal)
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
    return timeval { static_cast<long>(_time.tv_sec), _time.tv_nsec / NanoSecondsPerMicroSecond };
}

int DateTime::DiffFromUTC() const
{
    if (!_isLocalTime)
        return 0;
    return static_cast<int>(_dateTime.ActiveLocalTimeOffsetSeconds());
}

DateTime & DateTime::operator +=(const TimeSpan & timeSpan)
{
    OSAL::Time::timespec newTime(_time.tv_sec + (_time.tv_nsec + timeSpan.NanoSeconds()) / NanoSecondsPerSecond,
                                 (_time.tv_nsec + timeSpan.NanoSeconds()) % NanoSecondsPerSecond);
    Assign(newTime, _isLocalTime);
    return *this;
}

DateTime & DateTime::operator -=(const TimeSpan & timeSpan)
{
    OSAL::Time::timespec newTime;
    newTime.tv_nsec = (_time.tv_nsec - (timeSpan.NanoSeconds() % NanoSecondsPerSecond) + NanoSecondsPerSecond) %
                      NanoSecondsPerSecond;
    newTime.tv_sec = _time.tv_sec - timeSpan.NanoSeconds() / NanoSecondsPerSecond - (newTime.tv_nsec > _time.tv_nsec ? 1 : 0);
    Assign(newTime, _isLocalTime);
    return *this;
}

bool DateTime::Equals(const DateTime & other) const
{
    if (this != &other)
    {
        return (_time == other._time);
    }
    return true;
}

bool DateTime::LessThan(const DateTime & other) const
{
    if (this != &other)
    {
        return (_time < other._time);
    }
    return false;
}

DateTime DateTime::NowUTC()
{
    OSAL::Time::timespec time;
    OSAL::Time::ClockGetTime(CLOCK_REALTIME, &time);
//    time_t epochTimeLocal = time.tv_sec;
//    OSAL::Time::tm localTime = *OSAL::Time::localtime(&epochTimeLocal);
//    time.tv_sec += localTime.ActiveLocalTimeOffsetSeconds();
    return DateTime(time, false);
}

DateTime DateTime::NowLocal()
{
    OSAL::Time::timespec time;
    OSAL::Time::ClockGetTime(CLOCK_REALTIME, &time);
    return DateTime(time, true);
}

DateTime DateTime::CreateUTC(int year, int month, int day, int hour, int minute, double second)
{
    DateTime dateTime;
    dateTime.Assign(year, month, day, hour, minute, second, false);
    return dateTime;
}

DateTime DateTime::CreateUTC(int year, int month, int day, int hour, int minute, int second,
                             int microSeconds /*= 0*/)
{
    ASSERT((microSeconds >= 0) || (microSeconds < MicroSecondsPerSecond));
    DateTime dateTime;
    dateTime.Assign(year, month, day, hour, minute,
                    second + static_cast<double>(microSeconds) / MicroSecondsPerSecond, false);
    return dateTime;
}

DateTime DateTime::CreateUTC(int year, MonthType month, int day, int hour, int minute, double second)
{
    DateTime dateTime;
    dateTime.Assign(year, ConvertMonth(month), day, hour, minute, second, false);
    return dateTime;
}

DateTime DateTime::CreateUTC(int year, MonthType month, int day, int hour, int minute, int second,
                             int microSeconds /*= 0*/)
{
    ASSERT((microSeconds >= 0) || (microSeconds < MicroSecondsPerSecond));
    DateTime dateTime;
    dateTime.Assign(year, ConvertMonth(month), day, hour, minute,
                    second + static_cast<double>(microSeconds) / MicroSecondsPerSecond, false);
    return dateTime;
}

DateTime DateTime::CreateLocal(int year, int month, int day, int hour, int minute, double second)
{
    return DateTime(year, month, day, hour, minute, second);
}

DateTime DateTime::CreateLocal(int year, int month, int day, int hour, int minute, int second,
                               int microSeconds /*= 0*/)
{
    ASSERT((microSeconds >= 0) || (microSeconds < MicroSecondsPerSecond));
    return DateTime(year, month, day, hour, minute, second, microSeconds);
}

DateTime DateTime::CreateLocal(int year, MonthType month, int day, int hour, int minute,
                               double second)
{
    return DateTime(year, month, day, hour, minute, second);
}

DateTime DateTime::CreateLocal(int year, MonthType month, int day, int hour, int minute, int second,
                               int microSeconds /*= 0*/)
{
    ASSERT((microSeconds >= 0) || (microSeconds < MicroSecondsPerSecond));
    return DateTime(year, month, day, hour, minute, second, microSeconds);
}

int DateTime::Hour() const
{
    return _dateTime.GetTime().tm_hour;
}

int DateTime::Minute() const
{
    return _dateTime.GetTime().tm_min;
}

int DateTime::Second() const
{
    return _dateTime.GetTime().tm_sec;
}

int DateTime::MicroSeconds() const
{
    return int(_time.tv_nsec / NanoSecondsPerMicroSecond);
}

int DateTime::Year() const
{
    return _dateTime.GetTime().tm_year + 1900;
}

int DateTime::Month() const
{
    return _dateTime.GetTime().tm_mon + 1;
}

MonthType DateTime::MonthName() const
{
    return ConvertMonth(Month());
}

int DateTime::MonthDay() const
{
    return _dateTime.GetTime().tm_mday;
}

int DateTime::YearDay() const
{
    return _dateTime.GetTime().tm_yday + 1;
}

WeekDayType DateTime::WeekDay() const
{
    return (_dateTime.GetTime().tm_wday == 0) ? WeekDayType::Sunday : ConvertWeekDay(_dateTime.GetTime().tm_wday);
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
    int dayOfYearCorrection;
    int leapDays;
    int dayOfYear;
    int weekDaysCorrection;
    int correctedDayOfYear;
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
    } else
    {
        calcLeapYearsReference = year;
        numLeapYearsSinceAD = (calcLeapYearsReference / 4) - (calcLeapYearsReference / 100) +
                              (calcLeapYearsReference / 400);
        numLeapYearsSinceADPrev = ((calcLeapYearsReference - 1) / 4) -
                                  ((calcLeapYearsReference - 1) / 100) +
                                  ((calcLeapYearsReference - 1) / 400);
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

    weekDaysCorrection = (calcLeapYearsReference + numLeapYearsSinceAD) % 7;
    dayOfYearCorrection = (dayOfYear + weekDaysCorrection - leapDays) % 7;
    correctedDayOfYear = dayOfYear + 3 - dayOfYearCorrection;

    if (correctedDayOfYear < 0)
        return 53 - ((weekDaysCorrection - numLeapYearsDiff) / 5);
    else if (correctedDayOfYear > 364 + numLeapYearsDiff)
        return 1;
    return (correctedDayOfYear / 7) + 1;
}

TimeSpan DateTime::OffsetFromUTC() const
{
    return TimeSpan(DiffFromUTC() * NanoSecondsPerSecond);
}

std::string DateTime::TimeZoneName() const
{
    if (!_isLocalTime)
        return UTCTimeZoneName;
    return _dateTime.ActiveTimeZoneName();
}

bool DateTime::IsDaylightSavings() const
{
    return _dateTime.GetTime().tm_isdst != 0;
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

void DateTime::PrintTo(std::ostream & stream) const
{
    PrintTo(stream, "%Y-%m-%d %H:%M:%S");
    stream << "." << dec << setfill('0') << setw(6) << MicroSeconds();
}

void DateTime::PrintTo(std::ostream & stream, string const & formatString) const
{
    char buffer[1000];
    if (0 == strftime(buffer, sizeof(buffer), formatString.c_str(), &_dateTime.GetTime()))
        throw OSAL::Exception(__func__, __FILE__, __LINE__, "Invalid format string.");
    stream << string(buffer);
}

void DateTime::Assign(timespec value, bool localTime)
{
    time_t rawtime = value.tv_sec;
    if (localTime)
        _dateTime = *OSAL::Time::localtime(&rawtime);
    else
        _dateTime = *OSAL::Time::gmtime(&rawtime);
//    rawtime = OSAL::Time::mktime(&_dateTime);
//    if (!localTime)
//        rawtime -= _dateTime.ActiveLocalTimeOffsetSeconds();
//    else
//        rawtime += _dateTime.ActiveDaylightSavingsTimeOffsetSeconds();

    _time = value;
    _isLocalTime = localTime;
}

void DateTime::Assign(timeval value, bool localTime)
{
    time_t rawtime = value.tv_sec;
    if (localTime)
        _dateTime = *OSAL::Time::localtime(&rawtime);
    else
        _dateTime = *OSAL::Time::gmtime(&rawtime);
    rawtime = OSAL::Time::mktime(&_dateTime);
    if (!localTime)
        rawtime -= _dateTime.ActiveLocalTimeOffsetSeconds();
    else
        rawtime += _dateTime.ActiveDaylightSavingsTimeOffsetSeconds();

    _time = { rawtime, int64_t(value.tv_usec) * NanoSecondsPerMicroSecond };
    _isLocalTime = localTime;
}

void DateTime::Assign(time_t value, bool localTime)
{
    time_t rawtime = value;
    if (localTime)
        _dateTime = *OSAL::Time::localtime(&rawtime);
    else
        _dateTime = *OSAL::Time::gmtime(&rawtime);
    rawtime = OSAL::Time::mktime(&_dateTime);
    if (!localTime)
        rawtime -= _dateTime.ActiveLocalTimeOffsetSeconds();
    else
        rawtime += _dateTime.ActiveDaylightSavingsTimeOffsetSeconds();

    _time = { rawtime, 0 };
    _isLocalTime = localTime;
}

void DateTime::Assign(int year, int month, int day, int hour, int minute, double second,
                      bool localTime)
{
    _dateTime = OSAL::Time::tm(static_cast<int>(second), minute, hour, day, month, year, localTime);
    time_t rawtime = OSAL::Time::mktime(&_dateTime);
    if (!localTime)
        rawtime -= _dateTime.ActiveLocalTimeOffsetSeconds();

    _time = { rawtime, static_cast<long>((second - int(second)) * NanoSecondsPerSecond + 0.5) };
    _isLocalTime = localTime;
}

void DateTime::Assign(const tm & value)
{
    _dateTime = value;
    _time = { OSAL::Time::mktime(&_dateTime), 0};
    _isLocalTime = false;
}

DateTime operator +(const TimeSpan & lhs, const DateTime & rhs)
{
    DateTime datetime = rhs;
    datetime += lhs;
    return datetime;
}

DateTime operator +(const DateTime & lhs, const TimeSpan & rhs)
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

WARNING_PUSH
WARNING_DISABLE(4592)
template<>
vector<Core::EnumConversion<Core::WeekDayType>> Core::EnumSerializationInfo<Core::WeekDayType>::Info =
{
    {Core::WeekDayType::Monday,    "Monday"},
    {Core::WeekDayType::Tuesday,   "Tuesday"},
    {Core::WeekDayType::Wednesday, "Wednesday"},
    {Core::WeekDayType::Thursday,  "Thursday"},
    {Core::WeekDayType::Friday,    "Friday"},
    {Core::WeekDayType::Saturday,  "Saturday"},
    {Core::WeekDayType::Sunday,    "Sunday"},
};

template<>
vector<Core::EnumConversion<Core::MonthType>> Core::EnumSerializationInfo<Core::MonthType>::Info =
{
    {Core::MonthType::January,   "January"},
    {Core::MonthType::February,  "February"},
    {Core::MonthType::March,     "March"},
    {Core::MonthType::April,     "April"},
    {Core::MonthType::May,       "May"},
    {Core::MonthType::June,      "June"},
    {Core::MonthType::July,      "July"},
    {Core::MonthType::August,    "August"},
    {Core::MonthType::September, "September"},
    {Core::MonthType::October,   "October"},
    {Core::MonthType::November,  "November"},
    {Core::MonthType::December,  "December"},
};
WARNING_POP

ostream & Core::operator << (ostream & stream, Core::WeekDayType value)
{
    stream << Core::EnumSerializationInfo<Core::WeekDayType>::Serialize(value);
    return stream;
}

ostream & Core::operator << (ostream & stream, Core::MonthType value)
{
    stream << Core::EnumSerializationInfo<Core::MonthType>::Serialize(value);
    return stream;
}
