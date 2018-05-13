#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

namespace OSAL {
namespace Files {

typedef int FileDescriptor;
constexpr FileDescriptor InvalidHandle = static_cast<FileDescriptor>(-1);

inline FileDescriptor GetFileDescriptor(FILE * file)
{
    return ::fileno(file);
}
inline bool IsTTY(int fd)
{
    return ::isatty(fd) != 0;
}

inline FileDescriptor Open(const char * path, int flags, mode_t mode)
{
    return ::open(path, flags, mode);
}

inline FileDescriptor Open(const char * path, int flags)
{
    return ::open(path, flags, S_IRUSR | S_IWUSR);
}

inline int Create(const char * path, mode_t mode)
{
    return ::creat(path, mode);
}

inline int Close(FileDescriptor fd)
{
    return ::close(fd);
}

inline ssize_t Read(FileDescriptor fd, void * buf, size_t count)
{
    return ::read(fd, buf, count);
}

inline ssize_t Write(FileDescriptor fd, const void * buf, size_t count)
{
    return ::write(fd, buf, count);
}

inline int Remove(const char * path)
{
    return ::unlink(path);
}

inline int Stat(const char * path, struct stat * data)
{
    return ::lstat(path, data);
}

inline mode_t GetFilePermissions(const char * path)
{
    struct stat data;
    if (::stat(path, &data) == -1)
        return 0;
    return data.st_mode;
}

inline __off_t GetSize(const char * path)
{
    struct stat data;
    if (::stat(path, &data) == -1)
        return 0;
    return data.st_size;
}

inline int CreatePipe(FileDescriptor fds[2])
{
    return ::pipe(fds);
}

} // namespace Files
} // namespace OSAL