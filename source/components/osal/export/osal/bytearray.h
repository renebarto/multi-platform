#pragma once

#include "osal/exports.h"

#if defined(WIN_MSVC)
#include "osal/windows/ByteArray.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/ByteArray.h"
#elif defined(DARWIN)
#include "osal/darwin/ByteArray.h"
#elif defined(LINUX)
#include "osal/linux/ByteArray.h"
#endif
