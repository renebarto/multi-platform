#include <unit-test-c++/Checks.h>

using namespace std;

namespace UnitTestCpp
{

bool IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return OSAL::Strings::StrCaseCmp(lhs, rhs) == 0;
}

bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::Strings::StrCaseCmp(lhs, rhs) == 0;
}

namespace internal
{

AssertionResult CheckStringsEqual(const std::string & expectedExpression,
                                  const std::string & actualExpression,
                                  const char * expected, const char * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (strcmp(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsEqual(const std::string & expectedExpression,
                                  const std::string & actualExpression,
                                  const wchar_t * expected, const wchar_t * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (wcscmp(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqual(const std::string & expectedExpression,
                                     const std::string & actualExpression,
                                     const char * expected, const char * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (!strcmp(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqual(const std::string & expectedExpression,
                                     const std::string & actualExpression,
                                     const wchar_t * expected, const wchar_t * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (!wcscmp(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsEqualIgnoreCase(const std::string & expectedExpression,
                                            const std::string & actualExpression,
                                            const char * expected, const char * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (!IsEqualIgnoreCase(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsEqualIgnoreCase(const std::string & expectedExpression,
                                            const std::string & actualExpression,
                                            const wchar_t * expected, const wchar_t * actual)
{
    if (expected == actual)
        return AssertionSuccess();

    if (!IsEqualIgnoreCase(expected, actual))
    {
        return EqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqualIgnoreCase(const std::string & expectedExpression,
                                               const std::string & actualExpression,
                                               const char * expected, const char * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (IsEqualIgnoreCase(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

AssertionResult CheckStringsNotEqualIgnoreCase(const std::string & expectedExpression,
                                               const std::string & actualExpression,
                                               const wchar_t * expected, const wchar_t * actual)
{
    if (expected == actual)
        return InEqFailure(expectedExpression, actualExpression, expected, actual);

    if (IsEqualIgnoreCase(expected, actual))
    {
        return InEqFailure(expectedExpression, actualExpression, expected, actual);
    }
    return AssertionSuccess();
}

} // namespace internal

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   const char * expected, const char * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   char * expected, char * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   char * expected, const char * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   const char * expected, char * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   const wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternal(const std::string & expectedExpression,
                                   const std::string & actualExpression,
                                   const wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      const char * expected, const char * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      char * expected, char * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      char * expected, const char * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      const char * expected, char * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      const wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckNotEqualInternal(const std::string & expectedExpression,
                                      const std::string & actualExpression,
                                      const wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqual(expectedExpression, actualExpression, expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             const std::string & expected,
                                             const std::string & actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected.c_str(), actual.c_str());
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             const char * expected, const char * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             char * expected, char * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             char * expected, const char * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             const char * expected, char * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             const wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckEqualInternalIgnoreCase(const std::string & expectedExpression,
                                             const std::string & actualExpression,
                                             const wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsEqualIgnoreCase(expectedExpression, actualExpression,
                                                 expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                const std::string & expected,
                                                const std::string & actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected.c_str(), actual.c_str());
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                const char * expected, const char * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                char * expected, char * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                char * expected, const char * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                const char * expected, char * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                const wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                wchar_t * expected, const wchar_t * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

AssertionResult CheckNotEqualInternalIgnoreCase(const std::string & expectedExpression,
                                                const std::string & actualExpression,
                                                const wchar_t * expected, wchar_t * actual)
{
    return internal::CheckStringsNotEqualIgnoreCase(expectedExpression, actualExpression,
                                                    expected, actual);
}

} // namespace UnitTestCpp
