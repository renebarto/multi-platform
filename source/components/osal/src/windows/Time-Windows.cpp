#include "osal/Time.h"

#include <climits>
#include <direct.h>

namespace OSAL {
namespace Time {

long tm::tm_tzOffset = 0;
int tm::tm_dstOffset = 0;
char tm::tm_tzName[MAX_TIME_ZONE_NAME + 1] = "";
char tm::tm_tzNameDst[MAX_TIME_ZONE_NAME + 1] = "";
bool tm::initialized = false;
static constexpr int SecondsPerMinute = 60;

tm::tm()
    : _tm()
{
    if (!initialized)
        Initialize();
}

tm::tm(const tm & other)
    : _tm(other._tm)
{
}

tm::tm(int second, int minute, int hour, int day, int month, int year)
    : _tm()
{
    if (!initialized)
        Initialize();
    _tm.tm_sec = second;
    _tm.tm_min = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month;
    _tm.tm_year = year;
}

void tm::Initialize()
{
    TIME_ZONE_INFORMATION timeZoneInformation;
    /* DWORD result = */ GetTimeZoneInformation(&timeZoneInformation);
    tm_tzOffset= SecondsPerMinute * (timeZoneInformation.Bias + timeZoneInformation.StandardBias);
    tm_dstOffset= SecondsPerMinute * timeZoneInformation.DaylightBias;
    OSAL::Strings::strncpy(tm_tzName, OSAL::Strings::WStringToString(timeZoneInformation.StandardName).c_str(), MAX_TIME_ZONE_NAME + 1);
    OSAL::Strings::strncpy(tm_tzNameDst, OSAL::Strings::WStringToString(timeZoneInformation.DaylightName).c_str(), MAX_TIME_ZONE_NAME + 1);
    initialized = true;
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

int clock_gettime(int X, class timespec * tv)
{
    static LARGE_INTEGER _offsetAtInitialization{};
    static double _frequencyToNanoseconds;
    static bool _initialized{};
    static BOOL _usePerformanceCounter{};
    static time_t _timeAtInitialization;

    if (!tv)
        return EFAULT;
    if (X != CLOCK_REALTIME)
        return EINVAL;
    if (!_initialized)
    {
        LARGE_INTEGER performanceFrequency;
        _initialized = true;
        _usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (_usePerformanceCounter)
        {
            QueryPerformanceCounter(&_offsetAtInitialization);
            _frequencyToNanoseconds = (double) performanceFrequency.QuadPart / 1000000000.0;
        } else
        {
            _offsetAtInitialization = GetFileTimeOffset();
            _frequencyToNanoseconds = 10000.;
        }
        _timeAtInitialization = time(nullptr);
    }
    LARGE_INTEGER t;
    if (_usePerformanceCounter)
        QueryPerformanceCounter(&t);
    else
    {
        FILETIME f;
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= _offsetAtInitialization.QuadPart;
    double nanoseconds = (double) t.QuadPart / _frequencyToNanoseconds;
    t.QuadPart = LONGLONG(nanoseconds);
    tv->tv_sec = long(t.QuadPart / 1000000000);
    tv->tv_nsec = long(t.QuadPart % 1000000000);
    tv->tv_sec += long(_timeAtInitialization);
    return (0);
}

} // namespace Time
} // namespace OSAL
