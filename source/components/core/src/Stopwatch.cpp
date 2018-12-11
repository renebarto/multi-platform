#include "core/Stopwatch.h"

#include "osal/Exception.h"

using namespace std;
using namespace Core;

Stopwatch::Stopwatch()
{
    memset(&_startTime, 0, sizeof(_startTime));
    memset(&_lapTime, 0, sizeof(_lapTime));

    if (OSAL::Time::clock_getres(CLOCK_REALTIME, &_freq) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::Start()
{
    if (OSAL::Time::clock_gettime(CLOCK_REALTIME, &_startTime) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void Stopwatch::Lap()
{
    if (OSAL::Time::clock_gettime(CLOCK_REALTIME, &_lapTime) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

OSAL::Time::timespec Stopwatch::GetStartTime()
{
    return _startTime;
}

OSAL::Time::timespec Stopwatch::GetLapTime()
{
    return _lapTime;
}

OSAL::Time::timespec Stopwatch::GetResolution()
{
    return _freq;
}

double Stopwatch::GetElapsedTime()
{
    double diffTime = 0;

    diffTime = (double)(_lapTime.tv_sec - _startTime.tv_sec) +
               1e-9 * (_lapTime.tv_nsec - _startTime.tv_nsec);
    return diffTime;
}
