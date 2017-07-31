#include <unit-test-c++/TestInfo.h>

using namespace std;

namespace UnitTestCpp
{

TestInfo::TestInfo(const OSAL::String & testName,
                   const OSAL::String & fixtureName,
                   const OSAL::String & suiteName,
                   const OSAL::String & fileName, int lineNumber)
    : _details(testName, fixtureName, suiteName, fileName, lineNumber)
{
}

TestInfo::~TestInfo()
{
}

} // namespace UnitTestCpp
