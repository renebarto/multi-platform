#pragma once

#include <core/Core.h>
#include <osal/Files.h>

namespace Core
{

class CORE_EXPORT Pipe
{
public:
    Pipe();
    Pipe(const Pipe &) = delete;
    Pipe(int fdRead, int fdWrite);
    ~Pipe();

    Pipe & operator = (const Pipe &) = delete;

    void Create();
    void Close();
    void CloseRead();
    void CloseWrite();
	ssize_t Read(void * data, size_t numBytes);
	ssize_t Write(void * data, size_t numBytes);
    int ReadFD() { return _fd[PipeRead]; }
    int WriteFD() { return _fd[PipeWrite]; }

protected:
    static const int PipeRead = 0;
    static const int PipeWrite = 1;
    static const int PipeCount = 2;
    int _fd[PipeCount];
};

} // namespace Core

