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

    FileInfoList ScanForFiles(SearchOption searchOption = SearchOption::Normal) const;
    FileInfoList ScanForFiles(const std::string & searchPattern,
                              SearchOption searchOption = SearchOption::Normal) const;
    List ScanForDirectories() const;
    List ScanForDirectories(const std::string & searchPattern,
                            SearchOption searchOption = SearchOption::Normal) const;
    void CreateSubdirectory(const std::string & path);
    bool Exists() const;
    void Create();
    void MoveTo(const std::string & destination);
    void MoveTo(const Directory & destination);
    void Delete();

    static bool Exists(const std::string & path);
    static void CreateDirectory(const std::string & path);
    static void Move(const std::string & source, const std::string & destination);
    static void Delete(const std::string & path);

protected:
    static void GetFilesInCurrentDirectory(FileInfoList & result,
                                           const std::string & path,
                                           const std::string & searchPattern,
                                           SearchOption searchOption);

    mutable List _subDirectories;

    friend class FileInfo;
};

} // namespace Files
} // namespace Core

template<>
struct is_flag<Core::Files::SearchOption>
{
    static constexpr bool value=true;
};
