#pragma once

#include <osal/Exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Time.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Time.h"
#elif defined(DARWIN)
#include "osal/darwin/Time.h"
#elif defined(LINUX)
#include "osal/linux/Time.h"
#endif
