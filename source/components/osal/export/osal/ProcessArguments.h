#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace OSAL {

typedef std::vector<std::string> CommandArgs;

class ProcessArguments
{
public:
    ProcessArguments() = delete;
    ProcessArguments(const ProcessArguments &) = delete;
    ProcessArguments(const std::string & programPath, const std::string & commandLine);
    ProcessArguments(const std::string & programPath, const CommandArgs & commandArgs);
    ~ProcessArguments();

    ProcessArguments & operator = (const ProcessArguments &) = delete;

    char * const * GetArgv();
    std::string GetFullCommandLine() const;

protected:
    char ** _args;

    size_t CountEntries(const std::string & programPath, const std::string & commandLine);
    void CopyItem(size_t index, const std::string & programPath, size_t count);
};

} // namespace OSAL
