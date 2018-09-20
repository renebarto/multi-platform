#pragma once

#include <fcntl.h>
#include <osal/windows/unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

namespace OSAL {
namespace Files {

typedef int FileDescriptor;

inline FileDescriptor GetFileDescriptor(FILE * file)
{
    return ::_fileno(file);
}
inline bool IsTTY(int fd)
{
    return ::_isatty(fd) != 0;
}

inline FileDescriptor Open(const char * path, int flags)
{
    return ::_open(path, flags);
}

inline FileDescriptor Open(const char * path, int flags, mode_t mode)
{
    return ::_open(path, flags, mode);
}

inline int Create(const char * path, mode_t mode)
{
    return ::_creat(path, mode);
}

inline int Close(FileDescriptor fd)
{
    return ::_close(fd);
}

inline ssize_t Read(FileDescriptor fd, void * buf, size_t count)
{
    return ::_read(fd, buf, static_cast<unsigned int>(count));
}

inline ssize_t Write(FileDescriptor fd, const void * buf, size_t count)
{
    return ::_write(fd, buf, static_cast<unsigned int>(count));
}

inline int Remove(const char * path)
{
    return ::_unlink(path);
}

inline int Stat(const char * path, struct stat * data)
{
    return ::stat(path, data);
}

inline mode_t GetFilePermissions(const char * path)
{
    struct stat data;
    if (::stat(path, &data) == -1)
        return 0;
    return data.st_mode;
}

inline _off_t GetSize(const char * path)
{
    struct stat data;
    if (::stat(path, &data) == -1)
        return 0;
    return data.st_size;
}

} // namespace Files
} // namespace OSAL
