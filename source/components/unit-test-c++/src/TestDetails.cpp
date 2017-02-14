#include <unit-test-c++/TestDetails.h>

using namespace std;

namespace UnitTestCpp
{

TestDetails::TestDetails(const string & testName,
                         const string & fixtureName,
                         const string & suiteName,
                         const string & fileName, int lineNumber)
    : suiteName(suiteName)
    , fixtureName(fixtureName)
    , testName(testName)
    , fileName(fileName)
    , lineNumber(lineNumber)
{
}

TestDetails::TestDetails(const TestDetails & details, int lineNumber)
    : suiteName(details.suiteName)
    , fixtureName(details.fixtureName)
    , testName(details.testName)
    , fileName(details.fileName)
    , lineNumber(lineNumber)
{
}

} // namespace UnitTestCpp
