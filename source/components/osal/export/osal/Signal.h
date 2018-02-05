#pragma once

#include "osal/exports.h"
#if defined(WIN_MSVC)
#include "osal/windows/Signal.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Signal.h"
#elif defined(DARWIN)
#include "osal/darwin/Signal.h"
#elif defined(LINUX)
#include "osal/linux/Signal.h"
#endif
