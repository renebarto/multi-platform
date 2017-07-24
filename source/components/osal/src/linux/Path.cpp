#include "osal/Path.h"

#include <climits>
#include <unistd.h>
#include <sys/stat.h>
#include "osal/OSAL.h"

static const OSAL::Char _PathSeparator = _('/');

OSAL::String OSAL::Path::AddSlashIfNeeded(const String & path)
{
    String result = path;
    if ((path.length() > 0) && (path[path.length() - 1] != _PathSeparator))
        result += _PathSeparator;
    return result;
}

OSAL::Char OSAL::Path::PathSeparator()
{
    return _PathSeparator;
}

bool OSAL::Path::FileExists(const String & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    return (S_ISREG(status.st_mode));
}

bool OSAL::Path::DirectoryExists(const String & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    return (S_ISDIR(status.st_mode));
}

void OSAL::Path::MakeSureDirectoryExists(const String & path)
{
    if (FileExists(path))
    {
        unlink(path.c_str());
    }
}

void OSAL::Path::MakeSureFileDoesNotExist(const String & path)
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

OSAL::String OSAL::Path::ResolveTilde(const String & path)
{
    if (path.length() < 2)
        return path;
    if (path.substr(0, 2) != "~/")
        return path;
    return CombinePath(String(getenv("HOME")), path.substr(2));
}

OSAL::String OSAL::Path::FullPath(const String & path)
{
    char buffer[PATH_MAX];
    String resolvedPath = ResolveTilde(path);
    const char * fullpath = realpath(resolvedPath.c_str(), buffer);
    if (fullpath == nullptr)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return String(fullpath);
}

OSAL::String OSAL::Path::CurrentDir()
{
    char * currentDirectory = get_current_dir_name();
    if (currentDirectory == nullptr)
    {
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    }
    String result(currentDirectory);
    free(currentDirectory);
    return FullPath(result);
}

