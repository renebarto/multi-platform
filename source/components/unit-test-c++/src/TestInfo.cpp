#include <unit-test-c++/TestInfo.h>

using namespace std;

namespace UnitTestCpp
{

TestInfo::TestInfo(const string & testName,
                   const string & fixtureName,
                   const string & suiteName,
                   const string & fileName, int lineNumber)
    : _details(testName, fixtureName, suiteName, fileName, lineNumber)
{
}

TestInfo::~TestInfo()
{
}

} // namespace UnitTestCpp
