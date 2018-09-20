#pragma once

#include "osal/exports.h"
#include <osal/windows/unistd.h>
#include <osal/osal.h>

namespace OSAL {

class Pipe
{
public:
    Pipe();
    Pipe(const Pipe &) = delete;
    Pipe(HANDLE fdRead, HANDLE fdWrite);
    ~Pipe();

    Pipe & operator = (const Pipe &) = delete;

    int Create();
    void Close();
    void CloseRead();
    void CloseWrite();
    ssize_t Read(void * data, ssize_t numBytes);
    ssize_t Write(void * data, ssize_t numBytes);
    HANDLE ReadFD() { return _fd[PIPE_READ]; }
    HANDLE WriteFD() { return _fd[PIPE_WRITE]; }

protected:
    static const int PIPE_READ = 0;
    static const int PIPE_WRITE = 1;
    static const int PIPE_COUNT = 2;
    HANDLE _fd[PIPE_COUNT];
};

} // namespace OSAL
