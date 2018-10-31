#pragma once

#include "osal/exports.h"
#include <unistd.h>
#include <osal/Files.h>

namespace OSAL {

typedef HANDLE FileHandle; // Portable file handle (file descriptor for Linux, HANDLE for Windows)
extern const OSAL_EXPORT FileHandle InvalidHandle;

class OSAL_EXPORT Pipe
{
public:
    Pipe();
    Pipe(const Pipe &) = delete;
    Pipe(FileHandle fdRead, FileHandle fdWrite);
    ~Pipe();

    Pipe & operator = (const Pipe &) = delete;

    int Create();
    void Close();
    void CloseRead();
    void CloseWrite();
    ssize_t Read(void * data, ssize_t numBytes);
    ssize_t Write(void * data, ssize_t numBytes);
    FileHandle ReadFD() { return _fd[PIPE_READ]; }
    FileHandle WriteFD() { return _fd[PIPE_WRITE]; }

protected:
    static const int PIPE_READ = 0;
    static const int PIPE_WRITE = 1;
    static const int PIPE_COUNT = 2;
    FileHandle _fd[PIPE_COUNT];
};

} // namespace OSAL
