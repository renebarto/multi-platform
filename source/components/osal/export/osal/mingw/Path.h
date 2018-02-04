#pragma once

#include <unistd.h>
#include "osal/Unused.h"

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::chdir(path);
}
inline int CreateDir(const char * path, mode_t UNUSED(mode))
{
    return ::mkdir(path);
}

} // namespace Path
} // namespace OSAL