#include "osal/Pipe.h"

#if defined(WIN_MINGW)

using namespace std;
using namespace OSAL;

const FileHandle OSAL::InvalidHandle = INVALID_HANDLE_VALUE;

Pipe::Pipe()
{
    _fd[PIPE_READ] = OSAL::InvalidHandle;
    _fd[PIPE_WRITE] = OSAL::InvalidHandle;
}

Pipe::Pipe(OSAL::FileHandle fdRead, OSAL::FileHandle fdWrite)
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
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    return !CreatePipe(&_fd[0], &_fd[1], &sa, 0);
}

void Pipe::Close()
{
    CloseRead();
    CloseWrite();
}

void Pipe::CloseRead()
{
    if (_fd[PIPE_READ] != INVALID_HANDLE_VALUE)
        CloseHandle(_fd[PIPE_READ]);
    _fd[PIPE_READ] = INVALID_HANDLE_VALUE;
}

void Pipe::CloseWrite()
{
    if (_fd[PIPE_WRITE] != INVALID_HANDLE_VALUE)
        CloseHandle(_fd[PIPE_WRITE]);
    _fd[PIPE_WRITE] = INVALID_HANDLE_VALUE;
}

ssize_t Pipe::Read(void * data, ssize_t numBytes)
{
    DWORD bytesRead;
    if (!ReadFile(_fd[PIPE_READ], data, static_cast<size_t>(numBytes), &bytesRead, nullptr))
        return 0;
    return static_cast<ssize_t>(bytesRead);
}

ssize_t Pipe::Write(void * data, ssize_t numBytes)
{
    DWORD bytesWritten;
    if (!WriteFile(_fd[PIPE_WRITE], data, static_cast<size_t>(numBytes), &bytesWritten, nullptr))
        return 0;
    return static_cast<ssize_t>(bytesWritten);
}

#endif // defined(WIN_MINGW)
