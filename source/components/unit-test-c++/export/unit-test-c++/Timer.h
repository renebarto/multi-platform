#pragma once

#include <osal/OSAL.h>
#include <unit-test-c++/Exports.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT Timer
{
public:
    Timer();
    void Start();
    int GetTimeInMilliSeconds() const;

private:
    timeval startTime;
};

namespace TimeHelpers
{
    UNIT_TEST_CPP_EXPORT void SleepMilliSeconds(int ms);
} // namespace TimeHelpers

} // namespace UnitTestCpp

