#pragma once

#include "osal/exports.h"
#include "osal/Unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/NetworkEndPoint.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/NetworkEndPoint.h"
#elif defined(DARWIN)
#include "osal/darwin/NetworkEndPoint.h"
#elif defined(LINUX)
#include "osal/linux/NetworkEndPoint.h"
#endif
