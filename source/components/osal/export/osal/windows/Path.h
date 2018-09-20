#pragma once

#include <osal/windows/unistd.h>
#include <direct.h>

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::_chdir(path);
}
inline int MkDir(const char * path, mode_t UNUSED(mode))
{
    return ::_mkdir(path);
}
inline int RmDir(const char * path)
{
    return ::_rmdir(path);
}
inline int MoveDir(const char * oldPath, const char * newPath)
{
    return ::rename(oldPath, newPath);
}

} // namespace Path
} // namespace OSAL