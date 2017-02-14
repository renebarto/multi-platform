#pragma once

#include <string>
#include <unit-test-c++/exports.h>

namespace UnitTestCpp
{

struct AssertionResult
{
    AssertionResult(bool failed, const std::string & message) :
        failed(failed),
        message(message)
    {
    }
    const bool failed;
    const std::string message;
    operator bool() const { return failed; }
};

UNIT_TEST_CPP_EXPORT extern AssertionResult AssertionSuccess();
UNIT_TEST_CPP_EXPORT extern AssertionResult BooleanFailure(const std::string& valueExpression,
                                                           const std::string& expectedValue,
                                                           const std::string& actualValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult EqFailure(const std::string& expectedExpression,
                                                      const std::string& actualExpression,
                                                      const std::string& expectedValue,
                                                      const std::string& actualValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult EqFailure(const std::string& expectedExpression,
                                                      const std::string& actualExpression,
                                                      const std::wstring& expectedValue,
                                                      const std::wstring& actualValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult InEqFailure(const std::string& expectedExpression,
                                                        const std::string& actualExpression,
                                                        const std::string& expectedValue,
                                                        const std::string& actualValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult InEqFailure(const std::string& expectedExpression,
                                                        const std::string& actualExpression,
                                                        const std::wstring& expectedValue,
                                                        const std::wstring& actualValue);
extern AssertionResult ComparisonFailure(const std::string& expectedExpression,
                                         const std::string& actualExpression,
                                         const std::string& expectedValue,
                                         const std::string& actualValue,
                                         const std::string& comparisonOperator);
extern AssertionResult ComparisonFailure(const std::string& expectedExpression,
                                         const std::string& actualExpression,
                                         const std::wstring& expectedValue,
                                         const std::wstring& actualValue,
                                         const std::string& comparisonOperator);
UNIT_TEST_CPP_EXPORT extern AssertionResult CloseFailure(const std::string& expectedExpression,
                                                         const std::string& actualExpression,
                                                         const std::string& toleranceExpression,
                                                         const std::string& expectedValue,
                                                         const std::string& actualValue,
                                                         const std::string& toleranceValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult EqArrayFailure(const std::string& expectedExpression,
                                                           const std::string& actualExpression,
                                                           const std::string& countExpression,
                                                           const std::string& expectedValue,
                                                           const std::string& actualValue,
                                                           const std::string& countValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult CloseArrayFailure(const std::string& expectedExpression,
                                                              const std::string& actualExpression,
                                                              const std::string& toleranceExpression,
                                                              const std::string& countExpression,
                                                              const std::string& expectedValue,
                                                              const std::string& actualValue,
                                                              const std::string& toleranceValue,
                                                              const std::string& countValue);
UNIT_TEST_CPP_EXPORT extern AssertionResult CloseArray2DFailure(const std::string& expectedExpression,
                                                                const std::string& actualExpression,
                                                                const std::string& toleranceExpression,
                                                                const std::string& rowsName,
                                                                const std::string& columnsExpression,
                                                                const std::string& expectedValue,
                                                                const std::string& actualValue,
                                                                const std::string& toleranceValue,
                                                                const std::string& rowsValue,
                                                                const std::string& columnsValue);

} // namespace UnitTestCpp
