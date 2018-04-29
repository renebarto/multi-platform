#pragma once

#include <string>
#include <vector>
#include "Core/IProgressHandler.h"
#include "Core/FileInfo.h"

namespace Core
{

class IFileFindCallback
{
public:
    virtual ~IFileFindCallback() {}

    virtual bool OnFileFound(const std::string & path) = 0;
};

class FileFind : public Observable<IFileFindCallback>, public ICancellable
{
public:
    enum class Options : int
    {
        Default = 0x00,
        Recursive = 0x01,
        SkipHiddenFiles = 0x02,
    };

    FileFind();
    FileFind(const FileFind &) = delete;
    explicit FileFind(Options options);
    virtual ~FileFind();

    FileFind & operator = (const FileFind &) = delete;

    const FileInfo::List & SearchResult() { return searchResult; }

    size_t Search(const std::string & path, const std::string & wildcard, IProgressHandler * dialog = nullptr);

protected:
    static bool CompareFiles(const FileInfo & x, const FileInfo & y);
    static bool IsHiddenFile(const FileInfo & fileInfo);
    std::vector<std::string> Split(const std::string & wildcards, char delimiter) const;
    FileInfo::List GetFiles(const DirectoryInfo & directoryInfo, const std::string & wildcards, SearchOption option) const;

    void OnCancel();

    SearchOption searchOption;
    bool skipHiddenFiles;

    static bool stopScan;

    FileInfo::List fileList;
    FileInfo::List searchResult;
};

DEFINE_FLAG_OPERATORS(FileFind::Options, int);

} // namespace Core
