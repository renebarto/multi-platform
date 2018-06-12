#pragma once

#include <osal/exports.h>

namespace OSAL {
    enum class Endianness { LittleEndian, BigEndian };
}

#if defined(WIN_MSVC)
#include "osal/windows/System.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/System.h"
#elif defined(DARWIN)
#include "osal/darwin/System.h"
#elif defined(LINUX)
#include "osal/linux/System.h"
#endif
