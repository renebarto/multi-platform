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
    virtual void ReportTestSuiteStart(const std::string & UNUSED(suiteName),
                                      int UNUSED(numberOfTestFixtures))
    {
    }
    virtual void ReportTestSuiteFinish(const std::string & UNUSED(suiteName),
                                       int UNUSED(numberOfTestFixtures),
                                       int UNUSED(milliSecondsElapsed))
    {
    }
    virtual void ReportTestFixtureStart(const std::string & UNUSED(fixtureName),
                                        int UNUSED(numberOfTests))
    {
    }
    virtual void ReportTestFixtureFinish(const std::string & UNUSED(fixtureName),
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
    virtual void ReportTestFailure(const TestDetails & details, const std::string & failure)
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
    std::string lastStartedSuite;
    std::string lastStartedFixture;
    std::string lastStartedTest;

    int testFailedCount;
    std::string lastFailedFile;
    int lastFailedLine;
    std::string lastFailedSuite;
    std::string lastFailedFixture;
    std::string lastFailedTest;
    std::string lastFailedMessage;

    int testFinishedCount;
    std::string lastFinishedSuite;
    std::string lastFinishedFixture;
    std::string lastFinishedTest;
    int lastFinishedTestTime;

    int summaryTotalTestCount;
    int summaryFailedTestCount;
    int summaryFailureCount;
    int summarySecondsElapsed;
};

} // namespace _Test

} // namespace UnitTestCpp

