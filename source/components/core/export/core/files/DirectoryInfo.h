#pragma once

#include <memory>
#include <string>
#include <vector>
#include "core/exports.h"
#include "core/files/FileInfo.h"
#include "core/files/FileSystemInfo.h"

namespace Core {
namespace Files {

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

    bool Exists() const;

    static bool Exists(const std::string & path);

protected:

    friend class FileInfo;
};

} // namespace Files
} // namespace Core
