#pragma once

#include <fcntl.h>
#include <osal/windows/unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <osal/osal.h>

namespace OSAL {
namespace Files {

typedef int FileDescriptor; // Non portable file descriptor
//typedef HANDLE FileHandle; // Portable file handle (descriptor for Linux, HANDLE for Windows)
constexpr FileHandle InvalidHandle = static_cast<FileHandle>(INVALID_HANDLE_VALUE);

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

struct stat
{
    stat()
        : st_dev()
        , st_ino()
        , st_mode()
        , st_nlink()
        , st_uid()
        , st_gid()
        , st_rdev()
        , st_size()
        , st_atime()
        , st_mtime()
        , st_ctime()
    {}
    stat(const struct ::stat & data)
        : st_dev(data.st_dev)
        , st_ino(data.st_ino)
        , st_mode(data.st_mode)
        , st_nlink(data.st_nlink)
        , st_uid(data.st_uid)
        , st_gid(data.st_gid)
        , st_rdev(data.st_rdev)
        , st_size(data.st_size)
        , st_atime(data.st_atime)
        , st_mtime(data.st_mtime)
        , st_ctime(data.st_ctime)
    {}

    unsigned int   st_dev;
    unsigned short st_ino;
    unsigned short st_mode;
    short          st_nlink;
    short          st_uid;
    short          st_gid;
    unsigned int   st_rdev;
    long           st_size;
    long long      st_atime;
    long long      st_mtime;
    long long      st_ctime;
};

inline int Stat(const char * path, stat * data)
{
    static struct ::stat value;
    int result = ::stat(path, &value);
    *data = stat(value);
    return result;
}

inline mode_t GetFilePermissions(const char * path)
{
    struct ::stat data;
    if (::stat(path, &data) == -1)
        return 0;
    return data.st_mode;
}

inline _off_t GetSize(const char * path)
{
    struct ::stat data;
    if (::stat(path, &data) == -1)
        return 0;
    return data.st_size;
}

} // namespace Files
} // namespace OSAL
