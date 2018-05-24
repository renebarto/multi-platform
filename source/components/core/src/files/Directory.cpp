#include "core/files/Directory.h"

#include <algorithm>
#include <glob.h>
#include <unistd.h>
#include "osal/Exception.h"
#include "osal/Path.h"

using namespace std;
using namespace Core;
using namespace Files;

Directory::Directory(const DirectoryInfo & info)
    : DirectoryInfo(info)
{
}

Directory::Directory(const string & path)
    : DirectoryInfo(path)
{
}

Directory::Directory(const Directory & other)
    : DirectoryInfo(other)
{
}

Directory::Directory(Directory && other)
    : DirectoryInfo(std::move(other))
{
}

Directory & Directory::operator = (const Directory & other)
{
    if (this != &other)
    {
        DirectoryInfo::operator =(other);
    }
    return *this;
}

Directory & Directory::operator = (Directory && other)
{
    if (&other != this)
    {
        DirectoryInfo::operator =(std::move(other));
    }
    return *this;
}

bool Directory::CreateSubdirectory(const std::string & name)
{
    return Create(OSAL::Path::CombinePath(FullPath(), name));
}

bool Directory::Create()
{
    return Create(FullPath());
}

bool Directory::CreateRecursive()
{
    return CreateRecursive(FullPath());
}

bool Directory::MoveTo(const std::string & destination)
{
    return Move(FullPath(), destination);
}

bool Directory::MoveTo(const Directory & destination)
{
    return MoveTo(destination.FullPath());
}

bool Directory::Delete()
{
    return Delete(FullPath());
}

bool Directory::DeleteRecursive()
{
    return DeleteRecursive(FullPath());
}

bool Directory::Create(const std::string & path)
{
    return OSAL::Path::MakeSureDirectoryExists(path);
}

bool Directory::CreateRecursive(const std::string & path)
{
    string parentDirectory = OSAL::Path::FirstPartOfPath(path);
    string subDirectory = OSAL::Path::LastPartOfPath(path);
    if (!Exists(parentDirectory) && !CreateRecursive(parentDirectory))
        return false;

    return OSAL::Path::MakeSureDirectoryExists(path);
}

bool Directory::Move(const std::string & source, const std::string & destination)
{
    return (OSAL::Path::MoveDirectory(source, destination) == 0);
}

bool Directory::Delete(const std::string & path)
{
    return (OSAL::Path::RemoveDirectory(path) == 0);
}

bool Directory::DeleteRecursive(const std::string & path)
{
    return (OSAL::Path::RemoveDirectory(path) == 0);
}

//FileInfo::List Directory::GetFiles(SearchOption searchOption) const
//{
//    return GetFiles("*", searchOption);
//}
//
//FileInfo::List Directory::GetFiles(const std::string & searchPattern,
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
//        Directory directoryInfo(OSAL::Path::FullPath(globbuf.gl_pathv[i]));
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
//Directory::List Directory::GetDirectories() const
//{
//    return GetDirectories("*", SearchOption::Normal);
//}
//
//Directory::List Directory::GetDirectories(const std::string & searchPattern,
//                                                  SearchOption searchOption) const
//{
//    Directory::List result;
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
//        Directory directoryInfo(OSAL::Path::FullPath(globbuf.gl_pathv[i]));
//        if (!HaveSubDirectory(result, directoryInfo.Name()))
//            AddSubDirectory(result, directoryInfo);
//
//        if (!HaveSubDirectory(directoryInfo.Name()))
//            AddSubDirectory(directoryInfo);
//        if ((searchOption & SearchOption::Recursive) != 0)
//        {
//            Directory::List subDirectoryResult = directoryInfo.GetDirectories(searchPattern, searchOption);
//            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
//        }
//    }
//    globfree(&globbuf);
//
//    return result;
//}
//
//void Directory::AddSubDirectory(const Directory & directory) const
//{
//    AddSubDirectory(_subDirectories, directory);
//}
//
//bool Directory::HaveSubDirectory(const std::string & name) const
//{
//    return HaveSubDirectory(_subDirectories, name);
//}
//
//void Directory::AddSubDirectory(Directory::List & list, const Directory & directory)
//{
//    list.push_back(directory);
//}
//
//bool Directory::HaveSubDirectory(Directory::List & list, const std::string & name)
//{
//    for (auto const & directory : list)
//    {
//        if (directory.Name() == name)
//            return true;
//    }
//    return false;
//}
//
//void Directory::GetFilesInCurrentDirectory(FileInfo::List & result,
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
