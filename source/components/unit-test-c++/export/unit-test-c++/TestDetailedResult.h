#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <osal/Strings.h>

namespace UnitTestCpp
{

class TestDetails;

class TestDetailedResult
{
public:
    TestDetailedResult() = delete;
    TestDetailedResult(const TestDetails & details);

    OSAL::String suiteName;
    OSAL::String fixtureName;
    OSAL::String testName;
    OSAL::String fileName;
    int lineNumber;

    typedef std::pair<int, OSAL::String> Failure;
    typedef std::vector<Failure> FailureList;

    void AddFailure(Failure failure);
    const FailureList & Failures() const { return failures; }
    FailureList & Failures() { return failures; }
    void Failed(bool value) { failed = value; }
    bool Failed() const { return failed; }
    void MilliSecondsElapsed(int value) { milliSecondsElapsed = value; }
    int MilliSecondsElapsed() const { return milliSecondsElapsed; }

private:
    FailureList failures;
    bool failed;
    int milliSecondsElapsed;
};

inline std::basic_ostream<OSAL::Char> & operator << (std::basic_ostream<OSAL::Char> & stream, const TestDetailedResult & result)
{
    return stream << result.fileName << _(":") << result.lineNumber << _(" : Test ")
                  << result.suiteName << _("::") << result.fixtureName << _("::") << result.testName;
}

} // namespace UnitTestCpp
