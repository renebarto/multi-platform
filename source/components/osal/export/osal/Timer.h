#pragma once

#include <osal/exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Timer.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Timer.h"
#elif defined(DARWIN)
#include "osal/darwin/Timer.h"
#elif defined(LINUX)
#include "osal/linux/Timer.h"
#endif
