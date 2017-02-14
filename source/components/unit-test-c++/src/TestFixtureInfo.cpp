#include <unit-test-c++/TestFixtureInfo.h>

#include <cassert>

using namespace std;

namespace UnitTestCpp
{

TestFixtureInfo::TestFixtureInfo(const string & fixtureName)
    : next(nullptr)
    , head(nullptr)
    , tail(nullptr)
    , fixtureName(fixtureName)
{
}

void TestFixtureInfo::AddTest(Test * test)
{
    if (!tail)
    {
        assert(!head);
        head = test;
        tail = test;
    }
    else
    {
        tail->_next = test;
        tail = test;
    }
}

Test * TestFixtureInfo::GetHead() const
{
    return head;
}

const string & TestFixtureInfo::Name() const
{
    return fixtureName;
}

int TestFixtureInfo::CountTests()
{
    int numberOfTests = 0;
    Test * test = GetHead();
    while (test)
    {
        ++numberOfTests;
        test = test->_next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
