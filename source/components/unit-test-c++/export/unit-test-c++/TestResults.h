#pragma once

#include <string>
#include <vector>
#include <unit-test-c++/exports.h>

namespace UnitTestCpp
{

class ITestReporter;
class TestDetails;
class TestSuiteInfo;
class TestFixtureInfo;

class UNIT_TEST_CPP_EXPORT TestResults
{
public:
    TestResults(const TestResults &) = delete;
    explicit TestResults(ITestReporter* reporter = nullptr);
    virtual ~TestResults();

    TestResults & operator = (const TestResults &) = delete;

    void OnTestSuiteStart(TestSuiteInfo * suiteInfo);
    void OnTestSuiteFinish(TestSuiteInfo * suiteInfo, int milliSecondsElapsed);
    void OnTestFixtureStart(TestFixtureInfo * fixtureInfo);
    void OnTestFixtureFinish(TestFixtureInfo * fixtureInfo, int milliSecondsElapsed);
    void OnTestStart(const TestDetails & details);
    void OnTestFailure(const TestDetails & details, std::string message);
    void OnTestFinish(const TestDetails & details, int milliSecondsElapsed);

    int GetTotalTestCount() const;
    int GetFailedTestCount() const;
    int GetFailureCount() const;

private:
    ITestReporter * reporter;
    int totalTestCount;
    int failedTestCount;
    int failureCount;
    bool currentTestFailed;
};

} // namespace UnitTestCpp
