#pragma once

#include <exception>
#include <string>
#include <unit-test-c++/exports.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT AssertionFailedException : public std::exception
{
public:
    AssertionFailedException(const std::string & fileName, int lineNumber);
    virtual ~AssertionFailedException();

    virtual const char * what() const noexcept { return "Assertion failed"; }

    const std::string & Filename() const { return fileName; }
    int LineNumber() const { return lineNumber; }

private:
    std::string fileName;
    int lineNumber;
};

} // namespace UnitTestCpp
