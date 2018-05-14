#include "core/files/Directory.h"

#include <algorithm>
#include <glob.h>
#include <unistd.h>
#include "osal/Exception.h"
#include "osal/Path.h"

using namespace std;
using namespace Core;
using namespace Files;

//DirectoryInfo::DirectoryInfo(const string & path)
//    : FileSystemInfo(path)
//    , _subDirectories()
//    , _files()
//{
//}
//
//DirectoryInfo::DirectoryInfo(const DirectoryInfo & other)
//    : FileSystemInfo(other._fullPath)
//    , _subDirectories(other._subDirectories)
//    , _files(other._files)
//{
//}
//
//DirectoryInfo::DirectoryInfo(DirectoryInfo && other)
//    : FileSystemInfo(std::move(other._fullPath))
//    , _subDirectories(std::move(other._subDirectories))
//    , _files(std::move(other._files))
//{
//}
//
//DirectoryInfo & DirectoryInfo::operator = (const DirectoryInfo & other)
//{
//    if (&other != this)
//    {
//        FileSystemInfo::Copy(other);
//        _subDirectories = other._subDirectories;
//        _files = other._files;
//    }
//    return *this;
//}
//
//DirectoryInfo & DirectoryInfo::operator = (DirectoryInfo && other)
//{
//    if (&other != this)
//    {
//        FileSystemInfo::Move(std::move(other));
//        _subDirectories = std::move(other._subDirectories);
//        _files = std::move(other._files);
//    }
//    return *this;
//}
//
//bool DirectoryInfo::Exists() const
//{
//    return Exists(FullPath());
//}
//
//void DirectoryInfo::CreateSubdirectory(const std::string & name)
//{
//    return CreateDirectory(OSAL::Path::CombinePath(FullPath(), name));
//}
//
//void DirectoryInfo::Create()
//{
//    CreateDirectory(FullPath());
//}
//
//FileInfo::List DirectoryInfo::GetFiles(SearchOption searchOption) const
//{
//    return GetFiles("*", searchOption);
//}
//
//FileInfo::List DirectoryInfo::GetFiles(const std::string & searchPattern,
//                                       SearchOption searchOption) const
//{
//    FileInfo::List result;
//
//    GetFilesInCurrentDirectory(result, FullPath(), searchPattern, searchOption);
//
//    string wildcardSpecifier = OSAL::Path::CombinePath(FullPath(), "*");
//    glob_t globbuf;
//
//    glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globbuf);
//
//    for (size_t i = 0; i < globbuf.gl_pathc; i++)
//    {
//        string directoryName = OSAL::Path::LastPartOfPath(globbuf.gl_pathv[i]);
//        if ((directoryName == ".") || (directoryName == ".."))
//            continue;
//        DirectoryInfo directoryInfo(OSAL::Path::FullPath(globbuf.gl_pathv[i]));
//
//        if (!HaveSubDirectory(directoryInfo.Name()))
//            AddSubDirectory(directoryInfo);
//        if ((searchOption & SearchOption::Recursive) != 0)
//        {
//            FileInfo::List subDirectoryResult = directoryInfo.GetFiles(searchPattern, searchOption);
//            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
//        }
//    }
//    globfree(&globbuf);
//
//    return result;
//}
//
//DirectoryInfo::List DirectoryInfo::GetDirectories() const
//{
//    return GetDirectories("*", SearchOption::Normal);
//}
//
//DirectoryInfo::List DirectoryInfo::GetDirectories(const std::string & searchPattern,
//                                                  SearchOption searchOption) const
//{
//    DirectoryInfo::List result;
//
//    string wildcardSpecifier = OSAL::Path::CombinePath(FullPath(), searchPattern);
//    glob_t globbuf;
//
//    glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globbuf);
//
//    for (size_t i = 0; i < globbuf.gl_pathc; i++)
//    {
//        string directoryName = OSAL::Path::LastPartOfPath(globbuf.gl_pathv[i]);
//        if ((directoryName == ".") || (directoryName == ".."))
//            continue;
//        DirectoryInfo directoryInfo(OSAL::Path::FullPath(globbuf.gl_pathv[i]));
//        if (!HaveSubDirectory(result, directoryInfo.Name()))
//            AddSubDirectory(result, directoryInfo);
//
//        if (!HaveSubDirectory(directoryInfo.Name()))
//            AddSubDirectory(directoryInfo);
//        if ((searchOption & SearchOption::Recursive) != 0)
//        {
//            DirectoryInfo::List subDirectoryResult = directoryInfo.GetDirectories(searchPattern, searchOption);
//            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
//        }
//    }
//    globfree(&globbuf);
//
//    return result;
//}
//
//void DirectoryInfo::MoveTo(const std::string & destination)
//{
//    Move(FullPath(), destination);
//}
//
//void DirectoryInfo::Delete()
//{
//    Delete(FullPath());
//}
//
//void DirectoryInfo::AddSubDirectory(const DirectoryInfo & directory) const
//{
//    AddSubDirectory(_subDirectories, directory);
//}
//
//bool DirectoryInfo::HaveSubDirectory(const std::string & name) const
//{
//    return HaveSubDirectory(_subDirectories, name);
//}
//
//bool DirectoryInfo::Exists(const std::string & path)
//{
//    return OSAL::Path::DirectoryExists(path);
//}
//
//void DirectoryInfo::CreateDirectory(const std::string & path)
//{
//    OSAL::Path::MakeSureDirectoryExists(path);
//}
//
//void DirectoryInfo::Move(const std::string & source, const std::string & destination)
//{
//    if (rename(source.c_str(), destination.c_str()) != 0)
//        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
//}
//
//void DirectoryInfo::Delete(const std::string & path)
//{
//    unlink(path.c_str());
//}
//
//void DirectoryInfo::AddSubDirectory(DirectoryInfo::List & list, const DirectoryInfo & directory)
//{
//    list.push_back(directory);
//}
//
//bool DirectoryInfo::HaveSubDirectory(DirectoryInfo::List & list, const std::string & name)
//{
//    for (auto const & directory : list)
//    {
//        if (directory.Name() == name)
//            return true;
//    }
//    return false;
//}
//
//void DirectoryInfo::GetFilesInCurrentDirectory(FileInfo::List & result,
//                                               const string & path,
//                                               const string & searchPattern,
//                                               SearchOption searchOption)
//{
//    string wildcardSpecifier = OSAL::Path::CombinePath(path, searchPattern);
//    glob_t globbuf;
//
//    glob(wildcardSpecifier.c_str(), GLOB_PERIOD, nullptr, &globbuf);
//
//    for (size_t i = 0; i < globbuf.gl_pathc; i++)
//    {
//        FileInfo fileInfo(OSAL::Path::FullPath(globbuf.gl_pathv[i]));
//        if (!fileInfo.IsDirectory() &&
//            (((searchOption & SearchOption::IncludeHidden) != 0) ||
//             !fileInfo.IsHiddenFile()))
//            result.push_back(fileInfo);
//    }
//    globfree(&globbuf);
//}
