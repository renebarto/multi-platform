#pragma once

#include "osal/OSAL.h"
#include "osal/exports.h"
#include "osal/ProcessArguments.h"
#include "osal/Pipe.h"

namespace OSAL {

using pid_t = uint32_t;

class OSAL_EXPORT Process
{
public:
    Process() = delete;
    Process(const Process &) = delete;
    Process(const std::string & path, const CommandArgs & args);
    Process(const std::string & path, const std::string & commandLine);
    virtual ~Process();

    Process & operator = (const Process &) = delete;

    bool Start();
    bool DidExit();
    void WaitForExit();
    DWORD ExitCode() const
    {
        return _exitCode;
    }
    void Kill();

    void RedirectStdIn(std::iostream * stream);
    void WriteInputLine(const std::string & line);
    void WriteStdIn();
    void ReadStdOut();
    void ReadStdErr();
    const std::string & StdOut() const
    {
        return _stdOut;
    }
    const std::string & StdErr() const
    {
        return _stdErr;
    }

protected:
    std::iostream * _stdIn;
    bool _isStdInOwned;
    std::string _stdOut;
    std::string _stdErr;
    Pipe _stdInPipe;
    Pipe _stdOutPipe;
    Pipe _stdErrPipe;
    std::string _path;
    std::string _commandLine;
    pid_t _childPID;
    STARTUPINFOA _startupInfo;
    PROCESS_INFORMATION _procInfo;
    DWORD _exitCode;
};

} // namespace OSAL