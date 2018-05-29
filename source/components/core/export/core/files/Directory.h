#pragma once

#include <memory>
#include <string>
#include <vector>
#include "core/exports.h"
#include "core/files/FileInfo.h"
#include "core/files/FileSystemInfo.h"

namespace Core {
namespace Files {

enum class SearchOption : int
{
    Normal = 0x00,
    Recursive = 0x01,
    IncludeHidden = 0x02,
    IncludeSymbolicLinks = 0x04,
};

class FileInfo;
using FileInfoList = std::vector<FileInfo>;

class CORE_EXPORT Directory : public DirectoryInfo
{
public:
    using Ptr =  std::shared_ptr<Directory>;
    using WeakPtr = std::weak_ptr<Directory>;
    using List = std::vector<Directory>;

    Directory() = delete;
    explicit Directory(const DirectoryInfo & info);
    Directory(const std::string & path);
    Directory(const Directory &);
    Directory(Directory &&);
    Directory & operator = (const Directory &);
    Directory & operator = (Directory &&);

    bool CreateSubdirectory(const std::string & path);
    bool Create();
    bool CreateRecursive();
    bool MoveTo(const std::string & destination);
    bool MoveTo(const Directory & destination);
    bool Delete();
    bool DeleteRecursive();

    static bool Create(const std::string & path);
    static bool CreateRecursive(const std::string & path);
    static bool Move(const std::string & source, const std::string & destination);
    static bool Delete(const std::string & path);
    static bool DeleteRecursive(const std::string & path);

    FileInfo::List ScanForFiles(SearchOption searchOption = SearchOption::Normal) const;
    FileInfo::List ScanForFiles(const std::string & searchPattern,
                                SearchOption searchOption = SearchOption::Normal) const;
    DirectoryInfo::List ScanForDirectories(SearchOption searchOption = SearchOption::Normal) const;
    DirectoryInfo::List ScanForDirectories(const std::string & searchPattern,
                                           SearchOption searchOption = SearchOption::Normal) const;
    static void AddDirectory(DirectoryInfo::List & directories, const DirectoryInfo & directory);
    static bool HaveDirectory(DirectoryInfo::List & directories, const std::string & name);
    static void AddFile(FileInfo::List & files, const FileInfo & file);
    static bool HaveFile(FileInfo::List & files, const std::string & name);

protected:
    static bool GetFilesInCurrentDirectory(FileInfo::List & result,
                                           const std::string & path,
                                           const std::string & searchPattern,
                                           SearchOption searchOption);
};

} // namespace Files
} // namespace Core

template<>
struct is_flag<Core::Files::SearchOption>
{
    static constexpr bool value=true;
};
