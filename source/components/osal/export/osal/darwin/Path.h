#pragma once

#include <unistd.h>

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::chdir(path);
}
inline int CreateDir(const char * path, mode_t mode)
{
    return ::mkdir(path, mode);
}

} // namespace Path
} // namespace OSAL