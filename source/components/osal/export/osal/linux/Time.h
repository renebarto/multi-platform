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
    ::usleep(static_cast<__useconds_t>(microSeconds));
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
        _tm = other;
        Update();
        return *this;
    }
    struct ::tm _tm;
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