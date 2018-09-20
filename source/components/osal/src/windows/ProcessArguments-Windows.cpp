#include "osal/ProcessArguments.h"

#include "osal/Unused.h"
#include "osal/Strings.h"

using namespace std;
using namespace OSAL;

ProcessArguments::ProcessArguments(const string & programPath, const string & commandLine)
{
    int argc = CountEntries(programPath, commandLine);

    _args = new char *[argc];

    size_t index = 0;
    size_t count = programPath.length();
    CopyItem(index, programPath, count);
    index++;

    const char * command = commandLine.c_str();
    while ((command != 0) && (*command != '\0'))
    {
        const char * prevCommand = command;
        command = OSAL::Strings::strchr(command, ' ');
        if (command != 0)
        {
            count = command - prevCommand;
            command++;
        }
        else
        {
            count = OSAL::Strings::strlen(prevCommand);
        }
        CopyItem(index, prevCommand, count);
        index++;
    }
    _args[index] = 0;
}

ProcessArguments::ProcessArguments(const string & programPath, const CommandArgs & commandArgs)
{
    _args = 0;
    int argc = 2 + commandArgs.size();

    _args = new char *[argc];
    size_t index = 0;
    size_t count = programPath.length();
    CopyItem(index, programPath, count);
    index++;

    for (auto str : commandArgs)
    {
        count = str.length();
        CopyItem(index, str, count);
        index++;
    }
    _args[index] = 0;
}

ProcessArguments::~ProcessArguments()
{
    if (_args != 0)
    {
        size_t index = 0;
        while (_args[index] != 0)
        {
            delete [] _args[index];
            index++;
        }
        delete [] _args;
        _args = 0;
    }
}

char * const * ProcessArguments::GetArgv()
{
    return _args;
}

std::string ProcessArguments::GetFullCommandLine() const
{
    std::string result;
    size_t index = 0;
    while (_args[index] != 0)
    {
        result += _args[index++];
        if (_args[index] != 0)
            result += " ";
    }
    return result;
}

size_t ProcessArguments::CountEntries(const string & UNUSED(programPath), const string & commandLine)
{
    size_t argc = 1;
    const char * command = commandLine.c_str();
    while ((command != 0) && (*command != '\0'))
    {
        argc++;
        command = OSAL::Strings::strchr(command, ' ');
        if (command != 0)
            command++;
    }
    argc++;
    return argc;
}

void ProcessArguments::CopyItem(size_t index, const string & programPath, size_t count)
{
    _args[index] = new char[count + 1];
    OSAL::Strings::strncpy(_args[index], programPath.c_str(), count);
    _args[index][count] = '\0';
}
