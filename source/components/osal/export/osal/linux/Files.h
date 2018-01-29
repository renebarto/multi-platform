#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

namespace OSAL {
namespace Files {

typedef int FileDescriptor;

inline FileDescriptor GetFileDescriptor(FILE * file)
{
    return ::fileno(file);
}
inline bool IsTTY(int fd)
{
    return ::isatty(fd) != 0;
}

inline FileDescriptor Open(const char * path, int flags)
{
    return ::open(path, flags);
}

inline FileDescriptor Open(const char * path, int flags, mode_t mode)
{
    return ::open(path, flags, mode);
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

inline int ChDir(const char * path)
{
    return ::chdir(path);
}

class OSAL_EXPORT TemporaryFile
{
public:
    TemporaryFile();
    ~TemporaryFile();
    void Close();
    std::iostream & GetStream();
    std::string const & GetPath() const;

private:
    std::shared_ptr<std::iostream> _stream;
    std::string _path;
};

} // namespace Files
} // namespace OSAL