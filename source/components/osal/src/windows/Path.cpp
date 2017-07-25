#include "osal/Path.h"

#include <climits>
#include <direct.h>
#include <unistd.h>
#include <sys/stat.h>
#include "osal/OSAL.h"

static const Char _PathSeparator = _('/');

OSAL::Char OSAL::Path::PathSeparator()
{
    return _PathSeparator;
}

OSAL::String OSAL::Path::LastPartOfPath(const String & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != String::npos)
    {
        return path.substr(lastPathDelimiterPos + 1);
    }
    return path;
}

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
    _mkdir(path.c_str());
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
    Char buffer[PATH_MAX];
    String resolvedPath = ResolveTilde(path);

#if defined(UNICODE) || defined(_UNICODE)
    const Char * fullpath = _wfullpath(resolvedPath.c_str(), buffer, sizeof(buffer) / sizeof(Char));
#else
    const Char * fullpath = _fullpath(const_cast<Char *>(resolvedPath.c_str()), buffer, sizeof(buffer) / sizeof(Char));
#endif
    if (fullpath == nullptr)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return String(fullpath);
}

OSAL::String OSAL::Path::CurrentDir()
{
    char * currentDirectory = getcwd(nullptr, 0);
    if (currentDirectory == nullptr)
    {
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    }
    String result(currentDirectory);
    free(currentDirectory);
    return FullPath(result);
}

