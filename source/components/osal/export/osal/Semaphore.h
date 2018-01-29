#pragma once

#include <osal/exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Semaphore.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Semaphore.h"
#elif defined(DARWIN)
#include "osal/darwin/Semaphore.h"
#elif defined(LINUX)
#include "osal/linux/Semaphore.h"
#endif
