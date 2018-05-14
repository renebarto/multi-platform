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
    Directory(const std::string & path);
    Directory(const Directory &);
    Directory(Directory &&);
    Directory & operator = (const Directory &);
    Directory & operator = (Directory &&);

    bool Exists() const;
    void CreateSubdirectory(const std::string & path);
    void Create();
    FileInfoList GetFiles(SearchOption searchOption = SearchOption::Normal) const;
    FileInfoList GetFiles(const std::string & searchPattern,
                          SearchOption searchOption = SearchOption::Normal) const;
    List GetDirectories() const;
    List GetDirectories(const std::string & searchPattern,
                        SearchOption searchOption = SearchOption::Normal) const;
    void MoveTo(const std::string & destination);
    void Move(const std::string & source, const std::string & destination);
    void Delete();

    static bool Exists(const std::string & path);
    static void CreateDirectory(const std::string & path);
    static void Delete(const std::string & path);

    std::string const & GetPath() const;
    std::string GetDirectoryName() const;
    std::string GetFilename() const;
    std::string GetFilenameWithoutExtension() const;
    std::string GetExtension() const;
    std::shared_ptr<DirectoryInfo> GetDirectory() const { return Parent(); }

protected:
    void AddSubDirectory(const DirectoryInfo & directory) const;
    bool HaveSubDirectory(const std::string & name) const;
    static void AddSubDirectory(List & list, const DirectoryInfo & directory);
    static bool HaveSubDirectory(List & list, const std::string & name);
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
