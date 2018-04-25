#include <cstring>
#include <iostream>
#include <vector>
#include <glob.h>
#include <osal/Path.h>
#include <osal/FlagOperators.h>

using namespace std;
using namespace OSAL::Path;

enum class FileAttributes : uint16_t
{
    None = 0x0000,
    Directory = 0x4000,
    CharacterDevice = 0x2000,
    BlockDevice = 0x6000,
    Normal = 0x8000,
    Fifo = 0x2000,
    Symlink = 0xA000,
    Socket = 0xC000,
    MaskOwnerRights = 0x01C0,
    MaskGroupRights = 0x0038,
    MaskOtherRights = 0x0007,
    ExecOtherRights = 0x0001,
    WriteOtherRights = 0x0002,
    ReadOtherRights = 0x0004,
};

template<>
struct is_flag<FileAttributes>
{
    static constexpr bool value=true;
};

enum class SearchOption : int
{
    Normal = 0x00,
    Recursive = 0x01,
    IncludeHidden = 0x02,
};

template<>
struct is_flag<SearchOption>
{
    static constexpr bool value=true;
};

struct DirectoryInfo
{
    using List = vector<DirectoryInfo>;

    DirectoryInfo(const string & path)
    {
        struct stat fileInfo;
        if (stat(path.c_str(), &fileInfo) != 0)
            memset(&fileInfo, 0, sizeof(fileInfo));
        _fullPath = OSAL::Path::FullPath(path);
        _name = LastPartOfPath(_fullPath);
        _attributes = FileAttributes(fileInfo.st_mode);
        _userID = fileInfo.st_uid;
        _groupID = fileInfo.st_gid;
        _length = fileInfo.st_size;
    }
    const std::string & FullPath() const { return _fullPath; }
    const std::string & Name() const { return _name; }

    FileAttributes Attributes() const { return _attributes; }

    bool IsRegularFile() const
    {
        return (Attributes() & FileAttributes::Normal) != 0;
    }

    bool IsDirectory() const
    {
        return (Attributes() & FileAttributes::Directory) != 0;
    }

    uint16_t GetRights() const
    {
        __gid_t gid = getgid();
        __uid_t uid = getuid();

        if (int16_t(_attributes) == 0)
            return 0;
        if (_userID == uid)
        {
            return uint16_t(_attributes & FileAttributes::MaskOwnerRights) >> 6;
        }
        if (_groupID == gid)
        {
            return uint16_t(_attributes & FileAttributes::MaskGroupRights) >> 3;
        }
        return uint16_t(_attributes & FileAttributes::MaskOtherRights);
    }

    bool IsReadable() const
    {
        return (GetRights() & uint16_t(FileAttributes::ReadOtherRights)) != 0;
    }

    bool IsWritable() const
    {
        return (GetRights() & uint16_t(FileAttributes::WriteOtherRights)) != 0;
    }

    bool IsExecutable() const
    {
        return (GetRights() & uint16_t(FileAttributes::ExecOtherRights)) != 0;
    }

    bool IsReadOnly() const
    {
        return (GetRights() & uint16_t(FileAttributes::ReadOtherRights | FileAttributes::WriteOtherRights)) == FileAttributes::ReadOtherRights;
    }

    void AddSubDirectory(DirectoryInfo & directory)
    {
        AddSubDirectory(_subDirectories, directory);
    }

    bool HaveSubDirectory(const std::string & name) const
    {
        return HaveSubDirectory(_subDirectories, name);
    }

    void AddSubDirectory(DirectoryInfo::List & list, DirectoryInfo & directory)
    {
        list.push_back(directory);
    }

    bool HaveSubDirectory(const DirectoryInfo::List & list, const std::string & name) const
    {
        for (auto directory : list)
        {
            if (directory.Name() == name)
                return true;
        }
        return false;
    }

    std::string _fullPath;
    std::string _name;
    FileAttributes _attributes;
    List _subDirectories;
    uid_t _userID;
    gid_t _groupID;
    off_t _length;
};

class GlobInfo
{
public:
    GlobInfo()
        : _globData()
        , _contents()
    {}
    virtual ~GlobInfo()
    {
        globfree(&_globData);
    }

    void SearchDirs(const string & wildcardSpecification)
    {
        glob(wildcardSpecification.c_str(), GLOB_PERIOD | GLOB_ONLYDIR, nullptr, &_globData);
        for (size_t i = 0; i < _globData.gl_pathc; ++i)
        {
            string path = _globData.gl_pathv[i];
            _contents.push_back(path);
        }
    }

    const vector<string> & Contents() const { return _contents; }

private:
    glob_t _globData;
    vector<string> _contents;
};

vector<string> ScanForFiles(const string & directory)
{

}

DirectoryInfo::List ScanForDirectories(const string & directory)
{
    DirectoryInfo::List result;

    const string searchPattern = "*";
    const SearchOption searchOption = SearchOption::Recursive;
    string wildcardSpecifier = CombinePath(directory, searchPattern);
    GlobInfo globInfo;
    globInfo.SearchDirs(wildcardSpecifier);

    for (auto path : globInfo.Contents())
    {
        string directoryName = LastPartOfPath(path);
        if ((directoryName == ".") || (directoryName == ".."))
            continue;
        DirectoryInfo directoryInfo(path);
        result.push_back(directoryInfo);
//        if (!HaveSubDirectory(result, directoryInfo.Name()))
//            AddSubDirectory(result, directoryInfo);
//
//        if (!HaveSubDirectory(directoryInfo.Name()))
//            AddSubDirectory(directoryInfo);
        if ((searchOption & SearchOption::Recursive) != 0)
        {
//            DirectoryInfo::List subDirectoryResult = directoryInfo.GetDirectories(searchPattern, searchOption);
//            result.insert(result.end(), subDirectoryResult.begin(), subDirectoryResult.end());
        }
    }
    return result;
}

void SearchPath(const string & root, const vector<string> & extensions)
{
    cout << "Scanning " << root << " for files ";
    for (auto const & extension : extensions)
    {
        cout << extension << " ";
    }
    auto directories = ScanForDirectories(root);
    cout << "Subdirs" << endl;
    for (auto const & directory : directories)
    {
        cout << directory.FullPath() << endl;
    }
    cout << endl;
}

void SearchDirectories(const string & root, const vector<string> & subDirectories, const vector<string> & extensions)
{
    for (auto const & subDirectory : subDirectories)
    {
        SearchPath(CombinePath(root, subDirectory), extensions);
    }
}

int main()
{
    string rootPath = OSAL::Path::FullPath(OSAL::Path::CombinePath(TEST_DATA_ROOT, ".."));
    SearchDirectories(rootPath, vector<string> {"source/applications", "source/components", "source/tools"}, vector<string> {"h", "cpp"});

    return EXIT_SUCCESS;
}