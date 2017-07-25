#pragma once

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

namespace OSAL {
namespace Files {

inline int fileno(FILE * file)
{
    return ::fileno(file);
}
inline int isatty(int fd)
{
    return ::isatty(fd);
}

inline int open(const char * path, int flags)
{
    return ::open(path, flags);
}

inline int open(const char * path, int flags, mode_t mode)
{
    return ::open(path, flags, mode);
}

inline int creat(const char * path, mode_t mode)
{
    return ::creat(path, mode);
}

inline int close(int fd)
{
    return ::close(fd);
}

inline ssize_t read(int fd, void * buf, size_t count)
{
    return ::read(fd, buf, count);
}

inline ssize_t write(int fd, const void * buf, size_t count)
{
    return ::write(fd, buf, count);
}

} // namespace Files
} // namespace OSAL