#pragma once

#include <unit-test-c++/exports.h>
#include <unit-test-c++/Timer.h>
#include <unit-test-c++/TestDetails.h>

namespace UnitTestCpp
{

class TestResults;
class TestDetails;

class UNIT_TEST_CPP_EXPORT TimeConstraint
{
public:
    TimeConstraint() = delete;
    TimeConstraint(const TimeConstraint &) = delete;
    TimeConstraint(int timeConstraintMilliSeconds, const TestDetails & details);
    ~TimeConstraint();

    TimeConstraint & operator = (const TimeConstraint &) = delete;

private:
    Timer timer;
    TestDetails details;
    int const timeConstraintMilliSeconds;
};

#define UNITTEST_TIME_CONSTRAINT(ms)                                                               \
    UnitTestCpp::TimeConstraint unitTest__timeConstraint__(ms,                                     \
                                                           UnitTestCpp::TestDetails(_details, __LINE__))

#define UNITTEST_TIME_CONSTRAINT_EXEMPT() do { m_timeConstraintExempt = true; } while (0)

} // namespace UnitTestCpp
