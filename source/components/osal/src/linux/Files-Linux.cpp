#include "osal/PlatformDefines.h"

#if defined(LINUX)

#include <ext/stdio_filebuf.h>
#include <glob.h>
#include <unistd.h>

#include "osal/Files.h"
#include "osal/Path.h"

using namespace std;
using namespace OSAL;
using namespace Files;

TemporaryFile::TemporaryFile()
    : _stream()
    , _path()
{
    const int MAX_PATH = 4096;
    char fileMask[MAX_PATH];
    sprintf(fileMask, "%s/testXXXXXX", P_tmpdir);
    int fd = mkstemp(fileMask);
    _path = fileMask;
    __gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::out);
    _stream = std::make_shared<std::iostream>(&filebuf);
}

TemporaryFile::~TemporaryFile()
{
    Close();
    OSAL::Path::MakeSureFileDoesNotExist(_path);
}

void TemporaryFile::Close()
{
    _stream = nullptr;
}

std::iostream * TemporaryFile::GetStream()
{
    return _stream.get();
}

std::string const & TemporaryFile::GetPath() const
{
    return _path;
}

FileList Files::ScanForFiles(const std::string & searchPattern,
                             SearchOption searchOption)
{
    FileInfo::List result;

    if (!GetFilesInCurrentDirectory(result, FullPath(), searchPattern, searchOption))
        return {};

    string wildcardSpecifier = OSAL::Path::CombinePath(FullPath(), "*");
    glob_t globBuffer;

    DirectoryInfo::List subDirectories;

    glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globBuffer);

    for (size_t i = 0; i < globBuffer.gl_pathc; i++)
    {
        string directoryName = OSAL::Path::LastPartOfPath(globBuffer.gl_pathv[i]);
        if ((directoryName == ".") || (directoryName == ".."))
            continue;
        Directory directory(OSAL::Path::FullPath(globBuffer.gl_pathv[i]));

        if (!HaveDirectory(subDirectories, directory.Name()))
            AddDirectory(subDirectories, directory);
        if ((searchOption & SearchOption::Recursive) != 0)
        {
            FileInfo::List subDirectoryResult = directory.ScanForFiles(searchPattern, searchOption);
            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
        }
    }
    globfree(&globBuffer);

    return result;
}

FileList Files::ScanForDirectories(const std::string & searchPattern,
                                   SearchOption searchOption)
{
    DirectoryInfo::List result;

    string wildcardSpecifier = OSAL::Path::CombinePath(FullPath(), searchPattern);
    glob_t globBuffer;

    DirectoryInfo::List subDirectories;

    glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globBuffer);

    for (size_t i = 0; i < globBuffer.gl_pathc; i++)
    {
        string directoryName = OSAL::Path::LastPartOfPath(globBuffer.gl_pathv[i]);
        if ((directoryName == ".") || (directoryName == ".."))
            continue;
        string directoryPath = globBuffer.gl_pathv[i];
        if ((searchOption & SearchOption::IncludeSymbolicLinks) == 0)
        {
            // Resolve symbolic link
            directoryPath = OSAL::Path::FullPath(directoryPath);
        }
        else
        {
            DirectoryInfo tmp(OSAL::Path::FullPath(directoryPath));
            if (!tmp.IsDirectory())
                continue;
        }
        Directory directory(directoryPath);
        if (!HaveDirectory(result, directory.FullPath()) &&
            (((searchOption & SearchOption::IncludeHidden) != 0) ||
            !directory.IsHidden()))
            AddDirectory(result, directory);

        if (!HaveDirectory(subDirectories, directory.Name()))
            AddDirectory(subDirectories, directory);
        if ((searchOption & SearchOption::Recursive) != 0)
        {
            DirectoryInfo::List subDirectoryResult = directory.ScanForDirectories(searchPattern, searchOption);
            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
        }
    }
    globfree(&globBuffer);

    return result;
}

FileList Files::GetFilesInCurrentDirectory(const std::string & path,
                                           const std::string & searchPattern,
                                           SearchOption searchOption)
{
    string wildcardSpecifier = OSAL::Path::CombinePath(path, searchPattern);
    glob_t globBuffer;

    glob(wildcardSpecifier.c_str(), GLOB_PERIOD, nullptr, &globBuffer);

    for (size_t i = 0; i < globBuffer.gl_pathc; i++)
    {
        string filePath = globBuffer.gl_pathv[i];
        if ((searchOption & SearchOption::IncludeSymbolicLinks) == 0)
        {
            // Resolve symbolic link
            filePath = OSAL::Path::FullPath(filePath);
        }
        else
        {
            FileInfo tmp(OSAL::Path::FullPath(filePath));
            if (!tmp.IsRegularFile())
                continue;
        }
        FileInfo fileInfo(filePath);
        if (!fileInfo.IsDirectory() &&
            (((searchOption & SearchOption::IncludeHidden) != 0) ||
             !fileInfo.IsHidden()))
        {
            if (!HaveFile(result, fileInfo.FullPath()))
                AddFile(result, fileInfo);
        }
    }
    globfree(&globBuffer);
    return true;
}

#endif // defined(LINUX)
