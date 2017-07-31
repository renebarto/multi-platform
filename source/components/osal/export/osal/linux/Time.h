#pragma once

#include <unistd.h>
#include <sys/time.h>

namespace OSAL {
namespace Time {

inline int gettimeofday(struct timeval * time, struct timezone * timeZone)
{
    return ::gettimeofday(time, timeZone);
}
inline void usleep(int64_t microSeconds)
{
    ::usleep(static_cast<__useconds_t>(microSeconds));
}
inline int nanosleep(const timespec * req, timespec * rem)
{
	::nanosleep(req, res);
}

OSAL_EXPORT struct tm
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
        Update();
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

inline size_t strftime(char * strDest, size_t maxSize, const char * format, const ::tm * time)
{
	return ::strftime(strDest, maxSize, format, time);
}
inline size_t strftime(wchar_t * strDest, size_t maxSize, const wchar_t * format, const ::tm * time)
{
	return ::wcsftime(strDest, maxSize, format, time);
}

} // namespace Time
} // namespace OSAL