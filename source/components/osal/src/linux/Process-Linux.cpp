#include "osal/Process.h"

#include <sys/wait.h>
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;

Process::Process(const std::string & path, const CommandArgs & args):
    _stdIn(new stringstream()),
    _isStdInOwned(true),
    _stdOut(),
    _stdErr(),
    _stdInPipe(),
    _stdOutPipe(),
    _stdErrPipe(),
    _path(path),
    _args(path, args),
    _childPID(-1),
    _exitCode(0)
{
}

Process::Process(const std::string & path, const std::string & commandLine):
    _stdIn(new stringstream()),
    _isStdInOwned(true),
    _stdOut(),
    _stdErr(),
    _stdInPipe(),
    _stdOutPipe(),
    _stdErrPipe(),
    _path(path),
    _args(path, commandLine),
    _childPID(-1),
    _exitCode(0)
{
}

Process::~Process()
{
    if (_isStdInOwned)
        delete _stdIn;
}

bool Process::Start()
{
    _stdInPipe.Create();
    _stdOutPipe.Create();
    _stdErrPipe.Create();
    _childPID = fork();
    if (_childPID < 0)
        return false;
    if (_childPID == 0)
    {
        _stdInPipe.CloseWrite();
        _stdOutPipe.CloseRead();
        _stdErrPipe.CloseRead();
        dup2(_stdInPipe.ReadFD(), STDIN_FILENO);
        dup2(_stdOutPipe.WriteFD(), STDOUT_FILENO);
        dup2(_stdErrPipe.WriteFD(), STDERR_FILENO);
        execvp(_path.c_str(), _args.GetArgv());
    }
    else
    {
        _stdInPipe.CloseRead();
        _stdOutPipe.CloseWrite();
        _stdErrPipe.CloseWrite();
    }
    return true;
}

bool Process::DidExit()
{
    int status;
    int errorCode = waitpid(_childPID, &status, WNOHANG);
    if (errorCode == -1)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    if ((errorCode != 0) && WIFEXITED(status))
    {
        _exitCode = WEXITSTATUS(status);
        return true;
    }
    return false;
}

void Process::WaitForExit()
{
    int status;
    int errorCode = waitpid(_childPID, &status, 0);
    if (errorCode == -1)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    if (WIFEXITED(status))
        _exitCode = WEXITSTATUS(status);
    else
        _exitCode = -1;
}

void Process::Kill()
{
    kill(_childPID, SIGINT);
}

void Process::RedirectStdIn(std::iostream * stream)
{
    if (_isStdInOwned)
    {
        delete _stdIn;
        _isStdInOwned = false;
    }
    _stdIn = stream;
}

void Process::WriteInputLine(const string & line)
{
    *_stdIn << line << endl;
}

void Process::WriteStdIn()
{
    const int BufferSize = 65536;
    char buffer[BufferSize];
    char Linefeed = '\n';
    _stdIn->seekg(0);
    while (_stdIn->getline(buffer, BufferSize))
    {
        _stdInPipe.Write(buffer, strlen(buffer));
        _stdInPipe.Write(&Linefeed, 1);
    }
    _stdInPipe.CloseWrite();
}

void Process::ReadStdOut()
{
    char byte;
    _stdOut = {};
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(_stdOutPipe.ReadFD(), &fdset);
    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    int result = select(_stdOutPipe.ReadFD() + 1, &fdset, NULL, NULL, &timeout);
    if (result == 0)
    {
        return;
    }
    while (_stdOutPipe.Read(&byte, 1) == 1)
    {
        _stdOut += byte;
        if (byte == '\n')
            break;
    }
}

void Process::ReadStdErr()
{
    char byte;
    _stdErr = {};
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(_stdErrPipe.ReadFD(), &fdset);
    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    int result = select(_stdErrPipe.ReadFD() + 1, &fdset, NULL, NULL, &timeout);
    if (result == 0)
    {
        return;
    }
    while (_stdErrPipe.Read(&byte, 1) == 1)
    {
        _stdErr += byte;
        if (byte == '\n')
            break;
    }
}
