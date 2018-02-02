#pragma once

#include <unistd.h>

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::chdir(path);
}

} // namespace Path
} // namespace OSAL