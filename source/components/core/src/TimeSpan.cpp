#include "core/TimeSpan.h"
#include <iomanip>
#include <sstream>
#include <osal/OSAL.h>

using namespace std;
using namespace Core;

TimeSpan::TimeSpan()
    : _interval(0)
{
}

TimeSpan::TimeSpan(int64_t interval)
    : _interval(interval)
{
}

TimeSpan::TimeSpan(const TimeSpan & other)
    : _interval(other._interval)
{
}

TimeSpan::~TimeSpan()
{
}

TimeSpan & TimeSpan::operator = (int64_t interval)
{
    this->_interval = interval;
    return *this;
}

TimeSpan & TimeSpan::operator = (const TimeSpan & other)
{
    if (this != &other)
    {
        this->_interval = other._interval;
    }
    return *this;
}

bool Core::operator == (const TimeSpan & lhs, const TimeSpan & rhs)
{
    return lhs._interval == rhs._interval;
}

bool Core::operator == (int64_t lhs, const TimeSpan & rhs)
{
    return lhs == rhs._interval;
}

bool Core::operator == (const TimeSpan & lhs, int64_t rhs)
{
    return lhs._interval == rhs;
}

bool Core::operator != (const TimeSpan & lhs, const TimeSpan & rhs)
{
    return lhs._interval != rhs._interval;
}

bool Core::operator != (int64_t lhs, const TimeSpan & rhs)
{
    return lhs != rhs._interval;
}

bool Core::operator != (const TimeSpan & lhs, int64_t rhs)
{
    return lhs._interval != rhs;
}

TimeSpan::operator int64_t () const
{
    return _interval;
}

int64_t TimeSpan::NanoSeconds() const
{
    return _interval;
}

int64_t TimeSpan::MicroSeconds() const
{
    return _interval / NanoSecondsPerMicroSecond;
}

int64_t TimeSpan::MilliSeconds() const
{
    return _interval / NanoSecondsPerMilliSecond;
}

double TimeSpan::Seconds() const
{
    return static_cast<double>(_interval) / NanoSecondsPerSecond;
}

int TimeSpan::Minutes() const
{
    return static_cast<int>(_interval / NanoSecondsPerMinute);
}

int TimeSpan::Hours() const
{
    return static_cast<int>(_interval / NanoSecondsPerHour);
}

int TimeSpan::Days() const
{
    return static_cast<int>(_interval / NanoSecondsPerDay);
}


OSAL::String TimeSpan::ToString() const
{
    int days = static_cast<int>(_interval / NanoSecondsPerDay);
    int hours = (_interval / NanoSecondsPerHour) % HoursPerDay;
    int minutes = (_interval / NanoSecondsPerMinute) % MinutesPerHour;
    int seconds = (_interval / NanoSecondsPerSecond) % SecondsPerMinute;
    int microSeconds = (_interval / NanoSecondsPerMicroSecond) % MicroSecondsPerSecond;
    basic_stringstream<OSAL::Char> stream;
    stream << days << _(" days, ") << hours << _(" hours, ")
           << minutes << _(" minutes, ")
           << seconds << _(".") << setfill(_('0')) << setw(6) << microSeconds << _(" seconds");
    return stream.str();
}
