#pragma once

#include <unistd.h>

namespace OSAL {
namespace Path {

inline int ChDir(const char * path)
{
    return ::_chdir(path);
}
    inline int CreateDir(const char * path, mode_t UNUSED(mode))
    {
        return ::_mkdir(path);
    }

} // namespace Path
} // namespace OSAL