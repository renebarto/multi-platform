#include <unit-test-c++/StreamTestReporter.h>

#include <cstdio>
#include <sstream>
#include <iomanip>
#include <unit-test-c++/TestDetails.h>
#include <unit-test-c++/TestDetailedResult.h>
#include <unit-test-c++/TestResults.h>

#include <osal/Unused.h>

using namespace std;

namespace UnitTestCpp
{

const OSAL::String StreamTestReporter::TestRunSeparator =     _("[===========]");
const OSAL::String StreamTestReporter::TestSuiteSeparator =   _("[   SUITE   ]");
const OSAL::String StreamTestReporter::TestFixtureSeparator = _("[  FIXTURE  ]");
const OSAL::String StreamTestReporter::TestSuccessSeparator = _("[ SUCCEEDED ]");
const OSAL::String StreamTestReporter::TestFailSeparator =    _("[  FAILED   ]");

void StreamTestReporter::ReportTestRunStart(int numberOfTestSuites,
                                            int numberOfTestFixtures,
                                            int numberOfTests)
{
    _stream << TestRunSeparator << _(" ")
            << TestRunStartMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests) << endl;
}

void StreamTestReporter::ReportTestRunFinish(int numberOfTestSuites,
                                             int numberOfTestFixtures,
                                             int numberOfTests,
                                             int milliSecondsElapsed)
{
    _stream << TestRunSeparator << _(" ")
            << TestRunFinishMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                    milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    _stream << TestRunSummaryMessage(results, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestRunOverview(const TestResults * results)
{
    _stream << TestRunOverviewMessage(results) << endl;
}

void StreamTestReporter::ReportTestSuiteStart(const OSAL::String & suiteName,
                                              int numberOfTestFixtures)
{
    _stream << TestSuiteSeparator << _(" ")
            << TestSuiteStartMessage(suiteName, numberOfTestFixtures) << endl;
}

void StreamTestReporter::ReportTestSuiteFinish(const OSAL::String & suiteName,
                                               int numberOfTestFixtures,
                                               int milliSecondsElapsed)
{
    _stream << TestSuiteSeparator << _(" ")
            << TestSuiteFinishMessage(suiteName, numberOfTestFixtures, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestFixtureStart(const OSAL::String & fixtureName,
                                                int numberOfTests)
{
    _stream << TestFixtureSeparator << _(" ")
            << TestFixtureStartMessage(fixtureName, numberOfTests) << endl;
}

void StreamTestReporter::ReportTestFixtureFinish(const OSAL::String & fixtureName,
                                                 int numberOfTests,
                                                 int milliSecondsElapsed)
{
    _stream << TestFixtureSeparator << _(" ")
			<< TestFixtureFinishMessage(fixtureName, numberOfTests, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestStart(const TestDetails & details)
{
    DeferredTestReporter::ReportTestStart(details);
}

void StreamTestReporter::ReportTestFinish(const TestDetails & details, bool success,
                                          int milliSecondsElapsed)
{
    DeferredTestReporter::ReportTestFinish(details, success, milliSecondsElapsed);
    if (success)
        _stream << TestSuccessSeparator;
    else
        _stream << TestFailSeparator;

    _stream << _(" ") << TestFinishMessage(details, success, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestFailure(const TestDetails & details, const OSAL::String & failure)
{
    DeferredTestReporter::ReportTestFailure(details, failure);
}

static OSAL::String TestLiteral(int numberOfTests)
{
    return (numberOfTests == 1) ? _("test") : _("tests");
}

static OSAL::String TestFailureLiteral(int numberOfTestFailures)
{
    return (numberOfTestFailures == 1) ? _("failure") : _("failures");
}

static OSAL::String TestFixtureLiteral(int numberOfTestFixtures)
{
    return (numberOfTestFixtures == 1) ? _("test fixture") : _("test fixtures");
}

static OSAL::String TestSuiteLiteral(int numberOfTestSuites)
{
    return (numberOfTestSuites == 1) ? _("test suite") : _("test suites");
}

static OSAL::String TestSuiteName(OSAL::String name)
{
    return (!name.empty()) ? name : _("DefaultSuite");
}

static OSAL::String TestFixtureName(OSAL::String name)
{
    return (!name.empty()) ? name : _("DefaultFixture");
}

OSAL::String StreamTestReporter::TestRunStartMessage(int numberOfTestSuites,
                                                    int numberOfTestFixtures,
                                                    int numberOfTests)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << _("Running ") << numberOfTests << _(" ") << TestLiteral(numberOfTests)
           << _(" from ") << numberOfTestFixtures << _(" ") << TestFixtureLiteral(numberOfTestFixtures)
           << _(" in ") << numberOfTestSuites << _(" ") << TestSuiteLiteral(numberOfTestSuites) << _(" .");
    return stream.str();
}

OSAL::String StreamTestReporter::TestRunFinishMessage(int numberOfTestSuites,
                                                      int numberOfTestFixtures,
                                                      int numberOfTests,
                                                      int milliSecondsElapsed)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << numberOfTests << _(" ") << TestLiteral(numberOfTests)
           << _(" from ") << numberOfTestFixtures << _(" ") << TestFixtureLiteral(numberOfTestFixtures)
           << _(" in ") << numberOfTestSuites << _(" ") << TestSuiteLiteral(numberOfTestSuites)
           << _(" ran. (") << milliSecondsElapsed << _(" ms total)");
    return stream.str();
}

OSAL::String StreamTestReporter::TestRunSummaryMessage(const TestResults * results,
                                                 int milliSecondsElapsed)
{
    basic_ostringstream<OSAL::Char> stream;

    if (results->GetFailureCount() > 0)
        stream << _("FAILURE: ") << results->GetFailedTestCount()
               << _(" out of ") << results->GetTotalTestCount()
               << _(" tests failed (") << results->GetFailureCount() << _(" ")
               << TestFailureLiteral(results->GetFailureCount()) << _(").") << endl;
    else
        stream << _("Success: ") << results->GetTotalTestCount() << _(" ")
               << TestLiteral(results->GetTotalTestCount()) << _(" passed.") << endl;

    stream << _("Test time: ") << milliSecondsElapsed << _(" ms.") << endl;

    return stream.str();
}

OSAL::String StreamTestReporter::TestRunOverviewMessage(const TestResults * results)
{
    basic_ostringstream<OSAL::Char> stream;

    if (results->GetFailureCount() > 0)
    {
        stream << _("Failures:") << endl;
        for (auto testResult : Results())
        {
            if (testResult.Failed())
            {
                for (auto failure : testResult.Failures())
                    stream << testResult.fileName << _(":") << failure.first << _(" : Failure in ")
                           << TestName(testResult.suiteName, testResult.fixtureName,
                                       testResult.testName) << _(": ")
                           << failure.second << endl;
            }
        }
    }
    else
        stream << _("No failures") << endl;

    return stream.str();
}

OSAL::String StreamTestReporter::TestSuiteStartMessage(const OSAL::String & suiteName,
                                                       int numberOfTestFixtures)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << TestSuiteName(suiteName) << _(" (") << numberOfTestFixtures << _(" ")
           << TestFixtureLiteral(numberOfTestFixtures) << _(")");
    return stream.str();
}

OSAL::String StreamTestReporter::TestSuiteFinishMessage(const OSAL::String & suiteName,
                                                        int numberOfTestFixtures,
                                                        int milliSecondsElapsed)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << numberOfTestFixtures << _(" ") << TestFixtureLiteral(numberOfTestFixtures)
           << _(" from ") << TestSuiteName(suiteName)
           << _(" (") << milliSecondsElapsed << _(" ms total)") << endl;
    return stream.str();
}

OSAL::String StreamTestReporter::TestFixtureStartMessage(const OSAL::String & fixtureName, int numberOfTests)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << TestFixtureName(fixtureName)
           << _(" (") << numberOfTests << _(" ") << TestLiteral(numberOfTests) << _(")");
    return stream.str();
}

OSAL::String StreamTestReporter::TestFixtureFinishMessage(const OSAL::String & fixtureName, int numberOfTests,
                                                          int milliSecondsElapsed)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << numberOfTests << _(" ") << TestLiteral(numberOfTests)
           << _(" from ") << TestFixtureName(fixtureName)
           << _(" (") << milliSecondsElapsed << _(" ms total)") << endl;
    return stream.str();
}

OSAL::String StreamTestReporter::TestFinishMessage(const TestDetails & details,
                                                   bool UNUSED(success),
                                                   int milliSecondsElapsed)
{
    basic_ostringstream<OSAL::Char> stream;
    stream << TestName(details.suiteName, details.fixtureName, details.testName)
           << _(" (") << milliSecondsElapsed << _(" ms)");
    return stream.str();
}

OSAL::String StreamTestReporter::TestName(const OSAL::String & suiteName,
                                          const OSAL::String & fixtureName,
                                          const OSAL::String & testName)
{
	basic_ostringstream<OSAL::Char> stream;
    if (!suiteName.empty())
        stream << suiteName << _("::");
    if (!fixtureName.empty())
        stream << fixtureName << _("::");
    stream << testName;
    return stream.str();
}

} // namespace UnitTestCpp
