#pragma once

#include <iosfwd>
#include <unit-test-c++/exports.h>
#include <unit-test-c++/DeferredTestReporter.h>

namespace UnitTestCpp
{

class TestResult;

class UNIT_TEST_CPP_EXPORT XMLTestReporter : public DeferredTestReporter
{
public:
    explicit XMLTestReporter(std::ostream & stream);

    virtual void ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed) override;

private:
    XMLTestReporter(XMLTestReporter const&);
    XMLTestReporter& operator=(XMLTestReporter const&);

    void AddXmlElement(const char * encoding);
    void BeginResults(int totalTestCount, int failedTestCount, int failureCount,
                      int milliSecondsElapsed);
    void EndResults();
    void BeginTest(const TestDetailedResult & result);
    void AddFailure(const TestDetailedResult & result);
    void EndTest(const TestDetailedResult & result);

    std::ostream& stream;
};

} // namespace UnitTestCpp
