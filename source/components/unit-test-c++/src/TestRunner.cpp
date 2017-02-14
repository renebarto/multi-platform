#include <unit-test-c++/TestRunner.h>

#include <unit-test-c++/UnitTestC++.h>

using namespace std;

namespace UnitTestCpp
{

bool InSelection::operator () (const TestInfo * const test) const
{
    return !testName || (test->_details.testName == testName);
}

bool InSelection::operator () (const TestFixtureInfo * const fixture) const
{
    return !fixtureName || (fixture->Name() == fixtureName);
}

bool InSelection::operator () (const TestSuiteInfo * const suite) const
{
    return !suiteName || (suite->Name() == suiteName);
}

int RunAllTests()
{
    return RunSelectedTests(True());
}

int RunAllTests(ITestReporter & reporter)
{
    return RunSelectedTests(reporter, True());
}

TestRunner::TestRunner(ITestReporter & reporter)
    : reporter(&reporter)
    , testResults(new TestResults(&reporter))
    , timer(new Timer())
{
}

TestRunner::~TestRunner()
{
    delete testResults;
    delete timer;
}

} // namespace UnitTestCpp
