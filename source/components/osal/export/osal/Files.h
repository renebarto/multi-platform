#pragma once

#include <osal/Exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Files.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Files.h"
#elif defined(DARWIN)
#include "osal/darwin/Files.h"
#elif defined(LINUX)
#include "osal/linux/Files.h"
#endif
