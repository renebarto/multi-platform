#pragma once

#include <unistd.h>

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::chdir(path);
}
inline int MkDir(const char * path, mode_t mode)
{
    return ::mkdir(path, mode);
}
inline int RmDir(const char * path)
{
    return ::rmdir(path);
}

} // namespace Path
} // namespace OSAL