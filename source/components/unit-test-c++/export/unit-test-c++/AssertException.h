#pragma once

#include <exception>
#include <string>
#include <unit-test-c++/exports.h>
#include <osal/Strings.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT AssertException : public std::exception
{
public:
    AssertException(const OSAL::String & description, const OSAL::String & fileName, int lineNumber);
    virtual ~AssertException();

    virtual const char * what() const noexcept { return description.c_str(); }

    const OSAL::String & Filename() const { return fileName; }
    int LineNumber() const { return lineNumber; }

private:
    std::string description;
    OSAL::String fileName;
    int lineNumber;
};

} // namespace UnitTestCpp
