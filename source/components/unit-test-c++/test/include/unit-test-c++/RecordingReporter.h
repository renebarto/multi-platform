#pragma once

#include <string>
#include <unit-test-c++/ITestReporter.h>
#include <unit-test-c++/TestDetails.h>

namespace UnitTestCpp
{

namespace _Test
{

struct RecordingReporter : public ITestReporter
{
public:
    RecordingReporter() :
        testRunCount(0),
        lastStartedSuite(),
        lastStartedFixture(),
        lastStartedTest(),
        testFailedCount(0),
        lastFailedFile(),
        lastFailedLine(0),
        lastFailedSuite(),
        lastFailedFixture(),
        lastFailedTest(),
        lastFailedMessage(),
        testFinishedCount(0),
        lastFinishedSuite(),
        lastFinishedFixture(),
        lastFinishedTest(),
        lastFinishedTestTime(0),
        summaryTotalTestCount(0),
        summaryFailedTestCount(0),
        summaryFailureCount(0),
        summarySecondsElapsed(0)
    {
    }

    void ReportTestRunStart(int UNUSED(numberOfTestSuites),
                            int UNUSED(numberOfTestFixtures),
                            int UNUSED(numberOfTests))
    {
    }
    void ReportTestRunFinish(int UNUSED(numberOfTestSuites),
                             int UNUSED(numberOfTestFixtures),
                             int UNUSED(numberOfTests),
                             int UNUSED(milliSecondsElapsed))
    {
    }
    void ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
    {
        this->summaryTotalTestCount = results->GetTotalTestCount();
        this->summaryFailedTestCount = results->GetFailedTestCount();
        this->summaryFailureCount = results->GetFailureCount();
        this->summarySecondsElapsed = milliSecondsElapsed;
    }
    void ReportTestRunOverview(const TestResults * UNUSED(results))
    {
    }
    virtual void ReportTestSuiteStart(const OSAL::String & UNUSED(suiteName),
                                      int UNUSED(numberOfTestFixtures))
    {
    }
    virtual void ReportTestSuiteFinish(const OSAL::String & UNUSED(suiteName),
                                       int UNUSED(numberOfTestFixtures),
                                       int UNUSED(milliSecondsElapsed))
    {
    }
    virtual void ReportTestFixtureStart(const OSAL::String & UNUSED(fixtureName),
                                        int UNUSED(numberOfTests))
    {
    }
    virtual void ReportTestFixtureFinish(const OSAL::String & UNUSED(fixtureName),
                                         int UNUSED(numberOfTests),
                                         int UNUSED(milliSecondsElapsed))
    {
    }
    virtual void ReportTestStart(const TestDetails & details)
    {
        ++testRunCount;
        lastStartedSuite = details.suiteName;
        lastStartedFixture = details.fixtureName;
        lastStartedTest = details.testName;
    }
    virtual void ReportTestFinish(const TestDetails & details,
                                  bool UNUSED(success),
                                  int milliSecondsElapsed)
    {
        ++testFinishedCount;
        lastFinishedSuite = details.suiteName;
        lastFinishedFixture = details.fixtureName;
        lastFinishedTest = details.testName;
        lastFinishedTestTime = milliSecondsElapsed;
    }
    virtual void ReportTestFailure(const TestDetails & details, const OSAL::String & failure)
    {
        ++testFailedCount;
        lastFailedFile = details.fileName;
        lastFailedLine = details.lineNumber;
        lastFailedSuite = details.suiteName;
        lastFailedFixture = details.fixtureName;
        lastFailedTest = details.testName;
        lastFailedMessage = failure;
    }

    int testRunCount;
    OSAL::String lastStartedSuite;
    OSAL::String lastStartedFixture;
    OSAL::String lastStartedTest;

    int testFailedCount;
    OSAL::String lastFailedFile;
    int lastFailedLine;
    OSAL::String lastFailedSuite;
    OSAL::String lastFailedFixture;
    OSAL::String lastFailedTest;
    OSAL::String lastFailedMessage;

    int testFinishedCount;
    OSAL::String lastFinishedSuite;
    OSAL::String lastFinishedFixture;
    OSAL::String lastFinishedTest;
    int lastFinishedTestTime;

    int summaryTotalTestCount;
    int summaryFailedTestCount;
    int summaryFailureCount;
    int summarySecondsElapsed;
};

} // namespace _Test

} // namespace UnitTestCpp

