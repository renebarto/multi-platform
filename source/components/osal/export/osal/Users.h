#pragma once

#include <osal/exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Users.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Users.h"
#elif defined(DARWIN)
#include "osal/darwin/Users.h"
#elif defined(LINUX)
#include "osal/linux/Users.h"
#endif

