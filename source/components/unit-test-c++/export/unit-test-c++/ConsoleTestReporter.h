#pragma once

#include <string>
#include <osal/Console.h>
#include <unit-test-c++/exports.h>
#include <unit-test-c++/StreamTestReporter.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT ConsoleTestReporter : public StreamTestReporter
{
public:
    ConsoleTestReporter()
        : StreamTestReporter(std::cout)
        , console()
    { }

private:
	OSAL::Console console;

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
    virtual void ReportTestStart(const TestDetails & test) override;
    virtual void ReportTestFinish(const TestDetails & test, bool success,
                                  int milliSecondsElapsed) override;
    virtual void ReportTestFailure(const TestDetails & test, const std::string & failure) override;
};

} // namespace UnitTestCpp
