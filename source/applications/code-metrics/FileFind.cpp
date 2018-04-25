#include "FileFind.h"

#include <algorithm>
#include "DirectoryFind.h"
//#include "Core/Exception.h"
//#include "Core/Util.h"

using namespace std;
using namespace Core;

bool FileFind::stopScan = false;

FileFind::FileFind() :
    searchOption(SearchOption::Normal),
    skipHiddenFiles(false),
    fileList(),
    searchResult()
{
}

FileFind::FileFind(Options options) :
    searchOption(((options & Options::Recursive) != 0) ? SearchOption::Recursive : SearchOption::Normal),
    skipHiddenFiles((options & Options::SkipHiddenFiles) != 0)
{
}

FileFind::~FileFind()
{
}

size_t FileFind::Search(const std::string & path, const std::string & wildcards, IProgressHandler * dialog /*= nullptr*/)
{
    searchResult.clear();
    size_t count = 0;
    if (!DirectoryInfo::Exists(path))
    {
        return 0;
    }
    DirectoryInfo dirInfo = DirectoryInfo(path);
    stopScan = false;
    if (nullptr != dialog)
    {
        dialog->ProgressDialogShow();
        dialog->ProgressDialogInfo("Initializing");
        dialog->AddObserver(this);
    }
    fileList = GetFiles(dirInfo, wildcards, searchOption);
    std::sort(fileList.begin(), fileList.end(), CompareFiles);
    int i = 0;
    int numFiles = fileList.size();
    if (nullptr != dialog)
    {
        dialog->ProgressDialogRange(numFiles);
    }
    while (i < numFiles && !stopScan)
    {
        if (nullptr != dialog)
        {
            dialog->ProgressDialogValue(i);
            dialog->ProgressDialogInfo(fileList[i].DirectoryName());
        }
        bool isHidden = IsHiddenFile(fileList[i]);
        if (!(skipHiddenFiles && isHidden))
        {
            for (auto listener : *this)
            {
                stopScan = !listener->OnFileFound(fileList[i].FullPath());
            }
            searchResult.push_back(fileList[i]);
            count++;
        }
        i++;
    }

    if (nullptr != dialog)
    {
        dialog->ProgressDialogValue(numFiles);
        dialog->ProgressDialogInfo("Ready");
        dialog->ProgressDialogHide();
    }
    return count;
}

bool FileFind::CompareFiles(const FileInfo & x, const FileInfo & y)
{
    return x.FullPath() < y.FullPath();
}

void FileFind::OnCancel()
{
    stopScan = true;
}

bool FileFind::IsHiddenFile(const FileInfo & fileInfo)
{
    if (fileInfo.Name().empty())
        return false;
    return fileInfo.Name()[0] == '.';
}

vector<string> FileFind::Split(const string & wildcards, char delimiter) const
{
    size_t offset = 0;
    vector<string> result;
    size_t offsetNext = wildcards.find(delimiter, offset);
    while (offsetNext != string::npos)
    {
        result.push_back(wildcards.substr(offset, offsetNext - offset));
        offset = offsetNext;
    }
    result.push_back(wildcards.substr(offset));
    return result;
}

FileInfo::List FileFind::GetFiles(const DirectoryInfo & directoryInfo, const string & wildcards, SearchOption option) const
{
    FileInfoList result;
    vector<string> wildcardsList = Split(wildcards, ';');
    for (auto wildcard : wildcardsList)
    {
        FileInfoList fileInfo = directoryInfo.GetFiles(wildcard, option);
        result.insert(result.end(), fileInfo.begin(), fileInfo.end());
    }
    return result;
}
