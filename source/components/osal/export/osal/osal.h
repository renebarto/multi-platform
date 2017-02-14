#pragma once

#include "osal/exports.h"

#include "osal/unused.h"

#if defined(WIN_MSVC)
#include "osal/windows/osal.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/osal.h"
#elif defined(DARWIN)
#include "osal/darwin/osal.h"
#elif defined(LINUX)
#include "osal/linux/osal.h"
#endif

#include "osal/os.h"
#include "osal/files.h"
#include "osal/path.h"
#include "osal/strings.h"
#include "osal/system.h"
#include "osal/time.h"

