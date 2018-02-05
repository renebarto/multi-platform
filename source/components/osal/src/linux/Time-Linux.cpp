#include "osal/Time.h"

#include "osal/Strings.h"
#include <climits>

namespace OSAL {
namespace Time {

long tm::tm_tzOffset                          = 0;
int tm::tm_dstOffset                          = 0;
char tm::tm_tzName[MAX_TIME_ZONE_NAME + 1]    = "";
char tm::tm_tzNameDst[MAX_TIME_ZONE_NAME + 1] = "";
bool tm::initialized                          = false;

static constexpr int SecondsPerMinute = 60;

tm::tm()
    : _tm()
{
    if (!initialized)
        Initialize();
}

tm::tm(const ::tm & other)
    : _tm(_tm)
{
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
    _tm.tm_sec  = second;
    _tm.tm_min  = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon  = month;
    _tm.tm_year = year;
}

void tm::Initialize()
{
    tzset();
    tm_tzOffset  = ::timezone;
    tm_dstOffset = -3600;
    OSAL::Strings::strncpy(tm_tzName, ::tzname[0], MAX_TIME_ZONE_NAME + 1);
    OSAL::Strings::strncpy(tm_tzNameDst, ::tzname[1], MAX_TIME_ZONE_NAME + 1);
    initialized = true;
}
tm & tm::operator = (const tm & other)
{
    if (&other != this)
    {
        _tm = other._tm;
    }
    return *this;
}

tm & tm::operator = (const ::tm & other)
{
    _tm = other;
    return *this;
}

} // namespace Time
} // namespace OSAL
