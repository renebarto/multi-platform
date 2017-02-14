#pragma once

#include <unit-test-c++/AssertionFailedException.h>
#include <unit-test-c++/AssertMacros.h>
#include <unit-test-c++/ConsoleTestReporter.h>
#include <unit-test-c++/CurrentTest.h>
#include <unit-test-c++/ITestReporter.h>
#include <unit-test-c++/TestFixture.h>
#include <unit-test-c++/TestMacros.h>
#include <unit-test-c++/TestRunner.h>
#include <unit-test-c++/TimeConstraint.h>
#include <unit-test-c++/XMLTestReporter.h>

namespace UnitTestCpp
{

UNIT_TEST_CPP_EXPORT int RunAllTests();
UNIT_TEST_CPP_EXPORT int RunAllTests(ITestReporter & reporter);

template <class Predicate>
int RunSelectedTests(const Predicate & predicate)
{
	ConsoleTestReporter reporter;
	return RunSelectedTests(reporter, predicate);
}

template <class Predicate>
int RunSelectedTests(ITestReporter & reporter, const Predicate & predicate)
{
	TestRunner runner(reporter);
	return runner.RunTestsIf(Test::GetTestRegistry(), predicate, 0);
}

} // namespace UnitTestCpp
