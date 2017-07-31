#pragma once

#include <string>
#include <unit-test-c++/Exports.h>
#include <unit-test-c++/Test.h>
#include <unit-test-c++/TestResults.h>
#include <unit-test-c++/Timer.h>

namespace UnitTestCpp
{

class TestInfo;

class UNIT_TEST_CPP_EXPORT TestFixtureInfo
{
public:
    TestFixtureInfo() = delete;
    TestFixtureInfo(const OSAL::String & fixtureName);
    TestFixtureInfo(const TestFixtureInfo &) = delete;

    TestFixtureInfo & operator = (const TestFixtureInfo &) = delete;

    void AddTest(Test * test);
    Test * GetHead() const;

    const OSAL::String & Name() const;
    TestFixtureInfo * next;

    template <class Predicate> void RunIf(const Predicate & predicate, int const maxTestTimeInMs, TestResults * testResults);

    int CountTests();
    template <typename Predicate> int CountTestsIf(Predicate predicate);

private:
    Test * head;
    Test * tail;
    OSAL::String fixtureName;
};

template <class Predicate> void TestFixtureInfo::RunIf(const Predicate & predicate, int const maxTestTimeInMs, TestResults * testResults)
{
    Timer testTimer;
    testTimer.Start();

    testResults->OnTestFixtureStart(this);

    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            test->Run(maxTestTimeInMs, testResults);
        test = test->_next;
    }

    testResults->OnTestFixtureFinish(this, testTimer.GetTimeInMilliSeconds());
}

template <typename Predicate> int TestFixtureInfo::CountTestsIf(Predicate predicate)
{
    int numberOfTests = 0;
    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            numberOfTests++;
        test = test->_next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
