#pragma once

#include <unit-test-c++/exports.h>
#include <unit-test-c++/ITestReporter.h>
#include <unit-test-c++/TestRegistry.h>

namespace UnitTestCpp
{

class ITestReporter;
class TestInfo;
class TestFixtureInfo;
class TestResults;
class TestSuiteInfo;

struct True
{
    bool operator () (const TestInfo * const ) const { return true; }
    bool operator () (const TestFixtureInfo * const ) const { return true; }
    bool operator () (const TestSuiteInfo * const ) const { return true; }
};

struct UNIT_TEST_CPP_EXPORT InSelection
{
    InSelection(const char * suiteName, const char * fixtureName, const char * testName)
        : suiteName(suiteName)
        , fixtureName(fixtureName)
        , testName(testName)
    {
    }
    bool operator () (const TestInfo * const test) const;
    bool operator () (const TestFixtureInfo * const fixture) const;
    bool operator () (const TestSuiteInfo * const suite) const;

private:
    const char * suiteName;
    const char * fixtureName;
    const char * testName;
};

class UNIT_TEST_CPP_EXPORT TestRunner
{
public:
    TestRunner(const TestRunner &) = delete;
    explicit TestRunner(ITestReporter & reporter);
    ~TestRunner();

    TestRunner & operator = (const TestRunner &) = delete;

    template <class Predicate>
    int RunTestsIf(const TestRegistry & registry, const Predicate & predicate, int maxTestTimeInMs);

private:
    ITestReporter * reporter;
    TestResults * testResults;
    Timer * timer;

    template <class Predicate>
    void Start(const Predicate & predicate) const;

    template <class Predicate>
    int Finish(const Predicate & predicate) const;
};

template <class Predicate>
int TestRunner::RunTestsIf(const TestRegistry & registry,
                           const Predicate & predicate,
                           int maxTestTimeInMs)
{
    Start(predicate);

    TestSuiteInfo * curTestSuite = registry.GetHead();

    while (curTestSuite)
    {
        if (predicate(curTestSuite))
            curTestSuite->RunIf(predicate, maxTestTimeInMs, testResults);

        curTestSuite = curTestSuite->next;
    }

    return Finish(predicate);
}

template <class Predicate>
void TestRunner::Start(const Predicate & predicate) const
{
    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = registry.CountSuitesIf(predicate);
    int numberOfTestFixtures = registry.CountFixturesIf(predicate);
    int numberOfTests = registry.CountTestsIf(predicate);
    this->reporter->ReportTestRunStart(numberOfTestSuites, numberOfTestFixtures, numberOfTests);
    timer->Start();
}

template <class Predicate>
int TestRunner::Finish(const Predicate & predicate) const
{
    int const milliSecondsElapsed = timer->GetTimeInMilliSeconds();
    reporter->ReportTestRunSummary(testResults, milliSecondsElapsed);
    reporter->ReportTestRunOverview(testResults);

    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = registry.CountSuitesIf(predicate);
    int numberOfTestFixtures = registry.CountFixturesIf(predicate);
    int numberOfTests = registry.CountTestsIf(predicate);
    reporter->ReportTestRunFinish(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                  milliSecondsElapsed);

    return testResults->GetFailureCount();
}

} // namespace UnitTestCpp
