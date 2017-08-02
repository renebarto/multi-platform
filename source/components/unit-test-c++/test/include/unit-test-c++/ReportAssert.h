#pragma once

#include <string>
#include <unit-test-c++/AssertException.h>

namespace UnitTestCpp
{

inline void ReportAssert(OSAL::String description, OSAL::String filename, int lineNumber)
{
    throw AssertException(description, filename, lineNumber);
}

} // namespace UnitTestCpp
