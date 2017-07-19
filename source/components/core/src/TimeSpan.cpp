#include <iomanip>
#include <sstream>
#include "core/TimeSpan.h"

using namespace std;
using namespace Core;

const int64_t TimeSpan::NanoSecondsPerMicroSecond = 1000;
const int64_t TimeSpan::NanoSecondsPerMilliSecond = 1000 * TimeSpan::NanoSecondsPerMicroSecond;
const int64_t TimeSpan::NanoSecondsPerSecond = 1000 * TimeSpan::NanoSecondsPerMilliSecond;
const int64_t TimeSpan::MicroSecondsPerSecond = 1000000;
const int64_t TimeSpan::NanoSecondsPerMinute = 60 * TimeSpan::NanoSecondsPerSecond;
const int64_t TimeSpan::NanoSecondsPerHour = 60 * TimeSpan::NanoSecondsPerMinute;
const int64_t TimeSpan::NanoSecondsPerDay = 24 * TimeSpan::NanoSecondsPerHour;

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
    return (double)_interval / NanoSecondsPerSecond;
}

int TimeSpan::Minutes() const
{
    return _interval / NanoSecondsPerMinute;
}

int TimeSpan::Hours() const
{
    return _interval / NanoSecondsPerHour;
}

int TimeSpan::Days() const
{
    return _interval / NanoSecondsPerDay;
}


string TimeSpan::ToString() const
{
    int days = _interval / NanoSecondsPerDay;
    int hours = (_interval / NanoSecondsPerHour) % 24;
    int minutes = (_interval / NanoSecondsPerMinute) % 60;
    int seconds = (_interval / NanoSecondsPerSecond) % 60;
    int microSeconds = (_interval / NanoSecondsPerMicroSecond) % MicroSecondsPerSecond;
    stringstream stream;
    stream << days << " days, " << hours << " hours, "
           << minutes << " minutes, "
           << seconds << "." << setfill('0') << setw(6) << microSeconds << " seconds";
    return stream.str();
}
