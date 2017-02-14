#pragma once

#include <sstream>
#include <string>
#include <osal/unused.h>
#include <unit-test-c++/exports.h>
#include <unit-test-c++/AssertionResult.h>
#include <unit-test-c++/PrintValue.h>
#include <unit-test-c++/TestResults.h>

WARNING_PUSH_N(3)

namespace UnitTestCpp
{

class TestResults;
class TestDetails;

//template< typename Value >
//bool Check(Value const value)
//{
//    return !!value;
//}
//
template<typename Value>
bool CheckTrue(Value const value)
{
    return !!value;
}

template<typename Value>
bool CheckFalse(Value const value)
{
    return !value;
}

namespace internal
{

class Secret;

} // namespace internal

//template<bool> struct EnableIf;
//template<> struct EnableIf<true> { typedef void type; };  // NOLINT
//
template <typename ToPrint, typename OtherOperand>
class FormatForComparison
{
public:
    static std::string Format(const ToPrint& value)
    {
        return PrintToString(value);
    }
};

typedef uint64_t BiggestInt;

//template <typename ToPrint, size_t N, typename OtherOperand>
//class FormatForComparison<ToPrint[N], OtherOperand>
//{
//public:
//    static std::string Format(const ToPrint[N] value)
//    {
//        PrintToString(value);
//        //        return FormatForComparison<const ToPrint *, OtherOperand>::Format(value);
//    }
//};

template <typename T1, typename T2>
std::string FormatForComparisonFailureMessage(const T1& value, const T2& /* other_operand */)
{
    return FormatForComparison<T1, T2>::Format(value);
}

template< typename Value >
AssertionResult CheckTrue(const std::string & valueName, const Value & value)
{
    if (!CheckTrue(value))
    {
        return BooleanFailure(valueName,
                              "true",
                              PrintToString(value));
    }
    return AssertionSuccess();
}

template< typename Value >
AssertionResult CheckFalse(const std::string & valueName, const Value & value)
{
    if (!CheckFalse(value))
    {
        return BooleanFailure(valueName,
                              "false",
                              PrintToString(value));
    }
    return AssertionSuccess();
}

template< typename Op1, typename Op2 >
bool IsLessEqual(const Op1 & op1, const Op2 & op2)
{
    return (op1 <= op2);
}

template< typename Op1, typename Op2 >
bool IsLess(const Op1 & op1, const Op2 & op2)
{
    return (op1 < op2);
}

template< typename Op1, typename Op2 >
bool IsGreaterEqual(const Op1 & op1, const Op2 & op2)
{
    return (op1 >= op2);
}

template< typename Op1, typename Op2 >
bool IsGreater(const Op1 & op1, const Op2 & op2)
{
    return (op1 > op2);
}

template< typename Expected, typename Actual >
bool AreEqual(const Expected & expected, const Actual & actual)
{
    return (expected == actual);
}

template< typename Expected, typename Actual >
AssertionResult CheckEqualInternal(const std::string & expectedExpression, const std::string & actualExpression,
                                   const Expected & expected, const Actual & actual)
{
    if (!AreEqual(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression,
                         FormatForComparisonFailureMessage(expected, actual),
                         FormatForComparisonFailureMessage(actual, expected));
    }
    return AssertionSuccess();
}

template< typename Expected, typename Actual >
AssertionResult CheckNotEqualInternal(const std::string & expectedExpression, const std::string & actualExpression,
                                      const Expected & expected, const Actual & actual)
{
    if (AreEqual(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression,
                           FormatForComparisonFailureMessage(expected, actual),
                           FormatForComparisonFailureMessage(actual, expected));
    }
    return AssertionSuccess();
}

template< typename Op1, typename Op2 >
AssertionResult CheckLessEqualInternal(const std::string & op1Expression, const std::string & op2Expression,
    const Op1 & op1, const Op2 & op2)
{
    if (!IsLessEqual(op1, op2))
    {
        return ComparisonFailure(op1Expression, op2Expression,
            FormatForComparisonFailureMessage(op1, op2),
            FormatForComparisonFailureMessage(op2, op1),
            "<=");
    }
    return AssertionSuccess();
}

template< typename Op1, typename Op2 >
AssertionResult CheckLessInternal(const std::string & op1Expression, const std::string & op2Expression,
    const Op1 & op1, const Op2 & op2)
{
    if (!IsLess(op1, op2))
    {
        return ComparisonFailure(op1Expression, op2Expression,
            FormatForComparisonFailureMessage(op1, op2),
            FormatForComparisonFailureMessage(op2, op1),
            "<");
    }
    return AssertionSuccess();
}

template< typename Op1, typename Op2 >
AssertionResult CheckGreaterEqualInternal(const std::string & op1Expression, const std::string & op2Expression,
    const Op1 & op1, const Op2 & op2)
{
    if (!IsGreaterEqual(op1, op2))
    {
        return ComparisonFailure(op1Expression, op2Expression,
            FormatForComparisonFailureMessage(op1, op2),
            FormatForComparisonFailureMessage(op2, op1),
            ">=");
    }
    return AssertionSuccess();
}

template< typename Op1, typename Op2 >
AssertionResult CheckGreaterInternal(const std::string & op1Expression, const std::string & op2Expression,
    const Op1 & op1, const Op2 & op2)
{
    if (!IsGreater(op1, op2))
    {
        return ComparisonFailure(op1Expression, op2Expression,
            FormatForComparisonFailureMessage(op1, op2),
            FormatForComparisonFailureMessage(op2, op1),
            ">");
    }
    return AssertionSuccess();
}

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression, const std::string & actualExpression,
                                                        const char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression, const std::string & actualExpression,
                                                        char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression, const std::string & actualExpression,
                                                        char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                                        const std::string & actualExpression,
                                                        const char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                                        const std::string & actualExpression,
                                                        const wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                                        const std::string & actualExpression,
                                                        wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                                        const std::string & actualExpression,
                                                        wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                                        const std::string & actualExpression,
                                                        const wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           const char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           const char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           const wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                                           const std::string & actualExpression,
                                                           const wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                  const std::string & actualExpression,
                                                                  const std::string & expected,
                                                                  const std::string & actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const std::string & expected,
                                                                     const std::string & actual);

