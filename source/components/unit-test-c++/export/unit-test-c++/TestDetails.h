#pragma once

#include <string>
#include <unit-test-c++/exports.h>
#include <osal/Strings.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT TestDetails
{
public:
    TestDetails(const OSAL::String & testName,
                const OSAL::String & fixtureName,
                const OSAL::String & suiteName,
                const OSAL::String & fileName, int lineNumber);
    TestDetails(const TestDetails & details, int lineNumber);

    const OSAL::String suiteName;
    const OSAL::String fixtureName;
    const OSAL::String testName;
    const OSAL::String fileName;
    const int lineNumber;
};

} // namespace UnitTestCpp
