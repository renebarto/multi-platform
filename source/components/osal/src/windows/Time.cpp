#include "osal/Time.h"

#include <climits>
#include <direct.h>

namespace OSAL {
namespace Time {

const tm & BaseTime()
{
    static tm baseTime;
    return baseTime;
}

const long tm::tm_tzOffset = 0;
const int tm::tm_dstOffset = 0;
const OSAL::Char tm::tm_tzName[MAX_TIME_ZONE_NAME + 1] = _("");

tm::tm(bool initialize)
: _tm()
{
    if (initialize)
        Update();
}

tm::tm(const tm & other)
: _tm(other._tm)
{
}

tm::tm(int second, int minute, int hour, int day, int month, int year, bool initialize)
: _tm()
{
    if (initialize)
        Update();
    _tm.tm_sec = second;
    _tm.tm_min = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month;
    _tm.tm_year = year;
}

void tm::Update()
{
    TIME_ZONE_INFORMATION timeZoneInformation;
    DWORD result = GetTimeZoneInformation(&timeZoneInformation);
    switch (result)
    {
        case TIME_ZONE_ID_UNKNOWN:
            break;
        case TIME_ZONE_ID_STANDARD:
            OSAL::Strings::strncpy(const_cast<OSAL::Char *>(tm_tzName), timeZoneInformation.StandardName,
                                   MAX_TIME_ZONE_NAME + 1);
            break;
        case TIME_ZONE_ID_DAYLIGHT:
            OSAL::Strings::strncpy(const_cast<OSAL::Char *>(tm_tzName), timeZoneInformation.DaylightName,
                                   MAX_TIME_ZONE_NAME + 1);
            break;
    }
    *(const_cast<long *>(&tm_tzOffset)) = SecondsPerMinute * (timeZoneInformation.Bias + timeZoneInformation.StandardBias);
    *(const_cast<int *>(&tm_dstOffset)) = SecondsPerMinute * timeZoneInformation.DaylightBias;
}

tm & tm::operator=(const tm & other)
{
    if (&other != this)
    {
        _tm = other._tm;
    }
    return *this;
}

tm & tm::operator=(const ::tm & other)
{
    _tm = other;
    return *this;
}

static LARGE_INTEGER GetFileTimeOffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime(int X, struct timespec * tv)
{
    LARGE_INTEGER t;
    FILETIME f;
    double microseconds;
    static LARGE_INTEGER offset{};
    static double frequencyToMicroseconds;
    static bool initialized{};
    static BOOL usePerformanceCounter{};

    if (!tv)
        return EFAULT;
    if (X != CLOCK_REALTIME)
        return EINVAL;
    if (!initialized)
    {
        LARGE_INTEGER performanceFrequency;
        initialized = true;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter)
        {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double) performanceFrequency.QuadPart / 1000000.0;
        } else
        {
            offset = GetFileTimeOffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter)
        QueryPerformanceCounter(&t);
    else
    {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double) t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = LONGLONG(microseconds);
    tv->tv_sec = long(t.QuadPart / 1000000000);
    tv->tv_nsec = long(t.QuadPart % 1000000000);
    time_t timeSeconds;
    timeSeconds = time(nullptr);
    tv->tv_sec += long(timeSeconds);
    return (0);
}

} // namespace Time
} // namespace OSAL