//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             const char * expected, const char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             char * expected, char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             char * expected, const char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             const char * expected, char * actual);
//
UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                  const std::string & actualExpression,
                                                                  const wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                  const std::string & actualExpression,
                                                                  wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                  const std::string & actualExpression,
                                                                  wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                  const std::string & actualExpression,
                                                                  const wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const wchar_t * expected, wchar_t * actual);

//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
//                                             const std::string & actualExpression,
//                                             const std::string & expected,
//                                             const std::string & actual);
//
//AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
//                                                const std::string & actualExpression,
//                                                const std::string & expected,
//                                                const std::string & actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             const char * expected, const char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             char * expected, char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             char * expected, const char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             const char * expected, char * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             const wchar_t * expected, const wchar_t * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                            wchar_t * expected, wchar_t * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             wchar_t * expected, const wchar_t * actual);
//
//AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression, const std::string & actualExpression,
//                                             const wchar_t * expected, wchar_t * actual);
//
UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     char * expected, const char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const char * expected, char * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     wchar_t * expected, wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     wchar_t * expected, const wchar_t * actual);

UNIT_TEST_CPP_EXPORT AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                                     const std::string & actualExpression,
                                                                     const wchar_t * expected, wchar_t * actual);

// The helper class for {ASSERT|EXPECT}_EQ.  The template argument
// lhs_is_null_literal is true iff the first argument to ASSERT_EQ()
// is a null pointer literal.  The following default implementation is
// for lhs_is_null_literal being false.
template <bool lhs_is_null_literal>
class EqHelper
{
public:
    // This templatized version is for the general case.
    template <typename Expected, typename Actual>
    static AssertionResult CheckEqual(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      const Expected & expected, const Actual & actual)
    {
        return CheckEqualInternal(expectedExpression, actualExpression, expected, actual);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckNotEqual(const std::string & expectedExpression,
                                         const std::string & actualExpression,
                                         const Expected & expected, const Actual & actual)
    {
        return CheckNotEqualInternal(expectedExpression, actualExpression, expected, actual);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckLessEqual(const std::string & op1Expression,
                                          const std::string & op2Expression,
                                          const Expected & op1, const Actual & op2)
    {
        return CheckLessEqualInternal(op1Expression, op2Expression, op1, op2);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckLess(const std::string & op1Expression,
        const std::string & op2Expression,
        const Expected & op1, const Actual & op2)
    {
        return CheckLessInternal(op1Expression, op2Expression, op1, op2);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckGreaterEqual(const std::string & op1Expression,
        const std::string & op2Expression,
        const Expected & op1, const Actual & op2)
    {
        return CheckGreaterEqualInternal(op1Expression, op2Expression, op1, op2);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckGreater(const std::string & op1Expression,
        const std::string & op2Expression,
        const Expected & op1, const Actual & op2)
    {
        return CheckGreaterInternal(op1Expression, op2Expression, op1, op2);
    }

    // With this overloaded version, we allow anonymous enums to be used
    // in {ASSERT|EXPECT}_EQ when compiled with gcc 4, as anonymous
    // enums can be implicitly cast to BiggestInt.
    //
    // Even though its body looks the same as the above version, we
    // cannot merge the two, as it will make anonymous enums unhappy.
    static AssertionResult CheckEqual(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      BiggestInt expected, BiggestInt actual)
    {
        return CheckEqualInternal(expectedExpression, actualExpression, expected, actual);
    }

    static AssertionResult CheckNotEqual(const std::string & expectedExpression,
                                         const std::string & actualExpression,
                                         BiggestInt expected, BiggestInt actual)
    {
        return CheckNotEqualInternal(expectedExpression, actualExpression, expected, actual);
    }
};

class EqHelperStringCaseInsensitive
{
public:
    template <typename Expected, typename Actual>
    static AssertionResult CheckEqualIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                const Expected & expected, const Actual & actual)
    {
        return CheckEqualInternalIgnoreCase(expectedExpression, actualExpression, expected, actual);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckNotEqualIgnoreCase(const std::string & expectedExpression,
                                                   const std::string & actualExpression,
                                                   const Expected & expected, const Actual & actual)
    {
        return CheckNotEqualInternalIgnoreCase(expectedExpression, actualExpression, expected, actual);
    }
};

// This specialization is used when the first argument to ASSERT_EQ()
// is a null pointer literal, like nullptr, false, or 0.
template <>
class EqHelper<true>
{
public:
    // We define two overloaded versions of Compare().  The first
    // version will be picked when the second argument to ASSERT_EQ() is
    // NOT a pointer, e.g. ASSERT_EQ(0, AnIntFunction()) or
    // EXPECT_EQ(false, a_bool).
    template <typename Expected, typename Actual>
    static AssertionResult CheckEqual(const std::string & expectedExpression, const std::string & actualExpression,
                                      const Expected & expected, const Actual & actual,
      // The following line prevents this overload from being considered if T2
      // is not a pointer type.  We need this because ASSERT_EQ(nullptr, my_ptr)
      // expands to Compare("", "", nullptr, my_ptr), which requires a conversion
      // to match the Secret* in the other overload, which would otherwise make
      // this template match better.

                                      typename std::enable_if<!std::is_pointer<Actual>::value>::type* = 0)
//                                      typename std::enable_if<!std::is_null_pointer<Expected>::value>::type* = 0)
    {
        return CheckEqualInternal(expectedExpression, actualExpression, expected, actual);
    }

    template <typename Expected, typename Actual>
    static AssertionResult CheckNotEqual(const std::string & expectedExpression, const std::string & actualExpression,
                                         const Expected & expected, const Actual & actual,
      // The following line prevents this overload from being considered if T2
      // is not a pointer type.  We need this because ASSERT_EQ(nullptr, my_ptr)
      // expands to Compare("", "", nullptr, my_ptr), which requires a conversion
      // to match the Secret* in the other overload, which would otherwise make
      // this template match better.
                                         typename std::enable_if<!std::is_pointer<Actual>::value>::type* = 0)
//                                         typename std::enable_if<!std::is_null_pointer<Expected>::value>::type* = 0)
    {
        return CheckNotEqualInternal(expectedExpression, actualExpression, expected, actual);
    }

    // This version will be picked when the second argument to ASSERT_EQ() is a
    // pointer, e.g. ASSERT_EQ(nullptr, a_pointer).
    template <typename Actual>
    static AssertionResult CheckEqual(const std::string & expectedExpression, const std::string & actualExpression,
                                      internal::Secret * UNUSED(expected) /*(nullptr) */, Actual * actual)
    // We used to have a second template parameter instead of Secret*.  That
    // template parameter would deduce to 'long', making this a better match
    // than the first overload even without the first overload's EnableIf.
    // Unfortunately, gcc with -Wconversion-null warns when "passing 0 to
    // non-pointer argument" (even a deduced integral argument), so the old
    // implementation caused warnings in user code.
    {
        // We already know that 'expected' is a null pointer.
        return CheckEqualInternal(expectedExpression, actualExpression, static_cast<Actual*>(nullptr), actual);
    }

    template <typename Actual>
    static AssertionResult CheckNotEqual(const std::string & expectedExpression, const std::string & actualExpression,
                                         internal::Secret * UNUSED(expected) /* (nullptr) */, Actual * actual)
    // We used to have a second template parameter instead of Secret*.  That
    // template parameter would deduce to 'long', making this a better match
    // than the first overload even without the first overload's EnableIf.
    // Unfortunately, gcc with -Wconversion-null warns when "passing 0 to
    // non-pointer argument" (even a deduced integral argument), so the old
    // implementation caused warnings in user code.
    {
        // We already know that 'expected' is a null pointer.
        return CheckNotEqualInternal(expectedExpression, actualExpression, static_cast<Actual*>(nullptr), actual);
    }
};

template< typename Expected, typename Actual, typename Tolerance >
bool AreClose(const Expected & expected, const Actual & actual, const Tolerance & tolerance)
{
    return (actual >= (expected - tolerance)) && (actual <= (expected + tolerance));
}

template< typename Expected, typename Actual, typename Tolerance >
AssertionResult CheckClose(const std::string & expectedExpression,
                           const std::string & actualExpression,
                           const std::string & toleranceExpression,
                           const Expected & expected, const Actual & actual,
                           const Tolerance & tolerance)
{
    if (!AreClose(expected, actual, tolerance))
    {
        return CloseFailure(expectedExpression, actualExpression, toleranceExpression,
                            FormatForComparisonFailureMessage(expected, actual),
                            FormatForComparisonFailureMessage(actual, expected),
                            FormatForComparisonFailureMessage(tolerance, expected));
    }
    return AssertionSuccess();
}

template< typename Expected, typename Actual >
AssertionResult CheckArrayEqual(const std::string & expectedExpression,
                                const std::string & actualExpression,
                                const std::string & countExpression,
                                const Expected & expected, const Actual & actual,
                                int const count)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= (AreEqual(expected[i], actual[i]));

    if (!equal)
    {
        return EqArrayFailure(expectedExpression, actualExpression, countExpression,
                              FormatForComparisonFailureMessage(expected, actual),
                              FormatForComparisonFailureMessage(actual, expected),
                              PrintToString(count));
    }
    return AssertionSuccess();
}

template<typename Expected, typename Actual, typename Tolerance>
bool ArrayAreClose(const Expected & expected, const Actual & actual, int const count,
                   const Tolerance & tolerance)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= AreClose(expected[static_cast<size_t>(i)], actual[static_cast<size_t>(i)], tolerance);
    return equal;
}

template<typename Expected, typename Actual, typename Tolerance>
AssertionResult CheckArrayClose(const std::string & expectedExpression,
                                const std::string & actualExpression,
                                const std::string & countExpression,
                                const std::string & toleranceExpression,
                                const Expected & expected, const Actual & actual,
                                int const count, const Tolerance & tolerance)
{
    bool equal = ArrayAreClose(expected, actual, count, tolerance);

    if (!equal)
    {
        return CloseArrayFailure(expectedExpression, actualExpression, toleranceExpression,
                                 countExpression,
                                 FormatForComparisonFailureMessage(expected, actual),
                                 FormatForComparisonFailureMessage(actual, expected),
                                 FormatForComparisonFailureMessage(tolerance, expected),
                                 PrintToString(count));
    }
    return AssertionSuccess();
}

template< typename Expected, typename Actual, typename Tolerance >
AssertionResult CheckArray2DClose(const std::string & expectedExpression,
                                  const std::string & actualExpression,
                                  const std::string & rowsExpression,
                                  const std::string & columnsExpression,
                                  const std::string & toleranceExpression,
                                  const Expected & expected, const Actual & actual,
                                  int const rows, int const columns, const Tolerance & tolerance)
{
    bool equal = true;
    for (int i = 0; i < rows; ++i)
        equal &= ArrayAreClose(expected[i], actual[i], columns, tolerance);

    if (!equal)
    {
        return CloseArray2DFailure(expectedExpression, actualExpression, toleranceExpression,
                                   rowsExpression, columnsExpression,
                                   FormatForComparisonFailureMessage(expected, actual),
                                   FormatForComparisonFailureMessage(actual, expected),
                                   FormatForComparisonFailureMessage(tolerance, expected),
                                   PrintToString(rows),
                                   PrintToString(columns));
    }
    return AssertionSuccess();
}

} // namespace UnitTestCpp

WARNING_POP