#pragma once

#include <exception>
#include <string>
#include <unit-test-c++/exports.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT AssertException : public std::exception
{
public:
    AssertException(const std::string & description, const std::string & fileName, int lineNumber);
    virtual ~AssertException();

    virtual const char * what() const noexcept { return description.c_str(); }

    const std::string & Filename() const { return fileName; }
    int LineNumber() const { return lineNumber; }

private:
    std::string description;
    std::string fileName;
    int lineNumber;
};

} // namespace UnitTestCpp
