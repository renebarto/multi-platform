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

class CORE_EXPORT DirectoryInfo : public FileSystemInfo
{
public:
    using Ptr =  std::shared_ptr<DirectoryInfo>;
    using WeakPtr = std::weak_ptr<DirectoryInfo>;
    using List = std::vector<DirectoryInfo>;

    DirectoryInfo() = delete;
    explicit DirectoryInfo(const std::string & path);
    DirectoryInfo(const DirectoryInfo &);
    DirectoryInfo(DirectoryInfo &&);
    DirectoryInfo & operator = (const DirectoryInfo &);
    DirectoryInfo & operator = (DirectoryInfo &&);

    FileInfoList GetFiles() const { return _files; }
    List GetDirectories() const { return _subDirectories; }

    bool Exists() const;

    static bool Exists(const std::string & path);

protected:
    void AddSubDirectory(const DirectoryInfo & directory) const;
    bool HaveSubDirectory(const std::string & name) const;
    void AddFile(const FileInfo & file) const;
    bool HaveFile(const std::string & name) const;

    mutable List _subDirectories;
    mutable FileInfoList _files;

    friend class FileInfo;
};

} // namespace Files
} // namespace Core

template<>
struct is_flag<Core::Files::SearchOption>
{
    static constexpr bool value=true;
};
