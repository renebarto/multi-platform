#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>
#include "core/exports.h"

namespace Core {
namespace Util {

enum class Endianness { LittleEndian, BigEndian };

bool CORE_EXPORT Compare(long double expected, long double actual, long double epsilonAbs, long double epsilonRel);

bool CORE_EXPORT Compare(double expected, double actual, double epsilonAbs, double epsilonRel);

bool CORE_EXPORT Compare(float expected, float actual, float epsilonAbs, float epsilonRel);

template<class T>
bool Compare(T const * expected, int offsetExpected,
             T const * actual, int offsetActual, int numSamples)
{
    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
    {
        return false;
    }
    for (int i = 0; i < numSamples; i++)
    {
        if (expected[i + offsetExpected] != actual[i + offsetActual])
            return false;
    }
    return true;
}

template<class T>
bool Compare(T const * expected, T const * actual, int numSamples)
{
    return Compare(expected, 0, actual, 0, numSamples);
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(T const * expected, int offsetExpected,
        T const * actual, int offsetActual, int numSamples,
        T epsilonAbs, T epsilonRel)
{
    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
    {
        return false;
    }
    for (int i = 0; i < numSamples; i++)
    {
        if (!Compare(expected[i + offsetExpected],
                     actual[i + offsetActual],
                     epsilonAbs, epsilonRel))
            return false;
    }
    return true;
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(T const * expected, T const * actual, int numSamples,
        T epsilonAbs, T epsilonRel)
{
    return Compare(expected, 0, actual, 0, numSamples, epsilonAbs, epsilonRel);
}

} // namespace Util
} // namespace Core
