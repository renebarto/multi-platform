#include <unit-test-c++/AssertException.h>

#include <cstring>

using namespace UnitTestCpp;

AssertException::AssertException(const OSAL::String & description,
                                 const OSAL::String & filename,
                                 int lineNumber)
    : description(OSAL::ToNarrowString(description))
    , fileName(filename)
    , lineNumber(lineNumber)
{
}

AssertException::~AssertException()
{
}
