#include "osal/exports.h"
#if WIN_MSVC
#pragma warning(disable : 4668)
#endif

#include "osal/OSAL.h"

#if WIN_MSVC
#pragma warning(default : 4668)
#endif

size_t OSAL::NextPowerOfTwo(size_t value)
{
    size_t rest = value;
    size_t numberOfOnes = 0;
    size_t power = 0;
    while (rest != 0)
    {
        power++;
        if ((rest & 0x01) != 0)
            numberOfOnes++;
        rest >>= 1;
    }
    return static_cast<size_t>((numberOfOnes != 1) ? 1 << power : 1 << (power - 1));
}

