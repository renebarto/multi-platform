#pragma once

#include <string>
#include <osal/Strings.h>

namespace UnitTestCpp
{

class TestDetails;
class TestResults;

class ITestReporter
{
public:
    virtual ~ITestReporter() {}

    virtual void ReportTestRunStart(int numberOfTestSuites, 
                                    int numberOfTestFixtures, 
                                    int numberOfTests) = 0;
    virtual void ReportTestRunFinish(int numberOfTestSuites, 
                                     int numberOfTestFixtures, 
                                     int numberOfTests, 
                                     int milliSecondsElapsed) = 0;
    virtual void ReportTestRunSummary(const TestResults * results,
                                      int milliSecondsElapsed) = 0;
    virtual void ReportTestRunOverview(const TestResults * results) = 0;
    virtual void ReportTestSuiteStart(const OSAL::String & suiteName,
                                      int numberOfTests) = 0;
    virtual void ReportTestSuiteFinish(const OSAL::String & suiteName,
                                       int numberOfTests, 
                                       int milliSecondsElapsed) = 0;
    virtual void ReportTestFixtureStart(const OSAL::String & fixtureName,
                                        int numberOfTests) = 0;
    virtual void ReportTestFixtureFinish(const OSAL::String & fixtureName,
                                         int numberOfTests, 
                                         int milliSecondsElapsed) = 0;
    virtual void ReportTestStart(const TestDetails & details) = 0;
    virtual void ReportTestFinish(const TestDetails & details,
                                  bool success, 
                                  int milliSecondsElapsed) = 0;
    virtual void ReportTestFailure(const TestDetails & details,
                                   const OSAL::String & failure) = 0;
};

} // namespace UnitTestCpp
