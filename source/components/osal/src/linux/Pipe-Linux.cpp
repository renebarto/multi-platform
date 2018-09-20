#include "osal/Pipe.h"

using namespace std;
using namespace OSAL;

Pipe::Pipe()
{
    _fd[PIPE_READ] = -1;
    _fd[PIPE_WRITE] = -1;
}

Pipe::Pipe(Files::FileDescriptor fdRead, Files::FileDescriptor fdWrite)
{
    _fd[PIPE_READ] = fdRead;
    _fd[PIPE_WRITE] = fdWrite;
}

Pipe::~Pipe()
{
    Close();
}

int Pipe::Create()
{
    return ::pipe(_fd);
    return Files::CreatePipe(_fd);
}

void Pipe::Close()
{
    CloseRead();
    CloseWrite();
}

void Pipe::CloseRead()
{
    if (_fd[PIPE_READ] != -1)
        Files::Close(_fd[PIPE_READ]);
    _fd[PIPE_READ] = -1;
}

void Pipe::CloseWrite()
{
    if (_fd[PIPE_WRITE] != -1)
        Files::Close(_fd[PIPE_WRITE]);
    _fd[PIPE_WRITE] = -1;
}

ssize_t Pipe::Read(void * data, ssize_t numBytes)
{
    return Files::Read(_fd[PIPE_READ], data, static_cast<size_t>(numBytes));
}

ssize_t Pipe::Write(void * data, ssize_t numBytes)
{
    return Files::Write(_fd[PIPE_WRITE], data, static_cast<size_t>(numBytes));
}
