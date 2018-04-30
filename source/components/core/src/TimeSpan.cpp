#include <iomanip>
#include <sstream>
#include "core/TimeSpan.h"

using namespace std;
using namespace Core;

TimeSpan::TimeSpan()
    : _intervalNanoSeconds(0)
{
}

TimeSpan::TimeSpan(int64_t intervalNanoSeconds)
    : _intervalNanoSeconds(intervalNanoSeconds)
{
}

TimeSpan::TimeSpan(const TimeSpan & other)
    : _intervalNanoSeconds(other._intervalNanoSeconds)
{
}

TimeSpan::~TimeSpan()
{
}

TimeSpan & TimeSpan::operator = (int64_t intervalNanoSeconds)
{
    this->_intervalNanoSeconds = intervalNanoSeconds;
    return *this;
}

TimeSpan & TimeSpan::operator = (const TimeSpan & other)
{
    if (this != &other)
    {
        this->_intervalNanoSeconds = other._intervalNanoSeconds;
    }
    return *this;
}

bool Core::operator == (const TimeSpan & lhs, const TimeSpan & rhs)
{
    return lhs._intervalNanoSeconds == rhs._intervalNanoSeconds;
}

bool Core::operator == (int64_t intervalNanoSeconds, const TimeSpan & rhs)
{
    return intervalNanoSeconds == rhs._intervalNanoSeconds;
}

bool Core::operator == (const TimeSpan & lhs, int64_t intervalNanoSeconds)
{
    return lhs._intervalNanoSeconds == intervalNanoSeconds;
}

bool Core::operator != (const TimeSpan & lhs, const TimeSpan & rhs)
{
    return lhs._intervalNanoSeconds != rhs._intervalNanoSeconds;
}

bool Core::operator != (int64_t intervalNanoSeconds, const TimeSpan & rhs)
{
    return intervalNanoSeconds != rhs._intervalNanoSeconds;
}

bool Core::operator != (const TimeSpan & lhs, int64_t intervalNanoSeconds)
{
    return lhs._intervalNanoSeconds != intervalNanoSeconds;
}

TimeSpan::operator int64_t () const
{
    return _intervalNanoSeconds;
}

int64_t TimeSpan::NanoSeconds() const
{
    return _intervalNanoSeconds;
}

int64_t TimeSpan::MicroSeconds() const
{
    return _intervalNanoSeconds / NanoSecondsPerMicroSecond;
}

int64_t TimeSpan::MilliSeconds() const
{
    return _intervalNanoSeconds / NanoSecondsPerMilliSecond;
}

double TimeSpan::Seconds() const
{
    return static_cast<double>(_intervalNanoSeconds) / NanoSecondsPerSecond;
}

int TimeSpan::Minutes() const
{
    return static_cast<int>(_intervalNanoSeconds / NanoSecondsPerMinute);
}

int TimeSpan::Hours() const
{
    return static_cast<int>(_intervalNanoSeconds / NanoSecondsPerHour);
}

int TimeSpan::Days() const
{
    return static_cast<int>(_intervalNanoSeconds / NanoSecondsPerDay);
}

void TimeSpan::PrintTo(std::ostream & stream) const
{
    int days = Days();
    int hours = Hours() % 24;
    int minutes = Minutes() % 60;
    int seconds = (_intervalNanoSeconds / NanoSecondsPerSecond) % 60;
    int microSeconds = MicroSeconds() % MicroSecondsPerSecond;
    stream << days << " days, " << hours << " hours, "
           << minutes << " minutes, "
           << seconds << "." << setfill('0') << setw(6) << microSeconds << " seconds";
}
