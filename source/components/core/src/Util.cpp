#include "core/Util.h"

using namespace Core;

bool Util::Compare(long double expected, long double actual, long double epsilonAbs, long double epsilonRel)
{
    if (std::isinf(expected) && std::isinf(actual))
    {
        return (std::isinf(expected) == std::isinf(actual));
    }
    long double errorAbs = fabs(expected - actual);
    long double deltaMax = std::max(epsilonAbs, epsilonRel * std::max(fabs(actual), fabs(expected)));
    return (errorAbs <= deltaMax);
}
bool Util::Compare(double expected, double actual, double epsilonAbs, double epsilonRel)
{
    if (std::isinf(expected) && std::isinf(actual))
    {
        return (std::isinf(expected) == std::isinf(actual));
    }
    double errorAbs = fabs(expected - actual);
    double deltaMax = std::max(epsilonAbs, epsilonRel * std::max(fabs(actual), fabs(expected)));
    return (errorAbs <= deltaMax);
}
bool Util::Compare(float expected, float actual, float epsilonAbs, float epsilonRel)
{
    if (std::isinf(expected) && std::isinf(actual))
    {
        return (std::isinf(expected) == std::isinf(actual));
    }
    double errorAbs = fabsf(expected - actual);
    double deltaMax = std::max(epsilonAbs, epsilonRel * std::max(fabsf(actual), fabsf(expected)));
    return (errorAbs <= deltaMax);
}
