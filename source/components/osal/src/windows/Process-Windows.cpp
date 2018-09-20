#include "osal/Process.h"

#include <limits>
#include "osal/Exception.h"
#include "osal/Strings.h"

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
    _commandLine(),
    _childPID(0),
    _startupInfo(),
    _exitCode(0)
{
    for (auto const & arg : args)
    {
        _commandLine += arg;
        _commandLine += " ";
    }
    _commandLine = Strings::TrimSpacesRight(_commandLine);
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
    _commandLine(path + " " + commandLine),
    _childPID(0),
    _startupInfo(),
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
    char commandLine[MAX_PATH];
    size_t offset = 0;
    Strings::strcpy(commandLine + offset, _path.c_str());
    offset += _path.length();
    Strings::strcpy(commandLine + offset, " ");
    offset++;
    Strings::strcpy(commandLine + offset, _commandLine.c_str());
    offset += _commandLine.length();
    commandLine[offset] = {};
    _startupInfo = {};
    _startupInfo.cb = sizeof(_startupInfo);
    _startupInfo.hStdInput = _stdInPipe.ReadFD();
    _startupInfo.hStdOutput = _stdOutPipe.WriteFD();
    _startupInfo.hStdError = _stdErrPipe.WriteFD();
    _startupInfo.dwFlags = STARTF_USESTDHANDLES;
    _procInfo = {};
    if (!SetHandleInformation(_stdOutPipe.ReadFD(), HANDLE_FLAG_INHERIT, 0))
        return false;
    if (!SetHandleInformation(_stdErrPipe.ReadFD(), HANDLE_FLAG_INHERIT, 0))
        return false;
    if (!SetHandleInformation(_stdInPipe.WriteFD(), HANDLE_FLAG_INHERIT, 0))
        return false;

    if (!CreateProcessA(_path.c_str(), commandLine, nullptr, nullptr, true, 
                        CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, nullptr, nullptr, &_startupInfo, &_procInfo))
        return false;
    CloseHandle(_procInfo.hThread);
    _procInfo.hThread = INVALID_HANDLE_VALUE;
    return true;
}

bool Process::DidExit()
{
    if (!GetExitCodeProcess(_procInfo.hProcess, &_exitCode))
    {
        ThrowOnError(__func__, __FILE__, __LINE__, GetLastError());
    }
    else
    {
        CloseHandle(_procInfo.hProcess);
        _procInfo.hProcess = INVALID_HANDLE_VALUE;
        return true;
    }
    return false;
}

void Process::WaitForExit()
{
    if (!WaitForSingleObject(_procInfo.hProcess, INFINITE))
    {
        ThrowOnError(__func__, __FILE__, __LINE__, GetLastError());
    }
    if (!GetExitCodeProcess(_procInfo.hProcess, &_exitCode))
    {
        _exitCode = std::numeric_limits<DWORD>::max();
        ThrowOnError(__func__, __FILE__, __LINE__, GetLastError());
    }
    else
    {
        CloseHandle(_procInfo.hProcess);
        _procInfo.hProcess = INVALID_HANDLE_VALUE;
    }
}

void Process::Kill()
{
    if (!TerminateProcess(_procInfo.hProcess, 0))
    {
        ThrowOnError(__func__, __FILE__, __LINE__, GetLastError());
    }
    CloseHandle(_procInfo.hProcess);
    _procInfo.hProcess = INVALID_HANDLE_VALUE;
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
        _stdInPipe.Write(buffer, static_cast<int>(strlen(buffer)));
        _stdInPipe.Write(&Linefeed, 1);
    }
    _stdInPipe.CloseWrite();
}

void Process::ReadStdOut()
{
    char byte;
    DWORD bytesRead;
    _stdOut = {};
    while (ReadFile(_stdOutPipe.ReadFD(), &byte, 1, &bytesRead, nullptr) && bytesRead == 1)
    {
        _stdOut += byte;
        if (byte == '\n')
            break;
    }
}

void Process::ReadStdErr()
{
    char byte;
    DWORD bytesRead;
    _stdErr = {};
    while (ReadFile(_stdErrPipe.ReadFD(), &byte, 1, &bytesRead, nullptr) && bytesRead == 1)
    {
        _stdErr += byte;
        if (byte == '\n')
            break;
    }
}
