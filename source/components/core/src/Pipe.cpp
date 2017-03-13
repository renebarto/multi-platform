#include "core/Pipe.h"

#include <unistd.h>
#include "core/core.h"

using namespace std;
using namespace Core;

Pipe::Pipe()
{
    fd[PipeRead] = -1;
    fd[PipeWrite] = -1;
}

Pipe::Pipe(int fdRead, int fdWrite)
{
    fd[PipeRead] = fdRead;
    fd[PipeWrite] = fdWrite;
}

Pipe::~Pipe()
{
    Close();
}

void Pipe::Create()
{
    int err = pipe(fd);
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
    if (fd[PipeRead] != -1)
        close(fd[PipeRead]);
    fd[PipeRead] = -1;
}

void Pipe::CloseWrite()
{
    if (fd[PipeWrite] != -1)
        close(fd[PipeWrite]);
    fd[PipeWrite] = -1;
}

int Pipe::Read(void * data, int numBytes)
{
    return read(fd[PipeRead], data, numBytes);
}

int Pipe::Write(void * data, int numBytes)
{
    return write(fd[PipeWrite], data, numBytes);
}
