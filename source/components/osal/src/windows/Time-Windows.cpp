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

tm::tm(const ::tm & other, bool localTime)
    : _tm(other)
{
    if (!initialized)
        Initialize();
    if (!IsEmpty())
    {
        time_t epochTime = mktime(this);
        _tm = localTime ? *::localtime(&epochTime) : *::gmtime(&epochTime);
    }
}

tm::tm(const tm & other)
    : _tm(other._tm)
{
}

tm::tm(int second, int minute, int hour, int day, int month, int year, bool localTime)
    : _tm()
{
    if (!initialized)
        Initialize();
    _tm.tm_sec = second;
    _tm.tm_min = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month - 1;
    _tm.tm_year = year - 1900;
    time_t epochTime = mktime(this);
    if (!localTime)
        epochTime -= ActiveLocalTimeOffsetSeconds();
    else
        epochTime += ActiveDaylightSavingsTimeOffsetSeconds();
    _tm = localTime ? *::localtime(&epochTime) : *::gmtime(&epochTime);
}

void tm::Initialize()
{
    TIME_ZONE_INFORMATION timeZoneInformation;
    /* DWORD result = */ GetTimeZoneInformation(&timeZoneInformation);
    tm_tzOffset= SecondsPerMinute * (timeZoneInformation.Bias + timeZoneInformation.StandardBias);
    tm_dstOffset= SecondsPerMinute * timeZoneInformation.DaylightBias;
    std::string tmpName = OSAL::Strings::WStringToString(timeZoneInformation.StandardName);
    OSAL::Strings::strncpy(tm_tzName, tmpName.c_str(), MAX_TIME_ZONE_NAME + 1);
    tm_tzName[MAX_TIME_ZONE_NAME] = '\0';
    tmpName = OSAL::Strings::WStringToString(timeZoneInformation.DaylightName);
    OSAL::Strings::strncpy(tm_tzNameDst, tmpName.c_str(), MAX_TIME_ZONE_NAME + 1);
    tm_tzNameDst[MAX_TIME_ZONE_NAME] = '\0';
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

int ClockGetTime(clockid_t clockID, OSAL::Time::timespec * tp)
{
    LARGE_INTEGER t;
    FILETIME f;
    double microseconds;
    static LARGE_INTEGER offset{};
    static double frequencyToMicroseconds;
    static bool initialized{};
    static BOOL usePerformanceCounter{};

    if (!tp)
        return EFAULT;
    if (clockID != CLOCK_REALTIME)
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
            frequencyToMicroseconds = 10.0;
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
    tp->tv_sec = long(t.QuadPart / 1000000000);
    tp->tv_nsec = long(t.QuadPart % 1000000000);
    time_t timeSeconds;
    timeSeconds = time(nullptr);
    tp->tv_sec += long(timeSeconds);
    return (0);
}

} // namespace Time
} // namespace OSAL
