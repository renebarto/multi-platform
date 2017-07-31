#include "core/Pipe.h"

#include "core/Core.h"
#include "osal/Pipe.h"
#include "osal/Files.h"

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
    int err = OSAL::Pipe::pipe(_fd);
    if (err < 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void Pipe::Close()
{
    CloseRead();
    CloseWrite();
}

void Pipe::CloseRead()
{
    if (_fd[PipeRead] != -1)
        OSAL::Files::close(_fd[PipeRead]);
    _fd[PipeRead] = -1;
}

void Pipe::CloseWrite()
{
    if (_fd[PipeWrite] != -1)
		OSAL::Files::close(_fd[PipeWrite]);
    _fd[PipeWrite] = -1;
}

ssize_t Pipe::Read(void * data, size_t numBytes)
{
    return OSAL::Files::read(_fd[PipeRead], data, numBytes);
}

ssize_t Pipe::Write(void * data, size_t numBytes)
{
    return OSAL::Files::write(_fd[PipeWrite], data, numBytes);
}
