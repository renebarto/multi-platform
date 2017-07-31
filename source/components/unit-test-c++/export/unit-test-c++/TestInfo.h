#pragma once

#include <string>
#include <unit-test-c++/Exports.h>
#include <unit-test-c++/TestDetails.h>
#include <osal/Strings.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT TestInfo
{
public:
    TestInfo(const TestInfo &) = delete;
    explicit TestInfo(const OSAL::String & testName,
                      const OSAL::String & fixtureName = {},
                      const OSAL::String & suiteName = {},
                      const OSAL::String & fileName = {}, int lineNumber = 0);
    virtual ~TestInfo();

    TestInfo & operator = (const TestInfo &) = delete;

    TestDetails _details;
};

} // namespace UnitTestCpp
