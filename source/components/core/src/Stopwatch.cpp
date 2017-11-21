#include "core/Stopwatch.h"

#include <cstring>
#include <iomanip>
#include <sstream>
#include "osal/Time.h"

using namespace std;
using namespace Core;

Stopwatch::Stopwatch()
{
    memset(&startTime, 0, sizeof(startTime));
    memset(&lapTime, 0, sizeof(lapTime));

    if (OSAL::Time::clock_getres(CLOCK_REALTIME, &freq) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::Start()
{
    if (OSAL::Time::clock_gettime(CLOCK_REALTIME, &startTime) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void Stopwatch::Lap()
{
    if (OSAL::Time::clock_gettime(CLOCK_REALTIME, &lapTime) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

OSAL::Time::timespec Stopwatch::GetStartTime()
{
    return startTime;
}

OSAL::Time::timespec Stopwatch::GetLapTime()
{
    return lapTime;
}

OSAL::Time::timespec Stopwatch::GetResolution()
{
    return freq;
}

double Stopwatch::GetElapsedTime()
{
    double diffTime = 0;

    diffTime = (double)(lapTime.tv_sec - startTime.tv_sec) +
               1e-9 * (lapTime.tv_nsec - startTime.tv_nsec);
    return diffTime;
}

std::ostream & Stopwatch::PrintTo(std::ostream & stream) const
{
    stream << "start = " << startTime.tv_sec << "." << setfill('0') << setw(9) << startTime.tv_nsec
           << " lap = " << lapTime.tv_sec << "." << setfill('0') << setw(9) << lapTime.tv_nsec;
    return stream;
}
