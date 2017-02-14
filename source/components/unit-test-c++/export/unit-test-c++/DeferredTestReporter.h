#pragma once

#include <vector>
#include <unit-test-c++/exports.h>
#include <unit-test-c++/ITestReporter.h>
#include <unit-test-c++/TestDetailedResult.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT DeferredTestReporter : public ITestReporter
{
public:
    virtual void ReportTestRunStart(int numberOfTestSuites,
                                    int numberOfTestFixtures,
                                    int numberOfTests) override;
    virtual void ReportTestRunFinish(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests,
                                     int milliSecondsElapsed) override;
    virtual void ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed) override;
    virtual void ReportTestRunOverview(const TestResults * results) override;
    virtual void ReportTestSuiteStart(const std::string & suiteName,
                                      int numberOfTestFixtures) override;
    virtual void ReportTestSuiteFinish(const std::string & suiteName,
                                       int numberOfTestFixtures,
                                       int milliSecondsElapsed) override;
    virtual void ReportTestFixtureStart(const std::string & fixtureName,
                                        int numberOfTests) override;
    virtual void ReportTestFixtureFinish(const std::string & fixtureName,
                                         int numberOfTests,
                                         int milliSecondsElapsed) override;
    virtual void ReportTestStart(const TestDetails & details) override;
    virtual void ReportTestFinish(const TestDetails & details, bool success,
                                  int milliSecondsElapsed) override;
    virtual void ReportTestFailure(const TestDetails & details, const std::string & failure) override;

    using ResultList = std::vector<TestDetailedResult>;
    ResultList & Results();

private:
    ResultList _results;
};

} // namespace UnitTestCpp
