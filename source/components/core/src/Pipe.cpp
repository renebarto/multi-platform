#include "core/Pipe.h"

#include <unistd.h>
#include "core/Core.h"

using namespace std;
using namespace Core;

Pipe::Pipe()
{
    _fd[PipeRead] = -1;
    _fd[PipeWrite] = -1;
}

Pipe::Pipe(int fdRead, int fdWrite)
{
    _fd[PipeRead] = fdRead;
    _fd[PipeWrite] = fdWrite;
}

Pipe::~Pipe()
{
    Close();
}

void Pipe::Create()
{
    int err = pipe(_fd);
    if (err < 0)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void Pipe::Close()
{
    CloseRead();
    CloseWrite();
}

void Pipe::CloseRead()
{
    if (_fd[PipeRead] != -1)
        close(_fd[PipeRead]);
    _fd[PipeRead] = -1;
}

void Pipe::CloseWrite()
{
    if (_fd[PipeWrite] != -1)
        close(_fd[PipeWrite]);
    _fd[PipeWrite] = -1;
}

int Pipe::Read(void * data, int numBytes)
{
    return read(_fd[PipeRead], data, numBytes);
}

int Pipe::Write(void * data, int numBytes)
{
    return write(_fd[PipeWrite], data, numBytes);
}
