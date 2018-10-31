#pragma once

#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include "osal/exports.h"

namespace OSAL {
namespace Util {

OSAL_EXPORT size_t NextPowerOfTwo(size_t value);

//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(T expected, T actual, T epsilonAbs, T epsilonRel)
//{
//    if (std::isinf(expected) && std::isinf(actual))
//    {
//        bool expectedSign = (expected < std::numeric_limits<T>::lowest());
//        bool actualSign = (actual < std::numeric_limits<T>::lowest());
//        return (expectedSign == actualSign) && (std::isinf(expected) == std::isinf(actual));
//    }
//    T errorAbs = fabs(expected - actual);
//    T deltaMax = min(epsilonAbs, epsilonRel * max(fabs(actual), fabs(expected)));
//    return (errorAbs <= deltaMax);
//}
//
template<class T>
bool Compare(const T * expected, size_t offsetExpected,
             const T * actual, size_t offsetActual, size_t numSamples)
{
    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
    {
        return false;
    }
    const T * expectedPtr = expected + offsetExpected;
    const T * actualPtr = actual + offsetActual;
    for (size_t i = 0; i < numSamples; i++)
    {
        if (*expectedPtr++ != *actualPtr++)
            return false;
    }
    return true;
}

template<class T>
bool Compare(const T * expected, const T * actual, size_t numSamples)
{
    return Compare(expected, 0, actual, 0, numSamples);
}

//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(const T * expected, int offsetExpected,
//        const T * actual, int offsetActual, int numSamples,
//        T epsilonAbs, T epsilonRel)
//{
//    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
//    {
//        return false;
//    }
//    const T * expectedPtr = expected + offsetExpected;
//    const T * actualPtr = actual + offsetActual;
//    for (int i = 0; i < numSamples; i++)
//    {
//        if (!Compare(*expectedPtr++,
//                     *actualPtr++,
//                     epsilonAbs, epsilonRel))
//            return false;
//    }
//    return true;
//}
//
//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(const T * expected, const T * actual, int numSamples,
//        T epsilonAbs, T epsilonRel)
//{
//    return Compare(expected, 0, actual, 0, numSamples, epsilonAbs, epsilonRel);
//}
//
template<class T>
bool Compare(const T * const * expected, size_t offsetExpectedRows, size_t offsetExpectedCols,
             const T * const * actual, size_t offsetActualRows, size_t offsetActualCols,
             size_t numRows, size_t numCols)
{
    if ((offsetExpectedRows < 0) || (offsetExpectedCols < 0) ||
        (offsetActualRows < 0) || (offsetActualCols < 0) ||
        (numRows < 0) || (numCols < 0))
    {
        return false;
    }
    const T * const * expectedRowPtr = expected + offsetExpectedRows;
    const T * const * actualRowPtr = actual + offsetActualRows;
    for (size_t i = 0; i < numRows; i++)
    {
        const T * expectedPtr = *expectedRowPtr++ + offsetExpectedCols;
        const T * actualPtr = *actualRowPtr++ + offsetActualCols;
        for (size_t j = 0; j < numCols; j++)
        {
            if (*expectedPtr++ != *actualPtr++)
                return false;
        }
    }
    return true;
}

template<class T>
bool Compare(const T * const * expected, const T * const * actual, size_t numRows, size_t numCols)
{
    return Compare(expected, 0, 0, actual, 0, 0, numRows, numCols);
}

//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(const T * const * expected, int offsetExpectedRows, int offsetExpectedCols,
//        const T * const * actual, int offsetActualRows, int offsetActualCols,
//        int numRows, int numCols,
//        T epsilonAbs, T epsilonRel)
//{
//    if ((offsetExpectedRows < 0) || (offsetExpectedCols < 0) ||
//        (offsetActualRows < 0) || (offsetActualCols < 0) ||
//        (numRows < 0) || (numCols < 0))
//    {
//        return false;
//    }
//    const T * const * expectedRowPtr = expected + offsetExpectedRows;
//    const T * const * actualRowPtr = actual + offsetActualRows;
//    for (int i = 0; i < numRows; i++)
//    {
//        const T * expectedPtr = *expectedRowPtr++ + offsetExpectedCols;
//        const T * actualPtr = *actualRowPtr++ + offsetActualCols;
//        for (int j = 0; j < numCols; j++)
//        {
//            if (!Compare(*expectedPtr++, *actualPtr++,
//                         epsilonAbs, epsilonRel))
//                return false;
//        }
//    }
//    return true;
//}
//
//template<class T>
//typename std::enable_if<std::is_floating_point<T>::value, bool>::type
//Compare(const T * const * expected, const T * const * actual,
//        int numRows, int numCols,
//        T epsilonAbs, T epsilonRel)
//{
//    return Compare(expected, 0, 0, actual, 0, 0, numRows, numCols, epsilonAbs, epsilonRel);
//}

} // namespace Util
} // namespace OSAL
