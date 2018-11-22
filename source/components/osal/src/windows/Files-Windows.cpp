#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include <cassert>
#include <fstream>
#include "osal/osal.h"

#include "osal/Files.h"
#include "osal/Path.h"

using namespace std;
using namespace OSAL;
using namespace Files;

TemporaryFile::TemporaryFile()
    : _stream()
    , _path()
{
    char directory[MAX_PATH];
    if (!GetTempPathA(sizeof(directory), directory))
        return;
    char filePath[MAX_PATH];
    if (!GetTempFileNameA(directory, "test", 0, filePath))
        return;
    _path = filePath;
    _stream = std::make_shared<std::fstream>(_path);
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

//FileList Files::ScanForFiles(const std::string & directory,
//                             const std::string & searchPattern,
//                             SearchOption searchOption)
//{
//    FileList result;
//
//    if (!GetFilesInCurrentDirectory(result, directory, searchPattern, searchOption))
//        return {};
//
//    string wildcardSpecifier = OSAL::Path::CombinePath(directory, "*");
//    //glob_t globBuffer;
//
//    //DirectoryInfo::List subDirectories;
//
//    //glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globBuffer);
//
//    //for (size_t i = 0; i < globBuffer.gl_pathc; i++)
//    //{
//    //    string directoryName = OSAL::Path::LastPartOfPath(globBuffer.gl_pathv[i]);
//    //    if ((directoryName == ".") || (directoryName == ".."))
//    //        continue;
//    //    Directory directory(OSAL::Path::FullPath(globBuffer.gl_pathv[i]));
//
//    //    if (!HaveDirectory(subDirectories, directory.Name()))
//    //        AddDirectory(subDirectories, directory);
//    //    if ((searchOption & SearchOption::Recursive) != 0)
//    //    {
//    //        FileInfo::List subDirectoryResult = directory.ScanForFiles(searchPattern, searchOption);
//    //        result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
//    //    }
//    //}
//    //globfree(&globBuffer);
//
//    return result;
//}
//
//FileList Files::ScanForDirectories(const std::string & directory,
//                                   const std::string & searchPattern,
//                                   SearchOption searchOption)
//{
//    FileList result;
//
//    string wildcardSpecifier = OSAL::Path::CombinePath(directory, searchPattern);
//    //glob_t globBuffer;
//
//    //DirectoryInfo::List subDirectories;
//
//    //glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globBuffer);
//
//    //for (size_t i = 0; i < globBuffer.gl_pathc; i++)
//    //{
//    //    string directoryName = OSAL::Path::LastPartOfPath(globBuffer.gl_pathv[i]);
//    //    if ((directoryName == ".") || (directoryName == ".."))
//    //        continue;
//    //    string directoryPath = globBuffer.gl_pathv[i];
//    //    if ((searchOption & SearchOption::IncludeSymbolicLinks) == 0)
//    //    {
//    //        // Resolve symbolic link
//    //        directoryPath = OSAL::Path::FullPath(directoryPath);
//    //    }
//    //    else
//    //    {
//    //        DirectoryInfo tmp(OSAL::Path::FullPath(directoryPath));
//    //        if (!tmp.IsDirectory())
//    //            continue;
//    //    }
//    //    Directory directory(directoryPath);
//    //    if (!HaveDirectory(result, directory.FullPath()) &&
//    //        (((searchOption & SearchOption::IncludeHidden) != 0) ||
//    //        !directory.IsHidden()))
//    //        AddDirectory(result, directory);
//
//    //    if (!HaveDirectory(subDirectories, directory.Name()))
//    //        AddDirectory(subDirectories, directory);
//    //    if ((searchOption & SearchOption::Recursive) != 0)
//    //    {
//    //        DirectoryInfo::List subDirectoryResult = directory.ScanForDirectories(searchPattern, searchOption);
//    //        result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
//    //    }
//    //}
//    //globfree(&globBuffer);
//
//    return result;
//}
//
//bool Files::GetFilesInCurrentDirectory(FileList & result,
//                                       const std::string & path,
//                                       const std::string & searchPattern,
//                                       SearchOption searchOption)
//{
//    string wildcardSpecifier = OSAL::Path::CombinePath(path, searchPattern);
//    //glob_t globBuffer;
//
//    //glob(wildcardSpecifier.c_str(), GLOB_PERIOD, nullptr, &globBuffer);
//
//    //for (size_t i = 0; i < globBuffer.gl_pathc; i++)
//    //{
//    //    string filePath = globBuffer.gl_pathv[i];
//    //    if ((searchOption & SearchOption::IncludeSymbolicLinks) == 0)
//    //    {
//    //        // Resolve symbolic link
//    //        filePath = OSAL::Path::FullPath(filePath);
//    //    }
//    //    else
//    //    {
//    //        FileInfo tmp(OSAL::Path::FullPath(filePath));
//    //        if (!tmp.IsRegularFile())
//    //            continue;
//    //    }
//    //    FileInfo fileInfo(filePath);
//    //    if (!fileInfo.IsDirectory() &&
//    //        (((searchOption & SearchOption::IncludeHidden) != 0) ||
//    //         !fileInfo.IsHidden()))
//    //    {
//    //        if (!HaveFile(result, fileInfo.FullPath()))
//    //            AddFile(result, fileInfo);
//    //    }
//    //}
//    //globfree(&globBuffer);
//    return true;
//}

#endif // defined(WIN_MSVC)
