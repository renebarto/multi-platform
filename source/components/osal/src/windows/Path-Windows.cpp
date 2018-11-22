#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include "osal/osal.h"
WARNING_PUSH
WARNING_DISABLE(4668)
#include <osal/windows/unistd.h>
#include <osal/windows/dirent.h>
WARNING_POP
#include <sys/stat.h>
#include <climits>
#include <cstring>

#include "osal/osal.h"
//#include "osal/Exception.h"
#include "osal/Path.h"
#include "osal/Strings.h"
#include "osal/System.h"

using namespace std;

static const char _PathSeparator = '/';
static const char _Slash = '/';
static const char * _CurrentDir = ".";
static const char * _ParentDir = "..";

string OSAL::Path::AddSeparatorIfNeeded(const string & path)
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

bool OSAL::Path::MakeSureDirectoryExists(const string & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    if (S_ISDIR(status.st_mode))
        return true;
    if (S_ISREG(status.st_mode))
    {
        if (RemoveDirectory(path.c_str()) != 0)
            return false;
    }
    MkDir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return true;
}

bool OSAL::Path::MakeSureFileDoesNotExist(const string & path)
{
    if (FileExists(path))
    {
        _unlink(path.c_str());
        return false;
    }
    return true;
}

//string OSAL::Path::FullPath(const string & path)
//{
//    char fullpath[PATH_MAX];
//    string resolvedPath = ResolveTilde(path);
//    if (!GetFullPathNameA(resolvedPath.c_str(), sizeof(fullpath), fullpath, nullptr))
//        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
//    return fullpath;
//}
//
//string OSAL::Path::CurrentDir()
//{
//    char currentDirectory[PATH_MAX];
//    if (!GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory))
//    {
//        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
//    }
//    return FullPath(currentDirectory);
//}
//
//string OSAL::Path::RelativePath(const string & path)
//{
//    string currentDir = CurrentDir();
//    string fullPath = FullPath(path);
//    size_t index = 0;
//    while ((index < currentDir.length()) && (index < fullPath.length()) && (currentDir[index] == fullPath[index]))
//        index++;
//    string relativePath = {};
//#if defined(WIN_MSVC)
//    #if defined(UNICODE) || defined(_UNICODE)
//    wchar_t drive[_MAX_DRIVE];
//    wchar_t dir[_MAX_DIR];
//    _wsplitpath(Strings::StringToWString(fullPath).c_str(), drive, dir, nullptr, nullptr);
//    bool isAbsolutePath = ((wcslen(drive) != 0) && (index <= wcslen(drive))) ||
//                           ((wcslen(dir) != 0) && (dir[0] == _PathSeparator) && (index <= wcslen(drive) + 1));
//#else
//    char drive[_MAX_DRIVE];
//    char dir[_MAX_DIR];
//    _splitpath(fullPath.c_str, drive, dir, nullptr, nullptr);
//    bool isAbsolutePath = (strlen(drive) != 0) || ((strlen(dir) != 0) && (dir[0] == _PathSeparator));
//#endif
//#else
//    bool isAbsolutePath = (index == 1); // All paths start with /
//#endif
//    if (isAbsolutePath)
//    {
//        relativePath = fullPath;
//    }
//    else
//    {
//        currentDir = currentDir.substr(index);
//        fullPath = fullPath.substr(index);
//        if (currentDir.empty())
//        {
//            relativePath = _CurrentDir;
//            if ((fullPath.length() > 0) && (fullPath[0] == _PathSeparator))
//                fullPath = fullPath.substr(1); // Split was before /
//        }
//        else
//        {
//            size_t pathDelimiterPos = string::npos;
//            do
//            {
//                pathDelimiterPos = currentDir.find_last_of(_PathSeparator);
//                if (pathDelimiterPos != string::npos)
//                {
//                    currentDir = currentDir.substr(0, pathDelimiterPos);
//                    relativePath = AddSeparatorIfNeeded(relativePath);
//                    relativePath += _ParentDir;
//                }
//            }
//            while (pathDelimiterPos != string::npos);
//            if (!currentDir.empty())
//            {
//                relativePath = AddSeparatorIfNeeded(relativePath);
//                relativePath += _ParentDir;
//            }
//        }
//        if (!fullPath.empty())
//        {
//            relativePath = AddSeparatorIfNeeded(relativePath);
//            relativePath += fullPath;
//        }
//    }
//
//    return relativePath;
//}
//
//string OSAL::Path::HomePath()
//{
//    const char * homeDir = OSAL::System::GetEnvironmentVariable("USERPROFILE");
//    return homeDir ? homeDir : "";
//}
//
//string OSAL::Path::SystemBinariesPath()
//{
//    return "/usr/bin";
//}
//
//string OSAL::Path::SystemLibrariesPath()
//{
//    return "/usr/lib";
//}
//
string OSAL::Path::TempPath()
{
    const char * tempFolder = OSAL::System::GetEnvironment("TMP");
    return tempFolder ? tempFolder : "";
}

std::string OSAL::Path::ResolveSymbolicLink(const std::string & path)
{
    std::string result = path;
    bool done = false;
    while (!done)
    {
        //constexpr size_t BufferSize = 4096;
        //char buffer[BufferSize];
        //ssize_t returnValue = readlink(result.c_str(), buffer, BufferSize);
        //if (returnValue < 0)
        //{
        //    if (errno == EINVAL)
        //    {
                done = true;
        //        continue;
        //    }
        //    ThrowOnError(__func__, __FILE__, __LINE__, errno);
        //}
        //buffer[returnValue] = '\0';
        //result = buffer;
    }
    return result;
}

#endif // defined(WIN_MSVC)
