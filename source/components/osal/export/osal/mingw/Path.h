#pragma once

#include <unistd.h>
#include "osal/Unused.h"

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::chdir(path);
}
inline int MkDir(const char * path, mode_t UNUSED(mode))
{
    return ::mkdir(path);
}
inline int RmDir(const char * path)
{
    return ::rmdir(path);
}
inline int MoveDir(const char * oldPath, const char * newPath)
{
    return ::rename(oldPath, newPath);
}

} // namespace Path
} // namespace OSAL