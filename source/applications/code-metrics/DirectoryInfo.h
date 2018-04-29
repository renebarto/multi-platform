#pragma once

#include <memory>
#include <string>
#include <vector>
#include "FileInfo.h"
#include "FileSystemInfo.h"
//#include "Core/Timestamp.h"

namespace Core
{

enum class SearchOption : int
{
    Normal = 0x00,
    Recursive = 0x01,
    IncludeHidden = 0x02,
};

DEFINE_FLAG_OPERATORS(SearchOption, int);

class FileInfo;
typedef std::vector<FileInfo> FileInfoList;

class DirectoryInfo : public FileSystemInfo
{
public:
    typedef std::shared_ptr<DirectoryInfo> Ptr;
    typedef std::vector<DirectoryInfo> List;

    DirectoryInfo() = delete;
    bool Exists() const;
    static bool Exists(const std::string & path);
    void CreateSubdirectory(const std::string & path);
    static void CreateDirectory(const std::string & path);
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
    static void Delete(const std::string & path);

protected:
    explicit DirectoryInfo(const std::string & path);
    void AddSubDirectory(DirectoryInfo & directory) const;
    bool HaveSubDirectory(const std::string & name) const;
    void AddSubDirectory(List & list, DirectoryInfo & directory) const;
    bool HaveSubDirectory(List & list, const std::string & name) const;
    void GetFilesInCurrentDirectory(FileInfoList & result, const std::string & searchPattern, SearchOption searchOption) const;

    mutable List subDirectories;

    friend class FileInfo;
    friend class FileFind;
    friend class DirectoryFind;
};

} // namespace Core
