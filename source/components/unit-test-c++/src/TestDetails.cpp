#include <unit-test-c++/TestDetails.h>

using namespace std;

namespace UnitTestCpp
{

TestDetails::TestDetails(const OSAL::String & testName,
                         const OSAL::String & fixtureName,
                         const OSAL::String & suiteName,
                         const OSAL::String & fileName, int lineNumber)
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
