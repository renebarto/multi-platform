#include <unit-test-c++/Test.h>

#include <unit-test-c++/CurrentTest.h>
#include <unit-test-c++/ExecuteTest.h>
#include <unit-test-c++/TestRegistry.h>

#ifdef UNITTEST_POSIX
#include <unit-test-c++/SignalTranslator.h>
#endif

using namespace std;

namespace UnitTestCpp
{

TestRegistry & Test::GetTestRegistry()
{
    static TestRegistry s_registry;
    return s_registry;
}

Test::Test(const string & testName,
           const string & fixtureName,
           const string & suiteName,
           const string & fileName, int lineNumber)
    : TestInfo(testName, fixtureName, suiteName, fileName, lineNumber)
    , _next(nullptr)
{
}

Test::~Test()
{
}

void Test::Run(int const UNUSED(maxTestTimeInMs),
               TestResults * testResults)
{
    CurrentTest::Results() = testResults;

    Timer testTimer;
    testTimer.Start();

    testResults->OnTestStart(_details);

    Run();

    testResults->OnTestFinish(_details, testTimer.GetTimeInMilliSeconds());
}

void Test::Run()
{
    ExecuteTest(*this, _details);
}

void Test::RunImpl() const
{
}

} // namespace UnitTestCpp
