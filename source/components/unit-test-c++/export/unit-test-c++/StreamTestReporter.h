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
    static const std::string TestRunSeparator;
    static const std::string TestFixtureSeparator;
    static const std::string TestSuiteSeparator;
    static const std::string TestSuccessSeparator;
    static const std::string TestFailSeparator;

    StreamTestReporter(std::ostream & stream)
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

    std::string TestRunStartMessage(int numberOfTestSuites,
                                    int numberOfTestFixtures,
                                    int numberOfTests);
    std::string TestRunFinishMessage(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests,
                                     int milliSecondsElapsed);
    std::string TestSuiteStartMessage(const std::string & suiteName,
                                      int numberOfTestFixtures);
    std::string TestSuiteFinishMessage(const std::string & suiteName,
                                       int numberOfTestFixtures,
                                       int milliSecondsElapsed);
    std::string TestFixtureStartMessage(const std::string & fixtureName,
                                        int numberOfTests);
    std::string TestFixtureFinishMessage(const std::string & fixtureName,
                                         int numberOfTests,
                                         int milliSecondsElapsed);
    std::string TestFinishMessage(const TestDetails & test, bool success,
                                  int milliSecondsElapsed);
    std::string TestRunSummaryMessage(const TestResults * results, int milliSecondsElapsed);
    std::string TestRunOverviewMessage(const TestResults * results);
    std::string TestName(const std::string & suiteName,
                         const std::string & fixtureName,
                         const std::string & testName);

    std::ostream & _stream;
};

} // namespace UnitTestCpp
