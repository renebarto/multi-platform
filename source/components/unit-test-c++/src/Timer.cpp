#include <unit-test-c++/Timer.h>

namespace UnitTestCpp
{

Timer::Timer()
{
    memset(&startTime, 0, sizeof(startTime));
}

void Timer::Start()
{
    OSAL::Time::GetTimeOfDay(&startTime, nullptr);
}

int Timer::GetTimeInMilliSeconds() const
{
    timeval currentTime;
	OSAL::Time::GetTimeOfDay(&currentTime, nullptr);
    int const differenceSeconds = currentTime.tv_sec - startTime.tv_sec;
    int const differenceMicroSeconds = currentTime.tv_usec - startTime.tv_usec;
    return differenceSeconds * 1000 + differenceMicroSeconds / 1000;
}

void TimeHelpers::SleepMilliSeconds(int ms)
{
    OSAL::Time::USleep(ms * 1000);
}

} // namespace UnitTestCpp
