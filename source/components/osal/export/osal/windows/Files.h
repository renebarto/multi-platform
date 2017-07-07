#pragma once

#include <cstdio>
#include <unistd.h>

namespace OSAL {
namespace Files {

inline int FileNo(FILE * file)
{
    return ::fileno(file);
}
inline int IsAtty(int fd)
{
    return ::isatty(fd);
}

} // namespace Files
} // namespace OSAL