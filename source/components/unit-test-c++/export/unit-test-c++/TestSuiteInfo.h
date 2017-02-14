#pragma once

#include <string>
#include <unit-test-c++/exports.h>
#include <unit-test-c++/TestFixtureInfo.h>
#include <unit-test-c++/TestResults.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT TestSuiteInfo
{
public:
    TestSuiteInfo() = delete;
    TestSuiteInfo(const TestSuiteInfo &) = delete;
    TestSuiteInfo(const std::string & suiteName);
    virtual ~TestSuiteInfo();

    TestSuiteInfo & operator = (const TestSuiteInfo &) = delete;

    TestFixtureInfo * GetTestFixture(const std::string & fixtureName);

    TestFixtureInfo * GetHead() const;

    const std::string & Name() const;
    TestSuiteInfo * next;

    template <class Predicate> void RunIf(const Predicate & predicate, int const maxTestTimeInMs,
                                          TestResults * testResults);

    int CountFixtures();
    int CountTests();
    template <typename Predicate> int CountFixturesIf(Predicate predicate);
    template <typename Predicate> int CountTestsIf(Predicate predicate);

private:
    TestFixtureInfo * head;
    TestFixtureInfo * tail;
    std::string suiteName;

    void AddFixture(TestFixtureInfo * testFixture);
};

template <class Predicate> void TestSuiteInfo::RunIf(const Predicate & predicate,
                                                     int const maxTestTimeInMs,
                                                     TestResults * testResults)
{
    Timer testTimer;
    testTimer.Start();

    testResults->OnTestSuiteStart(this);

    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        if (predicate(testFixture))
            testFixture->RunIf(predicate, maxTestTimeInMs, testResults);
        testFixture = testFixture->next;
    }

    testResults->OnTestSuiteFinish(this, testTimer.GetTimeInMilliSeconds());
}

template <typename Predicate> int TestSuiteInfo::CountFixturesIf(Predicate predicate)
{
    int numberOfTestFixtures = 0;
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        if (predicate(testFixture))
            numberOfTestFixtures++;
        testFixture = testFixture->next;
    }
    return numberOfTestFixtures;
}

template <typename Predicate> int TestSuiteInfo::CountTestsIf(Predicate predicate)
{
    int numberOfTests = 0;
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        if (predicate(testFixture))
            numberOfTests += testFixture->CountTestsIf(predicate);
        testFixture = testFixture->next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
