#include "DirectoryInfo.h"

#include <algorithm>
#include <glob.h>
#include <unistd.h>
//#include "Core/Exception.h"
//#include "Core/Path.h"

using namespace std;
using namespace Core;

DirectoryInfo::DirectoryInfo(const string & path) :
    FileSystemInfo(path),
    subDirectories()
{
}

bool DirectoryInfo::Exists() const
{
    return Exists(FullPath());
}

bool DirectoryInfo::Exists(const std::string & path)
{
    return Path::DirectoryExists(path);
}

void DirectoryInfo::CreateSubdirectory(const std::string & name)
{
    return CreateDirectory(Path::CombinePath(FullPath(), name));
}

void DirectoryInfo::CreateDirectory(const std::string & path)
{
    Path::MakeSureDirectoryExists(path);
}

void DirectoryInfo::Create()
{
    CreateDirectory(FullPath());
}

FileInfo::List DirectoryInfo::GetFiles(SearchOption searchOption) const
{
    return GetFiles("*", searchOption);
}

FileInfo::List DirectoryInfo::GetFiles(const std::string & searchPattern,
                                       SearchOption searchOption) const
{
    FileInfo::List result;

    GetFilesInCurrentDirectory(result, searchPattern, searchOption);

    string wildcardSpecifier = Path::CombinePath(FullPath(), "*");
    glob_t globbuf;

    glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globbuf);

    for (size_t i = 0; i < globbuf.gl_pathc; i++)
    {
        string directoryName = Path::LastPartOfPath(globbuf.gl_pathv[i]);
        if ((directoryName == ".") || (directoryName == ".."))
            continue;
        DirectoryInfo directoryInfo(Path::FullPath(globbuf.gl_pathv[i]));

        if (!HaveSubDirectory(directoryInfo.Name()))
            AddSubDirectory(directoryInfo);
        if ((searchOption & SearchOption::Recursive) != 0)
        {
            FileInfo::List subDirectoryResult = directoryInfo.GetFiles(searchPattern, searchOption);
            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
        }
    }
    globfree(&globbuf);

    return result;
}

DirectoryInfo::List DirectoryInfo::GetDirectories() const
{
    return GetDirectories("*", SearchOption::Normal);
}

DirectoryInfo::List DirectoryInfo::GetDirectories(const std::string & searchPattern,
                                                  SearchOption searchOption) const
{
    DirectoryInfo::List result;

    string wildcardSpecifier = Path::CombinePath(FullPath(), searchPattern);
    glob_t globbuf;

    glob(wildcardSpecifier.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &globbuf);

    for (size_t i = 0; i < globbuf.gl_pathc; i++)
    {
        string directoryName = Path::LastPartOfPath(globbuf.gl_pathv[i]);
        if ((directoryName == ".") || (directoryName == ".."))
            continue;
        DirectoryInfo directoryInfo(Path::FullPath(globbuf.gl_pathv[i]));
        if (!HaveSubDirectory(result, directoryInfo.Name()))
            AddSubDirectory(result, directoryInfo);

        if (!HaveSubDirectory(directoryInfo.Name()))
            AddSubDirectory(directoryInfo);
        if ((searchOption & SearchOption::Recursive) != 0)
        {
            DirectoryInfo::List subDirectoryResult = directoryInfo.GetDirectories(searchPattern, searchOption);
            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
        }
    }
    globfree(&globbuf);

    return result;
}

void DirectoryInfo::MoveTo(const std::string & destination)
{
    Move(FullPath(), destination);
}

void DirectoryInfo::Move(const std::string & source, const std::string & destination)
{
    if (rename(source.c_str(), destination.c_str()) != 0)
        Object::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void DirectoryInfo::Delete()
{
    Delete(FullPath());
}

void DirectoryInfo::Delete(const std::string & path)
{
    unlink(path.c_str());
}

void DirectoryInfo::AddSubDirectory(DirectoryInfo & directory) const
{
    AddSubDirectory(subDirectories, directory);
}

bool DirectoryInfo::HaveSubDirectory(const std::string & name) const
{
    return HaveSubDirectory(subDirectories, name);
}

void DirectoryInfo::AddSubDirectory(DirectoryInfo::List & list, DirectoryInfo & directory) const
{
    list.push_back(directory);
}

bool DirectoryInfo::HaveSubDirectory(DirectoryInfo::List & list, const std::string & name) const
{
    for (auto directory : list)
    {
        if (directory.Name() == name)
            return true;
    }
    return false;
}

void DirectoryInfo::GetFilesInCurrentDirectory(FileInfo::List & result, const string & searchPattern, SearchOption searchOption) const
{
    string wildcardSpecifier = Path::CombinePath(FullPath(), searchPattern);
    glob_t globbuf;

    glob(wildcardSpecifier.c_str(), GLOB_PERIOD, nullptr, &globbuf);

    for (size_t i = 0; i < globbuf.gl_pathc; i++)
    {
        FileInfo fileInfo(Path::FullPath(globbuf.gl_pathv[i]));
        if (!fileInfo.IsDirectory() &&
            (((searchOption & SearchOption::IncludeHidden) != 0) ||
             !fileInfo.IsHiddenFile()))
            result.push_back(fileInfo);
    }
    globfree(&globbuf);
}
