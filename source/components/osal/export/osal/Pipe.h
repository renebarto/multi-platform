#pragma once

#include <unistd.h>
#include "osal/Files.h"

namespace OSAL {

class Pipe
{
public:
    Pipe();
    Pipe(const Pipe &) = delete;
    Pipe(Files::FileDescriptor fdRead, Files::FileDescriptor fdWrite);
    ~Pipe();

    Pipe & operator = (const Pipe &) = delete;

    int Create();
    void Close();
    void CloseRead();
    void CloseWrite();
    ssize_t Read(void * data, ssize_t numBytes);
    ssize_t Write(void * data, ssize_t numBytes);
    Files::FileDescriptor ReadFD() { return _fd[PIPE_READ]; }
    Files::FileDescriptor WriteFD() { return _fd[PIPE_WRITE]; }

protected:
    static const int PIPE_READ = 0;
    static const int PIPE_WRITE = 1;
    static const int PIPE_COUNT = 2;
    Files::FileDescriptor _fd[PIPE_COUNT];
};

} // namespace OSAL
