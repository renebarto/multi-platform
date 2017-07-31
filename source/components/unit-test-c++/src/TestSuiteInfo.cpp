#include <unit-test-c++/TestSuiteInfo.h>

#include <cassert>

using namespace std;

namespace UnitTestCpp
{

TestSuiteInfo::TestSuiteInfo(const OSAL::String & suiteName)
    : next(nullptr)
    , head(nullptr)
    , tail(nullptr)
    , suiteName(suiteName)
{
}

TestSuiteInfo::~TestSuiteInfo()
{
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        TestFixtureInfo * currentFixture = testFixture;
        testFixture = testFixture->next;
        delete currentFixture;
    }
}

TestFixtureInfo * TestSuiteInfo::GetTestFixture(const OSAL::String & fixtureName)
{
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
	wcout << L"Find suite " << fixtureName << L" ... ";
#else
	cout << "Find suite " << fixtureName << " ... ";
#endif
#endif
    TestFixtureInfo * testFixture = head;
    while (testFixture && (testFixture->Name() != fixtureName))
        testFixture = testFixture->next;
    if (!testFixture)
    {
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
		wcout << L"not found, creating new object" << endl;
#else
		cout << "not found, creating new object" << endl;
#endif
#endif
        testFixture = new TestFixtureInfo(fixtureName);
        AddFixture(testFixture);
    }
    else
    {
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
		wcout << L"found" << endl;
#else
		cout << "found" << endl;
#endif
#endif
    }
    return testFixture;
}

void TestSuiteInfo::AddFixture(TestFixtureInfo * testFixture)
{
    if (!tail)
    {
        assert(!head);
        head = testFixture;
        tail = testFixture;
    }
    else
    {
        tail->next = testFixture;
        tail = testFixture;
    }
}

TestFixtureInfo * TestSuiteInfo::GetHead() const
{
    return head;
}

const OSAL::String & TestSuiteInfo::Name() const
{
    return suiteName;
}

int TestSuiteInfo::CountFixtures()
{
    int numberOfTestFixtures = 0;
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        ++numberOfTestFixtures;
        testFixture = testFixture->next;
    }
    return numberOfTestFixtures;
}

int TestSuiteInfo::CountTests()
{
    int numberOfTests = 0;
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        numberOfTests += testFixture->CountTests();
        testFixture = testFixture->next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
