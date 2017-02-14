#include <unit-test-c++/AssertionFailedException.h>

#include <cstring>

using namespace UnitTestCpp;

AssertionFailedException::AssertionFailedException(const std::string & filename, int lineNumber)
    : fileName(filename)
    , lineNumber(lineNumber)
{
}

AssertionFailedException::~AssertionFailedException()
{
}
