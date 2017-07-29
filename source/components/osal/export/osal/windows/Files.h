#pragma once

#include <io.h>
#include <stdio.h>
#include <fcntl.h>

namespace OSAL {
namespace Files {

typedef int mode_t;
typedef signed __int64 ssize_t;

inline int fileno(FILE * file)
{
    return ::_fileno(file);
}
inline int isatty(int fd)
{
    return ::_isatty(fd);
}

inline int open(const char * path, int flags)
{
    return ::_open(path, flags);
}

inline int open(const char * path, int flags, mode_t mode)
{
    return ::_open(path, flags, mode);
}

inline int creat(const char * path, mode_t mode)
{
    return ::_creat(path, mode);
}

inline int close(int fd)
{
    return ::_close(fd);
}

inline ssize_t read(int fd, void * buf, size_t count)
{
    return ::_read(fd, buf, static_cast<unsigned>(count));
}

inline ssize_t write(int fd, const void * buf, size_t count)
{
    return ::_write(fd, buf, static_cast<unsigned>(count));
}

} // namespace Files
} // namespace OSAL