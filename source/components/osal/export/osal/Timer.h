#pragma once

#include <osal/exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/timer.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/timer.h"
#elif defined(DARWIN)
#include "osal/darwin/timer.h"
#elif defined(LINUX)
#include "osal/linux/timer.h"
#endif
