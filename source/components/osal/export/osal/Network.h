#pragma once

#include "osal/exports.h"

#include "osal/Unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/Network.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Network.h"
#elif defined(DARWIN)
#include "osal/darwin/Network.h"
#elif defined(LINUX)
#include "osal/linux/Network.h"
#endif
