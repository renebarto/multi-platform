#pragma once

#include <cassert>
#include <ctime>
#include "osal/Strings.h"
#include "osal/Time.h"
#include "core/Core.h"

namespace Core
{

class TimeSpan;

typedef void * DateTimeHandle;

enum class MonthType
{
    January = 1,
    February = 2,
    March = 3,
    April = 4,
    May = 5,
    June = 6,
    July = 7,
    August = 8,
    September = 9,
    October = 10,
    November = 11,
    December = 12,
};

enum class WeekDayType
{
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
    Sunday = 7,
};

class CORE_EXPORT DateTime
{
public:
    DateTime();
    DateTime(const DateTime & other);
    DateTime(int year, int month, int day, int hour, int minute, double second);
    DateTime(int year, int month, int day, int hour, int minute, int second, int microSeconds = 0);
    DateTime(int year, MonthType month, int day, int hour, int minute, double second);
    DateTime(int year, MonthType month, int day, int hour, int minute, int second, int microSeconds = 0);
    DateTime(time_t epochTime, bool localTime = false);
    DateTime(timespec timeSpec, bool localTime = false);
    DateTime(timeval timeVal, bool localTime = false);
    virtual ~DateTime();

    DateTime & operator = (const DateTime & other);
    DateTime & operator = (time_t epochTime);
    DateTime & operator = (timespec timeSpec);
    DateTime & operator = (timeval timeVal);

    operator time_t() const;
    operator timespec() const;
    operator timeval() const;

    DateTime & operator += (const TimeSpan & timeSpan);
    DateTime & operator -= (const TimeSpan & timeSpan);

    bool Equals(const DateTime & other) const;
    bool LessThan(const DateTime & other) const;

    static DateTime NowUTC();
    static DateTime NowLocal();
    static DateTime CreateUTC(int year, int month, int day, int hour, int minute, double second);
    static DateTime CreateUTC(int year, int month, int day, int hour, int minute, int second, int microSeconds = 0);
    static DateTime CreateUTC(int year, MonthType month, int day, int hour, int minute, double second);
    static DateTime CreateUTC(int year, MonthType month, int day, int hour, int minute, int second, int microSeconds = 0);
    static DateTime CreateLocal(int year, int month, int day, int hour, int minute, double second);
    static DateTime CreateLocal(int year, int month, int day, int hour, int minute, int second, int microSeconds = 0);
    static DateTime CreateLocal(int year, MonthType month, int day, int hour, int minute, double second);
    static DateTime CreateLocal(int year, MonthType month, int day, int hour, int minute, int second, int microSeconds = 0);

    int Hour() const;
    int Minute() const;
    int Second() const;
    int MicroSeconds() const;

    int Year() const;
    int Month() const;
    MonthType MonthName() const;
    int MonthDay() const;
    int YearDay() const;
    WeekDayType WeekDay() const;
    int WeekNumberingYear() const;
    int WeekOfYear() const;

    TimeSpan OffsetFromUTC() const;
    OSAL::String TimeZoneName() const;
    bool IsDaylightSavings() const;
    DateTime ConvertToLocalTime() const;
    DateTime ConvertToUTCTime() const;

    std::ostream & PrintTo(std::ostream & stream) const;
    std::ostream & PrintTo(std::ostream & stream, const OSAL::String & formatString) const;

    friend CORE_EXPORT DateTime operator + (const TimeSpan & lhs, const DateTime & rhs);
    friend CORE_EXPORT DateTime operator + (const DateTime & lhs, const TimeSpan & rhs);
    friend CORE_EXPORT DateTime operator - (const DateTime & lhs, const TimeSpan & rhs);
    friend CORE_EXPORT TimeSpan operator - (const DateTime & lhs, const DateTime & rhs);

    friend CORE_EXPORT bool operator == (const DateTime & lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator == (timespec lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator == (timeval lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator == (const DateTime & lhs, timespec rhs);
    friend CORE_EXPORT bool operator == (const DateTime & lhs, timeval rhs);
    friend CORE_EXPORT bool operator != (const DateTime & lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator != (timespec lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator != (timeval lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator != (const DateTime & lhs, timespec rhs);
    friend CORE_EXPORT bool operator != (const DateTime & lhs, timeval rhs);
    friend CORE_EXPORT bool operator < (const DateTime & lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator < (timespec lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator < (timeval lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator < (const DateTime & lhs, timespec rhs);
    friend CORE_EXPORT bool operator < (const DateTime & lhs, timeval rhs);
    friend CORE_EXPORT bool operator > (const DateTime & lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator > (timespec lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator > (timeval lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator > (const DateTime & lhs, timespec rhs);
    friend CORE_EXPORT bool operator > (const DateTime & lhs, timeval rhs);
    friend CORE_EXPORT bool operator <= (const DateTime & lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator <= (timespec lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator <= (timeval lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator <= (const DateTime & lhs, timespec rhs);
    friend CORE_EXPORT bool operator <= (const DateTime & lhs, timeval rhs);
    friend CORE_EXPORT bool operator >= (const DateTime & lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator >= (timespec lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator >= (timeval lhs, const DateTime & rhs);
    friend CORE_EXPORT bool operator >= (const DateTime & lhs, timespec rhs);
    friend CORE_EXPORT bool operator >= (const DateTime & lhs, timeval rhs);

private:
    timespec _time;
    OSAL::Time::tm _dateTime;
    void Assign(timespec value, bool localTime);
    void Assign(timeval value, bool localTime);
    void Assign(time_t value, bool localTime);
    void Assign(int year, int month, int day, int hour, int minute, double second, bool localTime);
    void Assign(const tm & value);
};

inline void PrintTo(const DateTime & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

} // namespace Core

inline std::ostream & operator << (std::ostream & stream, Core::DateTime value)
{
    value.PrintTo(stream);
    return stream;
}

