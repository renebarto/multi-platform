#pragma once

#include "osal/exports.h"

#if defined(WIN_MSVC)
#include "osal/windows/osal.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/osal.h"
#elif defined(DARWIN)
#include "osal/darwin/osal.h"
#elif defined(LINUX)
#include "osal/linux/osal.h"
#endif
