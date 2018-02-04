#include "osal/PlatformDefines.h"

#if defined(LINUX)

#include <unistd.h>
#include <sys/stat.h>
#include <climits>
#include <cstring>

#include "osal/osal.h"
#include "osal/Exception.h"
#include "osal/Path.h"

using namespace std;

static const char _PathSeparator = '/';
static const char _Slash = '/';
static const char * _CurrentDir = ".";
static const char * _ParentDir = "..";

string OSAL::Path::AddSlashIfNeeded(const string & path)
{
    string result = path;
    if ((path.length() > 0) && (path[path.length() - 1] != _PathSeparator))
        result += _PathSeparator;
    return result;
}

char OSAL::Path::PathSeparator()
{
    return _PathSeparator;
}

bool OSAL::Path::FileExists(const string & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    return (S_ISREG(status.st_mode));
}

bool OSAL::Path::DirectoryExists(const string & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    return (S_ISDIR(status.st_mode));
}

void OSAL::Path::MakeSureDirectoryExists(const string & path)
{
    if (FileExists(path))
    {
        unlink(path.c_str());
    }
}

void OSAL::Path::MakeSureFileDoesNotExist(const string & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    if (S_ISDIR(status.st_mode))
        return;
    if (S_ISREG(status.st_mode))
    {
        unlink(path.c_str());
    }
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

string OSAL::Path::FullPath(const string & path)
{
    char buffer[PATH_MAX];
    string resolvedPath = ResolveTilde(path);
    const char * fullpath = realpath(resolvedPath.c_str(), buffer);
    if (fullpath == nullptr)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return string(fullpath);
}

string OSAL::Path::CurrentDir()
{
    char * currentDirectory = get_current_dir_name();
    if (currentDirectory == nullptr)
    {
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    }
    string result(currentDirectory);
    free(currentDirectory);
    return FullPath(result);
}

string OSAL::Path::RelativePath(const string & path)
{
    string currentDir = CurrentDir();
    string fullPath = FullPath(path);
    size_t index = 0;
    while ((index < currentDir.length()) && (index < fullPath.length()) && (currentDir[index] == fullPath[index]))
        index++;
    string relativePath = {};
#if defined(WIN_MSVC)
    #if defined(UNICODE) || defined(_UNICODE)
    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    _wsplitpath(fullPath.c_str(), drive, dir, nullptr, nullptr);
    bool isAbsolutePath = ((wcslen(drive) != 0) && (index <= wcslen(drive))) ||
                           ((wcslen(dir) != 0) && (dir[0] == _PathSeparator) && (index <= wcslen(drive) + 1));
#else
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath(fullPath.c_str, drive, dir, nullptr, nullptr);
    bool isAbsolutePath = (strlen(drive) != 0) || ((strlen(dir) != 0) && (dir[0] == _PathSeparator));
#endif
#else
    bool isAbsolutePath = (index == 1); // All paths start with /
#endif
    if (isAbsolutePath)
    {
        relativePath = fullPath;
    }
    else
    {
        currentDir = currentDir.substr(index);
        fullPath = fullPath.substr(index);
        if (currentDir.empty())
        {
            relativePath = _CurrentDir;
            if ((fullPath.length() > 0) && (fullPath[0] == _PathSeparator))
                fullPath = fullPath.substr(1); // Split was before /
        }
        else
        {
            size_t pathDelimiterPos = string::npos;
            do
            {
                pathDelimiterPos = currentDir.find_last_of(_PathSeparator);
                if (pathDelimiterPos != string::npos)
                {
                    currentDir = currentDir.substr(0, pathDelimiterPos);
                    relativePath = AddSlashIfNeeded(relativePath);
                    relativePath += _ParentDir;
                }
            }
            while (pathDelimiterPos != string::npos);
            if (!currentDir.empty())
            {
                relativePath = AddSlashIfNeeded(relativePath);
                relativePath += _ParentDir;
            }
        }
        if (!fullPath.empty())
        {
            relativePath = AddSlashIfNeeded(relativePath);
            relativePath += fullPath;
        }
    }

    return relativePath;
}

string OSAL::Path::HomePath()
{
    const char * homeDir = OSAL::System::GetEnvironmentVariable("HOME");
    return homeDir ? homeDir : "";
}

string OSAL::Path::SystemBinariesPath()
{
    return "/usr/bin";
}

string OSAL::Path::SystemLibrariesPath()
{
    return "/usr/lib";
}

#endif // defined(LINUX)
