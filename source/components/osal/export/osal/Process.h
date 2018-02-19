#pragma once

#include "osal/exports.h"
#if defined(WIN_MSVC)
#include "osal/windows/Process.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Process.h"
#elif defined(DARWIN)
#include "osal/darwin/Process.h"
#elif defined(LINUX)
#include "osal/linux/Process.h"
#endif
