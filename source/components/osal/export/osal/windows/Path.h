#pragma once

#include <unistd.h>

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::_chdir(path);
}
inline int MkDir(const char * path, mode_t mode)
{
    return ::_mkdir(path, mode);
}
inline int RmDir(const char * path)
{
    return ::_rmdir(path);
}

} // namespace Path
} // namespace OSAL