#include "core/files/Directory.h"

#include <algorithm>
#include <glob.h>
#include <unistd.h>
#include "osal/Exception.h"
#include "osal/Path.h"
#include "core/files/File.h"

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
    Directory directory(path);
    FileInfo::List files = directory.ScanForFiles();
    for (auto const & file : files)
    {
        if (!File::Delete(file.FullPath()))
            return false;
    }
    DirectoryInfo::List directories = directory.ScanForDirectories();
    for (auto const & directory : directories)
    {
        if (!DeleteRecursive(directory.FullPath()))
        return false;
    }
    return Delete(path);
}

FileInfo::List Directory::ScanForFiles(SearchOption searchOption) const
{
    return ScanForFiles("*", searchOption);
}

FileInfo::List Directory::ScanForFiles(const std::string & searchPattern,
                                       SearchOption searchOption) const
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

DirectoryInfo::List Directory::ScanForDirectories(const std::string & searchPattern,
                                                  SearchOption searchOption) const
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

DirectoryInfo::List Directory::ScanForDirectories(SearchOption searchOption) const
{
    return ScanForDirectories("*", searchOption);
}

bool Directory::GetFilesInCurrentDirectory(FileInfo::List & result,
                                           const string & path,
                                           const string & searchPattern,
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

void Directory::AddDirectory(DirectoryInfo::List & directories, const DirectoryInfo & directory)
{
    directories.push_back(directory);
}

bool Directory::HaveDirectory(DirectoryInfo::List & directories, const std::string & name)
{
    for (auto const & directory : directories)
    {
        if (directory.FullPath() == name)
            return true;
    }
    return false;
}

void Directory::AddFile(FileInfo::List & files, const FileInfo & file)
{
    files.push_back(file);
}

bool Directory::HaveFile(FileInfo::List & files, const std::string & name)
{
    for (auto const & file : files)
    {
        if (file.FullPath() == name)
            return true;
    }
    return false;
}

