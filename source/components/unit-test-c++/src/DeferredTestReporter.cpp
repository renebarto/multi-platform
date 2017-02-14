#include <unit-test-c++/DeferredTestReporter.h>

#include <unit-test-c++/TestDetails.h>

#include <osal/unused.h>

using namespace std;

namespace UnitTestCpp
{

void DeferredTestReporter::ReportTestRunStart(int UNUSED(numberOfTestSuites),
                                              int UNUSED(numberOfTestFixtures),
                                              int UNUSED(numberOfTests))
{
}

void DeferredTestReporter::ReportTestRunFinish(int UNUSED(numberOfTestSuites),
                                               int UNUSED(numberOfTestFixtures),
                                               int UNUSED(numberOfTests),
                                               int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestRunSummary(const TestResults * UNUSED(_results),
                                                int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestRunOverview(const TestResults * UNUSED(_results))
{
}

void DeferredTestReporter::ReportTestSuiteStart(const string & UNUSED(suiteName),
                                                int UNUSED(numberOfTestFixtures))
{
}

void DeferredTestReporter::ReportTestSuiteFinish(const string & UNUSED(suiteName),
                                                 int UNUSED(numberOfTests),
                                                 int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestFixtureStart(const string & UNUSED(fixtureName),
                                                  int UNUSED(numberOfTests))
{
}

void DeferredTestReporter::ReportTestFixtureFinish(const string & UNUSED(fixtureName),
                                                   int UNUSED(numberOfTests),
                                                   int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestStart(const TestDetails & details)
{
    _results.push_back(TestDetailedResult(details));
}

void DeferredTestReporter::ReportTestFinish(const TestDetails & UNUSED(details),
                                            bool UNUSED(success),
                                            int milliSecondsElapsed)
{
    TestDetailedResult & result = _results.back();
    result.MilliSecondsElapsed(milliSecondsElapsed);
}

void DeferredTestReporter::ReportTestFailure(const TestDetails & details, const string & failure)
{
    TestDetailedResult & result = _results.back();
    result.AddFailure(TestDetailedResult::Failure(details.lineNumber, failure));
}

DeferredTestReporter::ResultList & DeferredTestReporter::Results()
{
    return _results;
}

} // namespace UnitTestCpp
