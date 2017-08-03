#include "osal/Path.h"

#include <climits>
#include <direct.h>
#include <sys/stat.h>
#include "osal/OSAL.h"

static const OSAL::Char _PathSeparator = _('\\');
static const OSAL::Char * _TildeDir = _("~\\");

OSAL::Char OSAL::Path::PathSeparator()
{
    return _PathSeparator;
}

OSAL::String OSAL::Path::AddSlashIfNeeded(const String & path)
{
    String result = path;
    if ((path.length() > 0) && (path[path.length() - 1] != _PathSeparator))
        result += _PathSeparator;
    return result;
}

bool OSAL::Path::FileExists(const String & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(ToNarrowString(path).c_str(), &status);
    return (S_ISREG(status.st_mode));
}

bool OSAL::Path::DirectoryExists(const String & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
	stat(ToNarrowString(path).c_str(), &status);
	return (S_ISDIR(status.st_mode));
}

void OSAL::Path::MakeSureDirectoryExists(const String & path)
{
    if (FileExists(path))
    {
        _unlink(ToNarrowString(path).c_str());
    }
}

void OSAL::Path::MakeSureFileDoesNotExist(const String & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
	const char * filePath = ToNarrowString(path).c_str();
    stat(filePath, &status);
    if (S_ISDIR(status.st_mode))
        return;
    if (S_ISREG(status.st_mode))
    {
		_unlink(filePath);
    }
    _mkdir(filePath);
}

OSAL::String OSAL::Path::ResolveTilde(const String & path)
{
    if (path.length() < 2)
        return path;
    if (path.substr(0, 2) != _TildeDir)
        return path;
#if defined(WIN_MSVC)
    OSAL::String home = OSAL::System::getenv(_("USERPROFILE"));
#else
    OSAL::String home = OSAL::System::getenv(_("HOME"));
#endif
    return CombinePath(home, path.substr(2));
}

OSAL::String OSAL::Path::FullPath(const String & path)
{
    Char buffer[PATH_MAX];
    String resolvedPath = ResolveTilde(path);

#if defined(UNICODE) || defined(_UNICODE)
    const Char * fullpath = _wfullpath(buffer, resolvedPath.c_str(), sizeof(buffer) / sizeof(Char));
#else
    const Char * fullpath = _fullpath(buffer, resolvedPath.c_str(), sizeof(buffer) / sizeof(Char));
#endif
    if (fullpath == nullptr)
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return String(fullpath);
}

OSAL::String OSAL::Path::CurrentDir()
{
    char * currentDirectory = _getcwd(nullptr, 0);
    if (currentDirectory == nullptr)
    {
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
    }
    String result = ToString(currentDirectory);
    free(currentDirectory);
    return FullPath(result);
}

