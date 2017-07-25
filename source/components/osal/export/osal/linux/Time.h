#pragma once

#include <unistd.h>
#include <sys/time.h>

namespace OSAL {
namespace Time {

inline int GetTimeOfDay(struct timeval * time, struct timezone * timeZone)
{
    return ::gettimeofday(time, timeZone);
}
inline void USleep(int64_t microSeconds)
{
    ::usleep(microSeconds);
}

struct tm
{
    tm()
        : _tm()
    {
        Update();
    }
    tm(int second, int minute, int hour, int day, int month, int year)
        : _tm()
    {
        _tm.tm_sec = second;
        _tm.tm_min = minute;
        _tm.tm_hour = hour;
        _tm.tm_mday = day;
        _tm.tm_mon = month;
        _tm.tm_year = year;
    }
    void Update()
    {
        tzset();
        tm_tzOffset = ::timezone;
        tm_dstOffset = ::daylight;
        tm_tzName = ::tzname[0];
    }
    tm & operator = (const ::tm & other)
    {
        tm_sec = other.tm_sec;
        tm_min = other.tm_min;
        tm_hour = other.tm_hour;
        tm_mday = other.tm_mday;
        tm_mon = other.tm_mon;
        tm_year = other.tm_year;
        tm_wday = other.tm_wday;
        tm_yday = other.tm_yday;
        tm_isdst = other.tm_isdst;
        Update();
        return *this;
    }
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
    long tm_tzOffset;
    int tm_dstOffset;
    const char * tm_tzName;
};

inline tm * localtime(const time_t * timep)
{
    static tm _time;
    _time = *::localtime(timep);
    return &_time;
}

inline tm * gmtime(const time_t * timep)
{
    static tm _time;
    _time = *::gmtime(timep);
    return &_time;
}

inline time_t mktime(tm * tim)
{
    return ::mktime(&(tim->_tm));
}

} // namespace Time
} // namespace OSAL