#include "osal/Path.h"

#if defined(UNICODE) || defined(_UNICODE)
static const OSAL::Char _Dot = L'.';
static const OSAL::Char _Slash = L'/';
static const OSAL::Char * _CurrentDir = L".";
static const OSAL::Char * _ParentDir = L"..";
#else
static const OSAL::Char _Dot = _Dot;
static const OSAL::Char * _CurrentDir = ".";
static const OSAL::Char * _ParentDir = "..";
#endif

void OSAL::Path::SplitPath(const String & path, String & directory, String & fileName)
{
    directory = {};
    fileName = {};
    if (DirectoryExists(path))
    {
        directory = path;
        return;
    }
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != String::npos)
    {
        fileName = path.substr(lastPathDelimiterPos + 1);
        directory = path.substr(0, lastPathDelimiterPos);
        return;
    }
    fileName = path.substr(0, lastPathDelimiterPos);
}

OSAL::String OSAL::Path::Extension(const String & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(_Dot);
    if (lastPathDelimiterPos != String::npos)
    {
        return path.substr(lastPathDelimiterPos);
    }
    return String();
}

OSAL::String OSAL::Path::StripExtension(const String & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(_Dot);
    if (lastPathDelimiterPos != String::npos)
    {
        return path.substr(0, lastPathDelimiterPos);
    }
    return path;
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

bool OSAL::Path::HasNoPath(const String & path)
{
    String directory;
    String fileName;
    Path::SplitPath(path, directory, fileName);
    return directory.empty();
}

OSAL::String OSAL::Path::CreatePathWithDefault(const String & pathOrFileName, const String & defaultPath)
{
    if (HasNoPath(pathOrFileName))
    {
        return CombinePath(defaultPath, pathOrFileName);
    }
    return pathOrFileName;
}

OSAL::String OSAL::Path::CombinePath(const String & basePath, const String & subPath)
{
    String result = basePath;
    if (result.empty())
        result += _CurrentDir;
    if (result[result.length() - 1] != PathSeparator())
        result += PathSeparator();
    result += subPath;
    return result;
}

OSAL::String OSAL::Path::RelativePath(const String & path)
{
    String currentDir = CurrentDir();
    String fullPath = FullPath(path);
    size_t index = 0;
    while ((index < currentDir.length()) && (index < fullPath.length()) && (currentDir[index] == fullPath[index]))
        index++;
	String relativePath = {};
    if (index == 1) // All paths start with /
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
            if ((fullPath.length() > 0) && (fullPath[0] == PathSeparator()))
                fullPath = fullPath.substr(1); // Split was before /
        }
        else
        {
            size_t pathDelimiterPos = String::npos;
            do
            {
                pathDelimiterPos = currentDir.find_last_of(_Slash);
                if (pathDelimiterPos != String::npos)
                {
                    currentDir = currentDir.substr(0, pathDelimiterPos);
                    relativePath = AddSlashIfNeeded(relativePath);
                    relativePath += _ParentDir;
                }
            }
            while (pathDelimiterPos != String::npos);
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

OSAL::String OSAL::Path::StripPathToSubDirectory(const String & path, const String & subDirectoryName)
{
    String strippedPath = path;
    bool done = false;
    while (!done)
    {
        size_t index = strippedPath.find_last_of(PathSeparator());
        if (index != String::npos)
        {
            if (strippedPath.substr(index + 1) == subDirectoryName)
            {
                done = true;
            }
            else
            {
                strippedPath = strippedPath.substr(0, index);
            }
        }
        else
        {
			return {};
        }
    }
    return strippedPath;
}

