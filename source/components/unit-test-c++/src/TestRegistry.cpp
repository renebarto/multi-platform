#include <unit-test-c++/TestRegistry.h>

#include <cassert>

using namespace std;

namespace UnitTestCpp
{

TestRegistry::TestRegistry()
    : head(nullptr)
    , tail(nullptr)
{
}

TestRegistry::~TestRegistry()
{
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        TestSuiteInfo * currentSuite = testSuite;
        testSuite = testSuite->next;
        delete currentSuite;
    }
}

TestSuiteInfo * TestRegistry::GetTestSuite(const OSAL::String & suiteName)
{
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
	wcout << L"Find suite " << suiteName << L" ... ";
#else
	cout << "Find suite " << suiteName << " ... ";
#endif
#endif
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite && (testSuite->Name() != suiteName))
        testSuite = testSuite->next;
    if (!testSuite)
    {
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
		wcout << L"not found, creating new object" << endl;
#else
		cout << "not found, creating new object" << endl;
#endif
#endif
        testSuite = new TestSuiteInfo(suiteName);
        AddSuite(testSuite);
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
    return testSuite;
}

void TestRegistry::AddSuite(TestSuiteInfo * testSuite)
{
    if (!tail)
    {
        assert(!head);
        head = testSuite;
        tail = testSuite;
    }
    else
    {
        tail->next = testSuite;
        tail = testSuite;
    }
}

TestSuiteInfo * TestRegistry::GetHead() const
{
    return head;
}

int TestRegistry::CountSuites()
{
    int numberOfTestSuites = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        ++numberOfTestSuites;
        testSuite = testSuite->next;
    }
    return numberOfTestSuites;
}

int TestRegistry::CountFixtures()
{
    int numberOfTestFixtures = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        numberOfTestFixtures += testSuite->CountFixtures();
        testSuite = testSuite->next;
    }
    return numberOfTestFixtures;
}

int TestRegistry::CountTests()
{
    int numberOfTests = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        numberOfTests += testSuite->CountTests();
        testSuite = testSuite->next;
    }
    return numberOfTests;
}

TestRegistrar::TestRegistrar(TestRegistry & registry, Test * test)
{
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
	wcout << L"Register test " << test->details.testName
		  << L" in fixture " << test->details.fixtureName << endl;
#else
    cout << "Register test " << test->details.testName
         << " in fixture " << test->details.fixtureName << endl;
#endif
#endif
    TestSuiteInfo * testSuite = registry.GetTestSuite(test->_details.suiteName);
    TestFixtureInfo * testFixture = testSuite->GetTestFixture(test->_details.fixtureName);
    testFixture->AddTest(test);
}

} // namespace UnitTestCpp
