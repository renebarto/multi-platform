#pragma once

namespace Core
{

class Pipe
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
    int Read(void * data, int numBytes);
    int Write(void * data, int numBytes);
    int ReadFD() { return fd[PipeRead]; }
    int WriteFD() { return fd[PipeWrite]; }

protected:
    static const int PipeRead = 0;
    static const int PipeWrite = 1;
    static const int PipeCount = 2;
    int fd[PipeCount];
};

} // namespace Core

