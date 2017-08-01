#pragma once

#include <iostream>
#include <string>
#include <unit-test-c++/exports.h>
#include <unit-test-c++/DeferredTestReporter.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT StreamTestReporter : public DeferredTestReporter
{
public:
    static const OSAL::String TestRunSeparator;
    static const OSAL::String TestFixtureSeparator;
    static const OSAL::String TestSuiteSeparator;
    static const OSAL::String TestSuccessSeparator;
    static const OSAL::String TestFailSeparator;

    StreamTestReporter(std::basic_ostream<OSAL::Char> & stream)
        : _stream(stream)
    { }

protected:
    virtual void ReportTestRunStart(int numberOfTestSuites,
                                    int numberOfTestFixtures,
                                    int numberOfTests) override;
    virtual void ReportTestRunFinish(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests,
                                     int milliSecondsElapsed) override;
    virtual void ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed) override;
    virtual void ReportTestRunOverview(const TestResults * results) override;
    virtual void ReportTestSuiteStart(const OSAL::String & suiteName,
                                      int numberOfTestFixtures) override;
    virtual void ReportTestSuiteFinish(const OSAL::String & suiteName,
                                       int numberOfTestFixtures,
                                       int milliSecondsElapsed) override;
    virtual void ReportTestFixtureStart(const OSAL::String & fixtureName,
                                        int numberOfTests) override;
    virtual void ReportTestFixtureFinish(const OSAL::String & fixtureName,
                                         int numberOfTests,
                                         int milliSecondsElapsed) override;
    virtual void ReportTestStart(const TestDetails & test) override;
    virtual void ReportTestFinish(const TestDetails & test, bool success,
                                  int milliSecondsElapsed) override;
    virtual void ReportTestFailure(const TestDetails & test, const OSAL::String & failure) override;

    OSAL::String TestRunStartMessage(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests);
    OSAL::String TestRunFinishMessage(int numberOfTestSuites,
                                      int numberOfTestFixtures,
                                      int numberOfTests,
                                      int milliSecondsElapsed);
    OSAL::String TestSuiteStartMessage(const OSAL::String & suiteName,
                                       int numberOfTestFixtures);
    OSAL::String TestSuiteFinishMessage(const OSAL::String & suiteName,
                                        int numberOfTestFixtures,
                                        int milliSecondsElapsed);
    OSAL::String TestFixtureStartMessage(const OSAL::String & fixtureName,
                                         int numberOfTests);
    OSAL::String TestFixtureFinishMessage(const OSAL::String & fixtureName,
                                          int numberOfTests,
                                          int milliSecondsElapsed);
    OSAL::String TestFinishMessage(const TestDetails & test, bool success,
                                   int milliSecondsElapsed);
    OSAL::String TestRunSummaryMessage(const TestResults * results, int milliSecondsElapsed);
    OSAL::String TestRunOverviewMessage(const TestResults * results);
    OSAL::String TestName(const OSAL::String & suiteName,
                          const OSAL::String & fixtureName,
                          const OSAL::String & testName);

    std::basic_ostream<OSAL::Char> & _stream;
};

} // namespace UnitTestCpp
