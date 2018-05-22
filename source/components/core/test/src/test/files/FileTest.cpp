#include <unittest-cpp/UnitTestC++.h>

#include "core/files/File.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class FileTest : public UnitTestCpp::TestFixture
{
public:
    void SetUp() override
    {
        OSAL::Path::MakeSureFileDoesNotExist(Core::Test::Data::RegularFileNonExistingPath());
        OSAL::Path::MakeSureFileDoesNotExist(Core::Test::Data::RegularFileNonExisting2Path());
    }
    void TearDown() override
    {
        OSAL::Path::MakeSureFileDoesNotExist(Core::Test::Data::RegularFileNonExistingPath());
        OSAL::Path::MakeSureFileDoesNotExist(Core::Test::Data::RegularFileNonExisting2Path());
    }
};

TEST_FIXTURE(FileTest, ConstructFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromFileInfoNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
    EXPECT_EQ(dummyTime, file.CreationTime());
    EXPECT_EQ(dummyTime, file.LastAccessTime());
    EXPECT_EQ(dummyTime, file.LastWriteTime());
    EXPECT_FALSE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x00, file.GetRights());
    EXPECT_FALSE(file.IsReadable());
    EXPECT_FALSE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::DummyFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromFileInfoHidden)
{
    string path = Core::Test::Data::HiddenFilePath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_TRUE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::HiddenFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::HiddenFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromFileInfoSymlink)
{
    string path = Core::Test::Data::SymlinkFilePath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(file.IsRegularFile());
    EXPECT_TRUE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x07, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_TRUE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(Core::Test::Data::RegularFileName().length(), file.Size());
    EXPECT_EQ(Core::Test::Data::SymlinkFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::SymlinkFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::SymlinkFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromFileInfoWithOpenParametersFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromFileInfoWithOpenParametersNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
    EXPECT_EQ(dummyTime, file.CreationTime());
    EXPECT_EQ(dummyTime, file.LastAccessTime());
    EXPECT_EQ(dummyTime, file.LastWriteTime());
    EXPECT_FALSE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x00, file.GetRights());
    EXPECT_FALSE(file.IsReadable());
    EXPECT_FALSE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::DummyFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromPathExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromPathNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
    EXPECT_EQ(dummyTime, file.CreationTime());
    EXPECT_EQ(dummyTime, file.LastAccessTime());
    EXPECT_EQ(dummyTime, file.LastWriteTime());
    EXPECT_FALSE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x00, file.GetRights());
    EXPECT_FALSE(file.IsReadable());
    EXPECT_FALSE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::DummyFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromPathHidden)
{
    string path = Core::Test::Data::HiddenFilePath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_TRUE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::HiddenFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::HiddenFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromPathSymlink)
{
    string path = Core::Test::Data::SymlinkFilePath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(file.IsRegularFile());
    EXPECT_TRUE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x07, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_TRUE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(Core::Test::Data::RegularFileName().length(), file.Size());
    EXPECT_EQ(Core::Test::Data::SymlinkFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::SymlinkFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::SymlinkFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromPathWithOpenParametersFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;

    File file(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructFromPathWithOpenParametersNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    Core::Time::DateTime dummyTime;

    File file(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
    EXPECT_EQ(dummyTime, file.CreationTime());
    EXPECT_EQ(dummyTime, file.LastAccessTime());
    EXPECT_EQ(dummyTime, file.LastWriteTime());
    EXPECT_FALSE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x00, file.GetRights());
    EXPECT_FALSE(file.IsReadable());
    EXPECT_FALSE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::DummyFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructCopyExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    File file(other);

    EXPECT_TRUE(other.IsOpen());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, ConstructMoveExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    File file(std::move(other));

    EXPECT_FALSE(other.IsOpen());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, AssignCopyExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    File file(Core::Test::Data::DummyFilePath());
    file = other;

    EXPECT_TRUE(other.IsOpen());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, AssignMoveExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    File file(Core::Test::Data::DummyFilePath());
    file = std::move(other);

    EXPECT_FALSE(other.IsOpen());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
    EXPECT_NE(dummyTime, file.CreationTime());
    EXPECT_NE(dummyTime, file.LastAccessTime());
    EXPECT_NE(dummyTime, file.LastWriteTime());
    EXPECT_TRUE((file.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((file.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(file.IsRegularFile());
    EXPECT_FALSE(file.IsSymlink());
    EXPECT_FALSE(file.IsDirectory());
    EXPECT_EQ(0x06, file.GetRights());
    EXPECT_TRUE(file.IsReadable());
    EXPECT_TRUE(file.IsWritable());
    EXPECT_FALSE(file.IsExecutable());
    EXPECT_FALSE(file.IsReadOnly());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_FALSE(file.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::RegularFilePath(), file.GetPath());
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetParentDirectory());
}

TEST_FIXTURE(FileTest, OpenReadOpenExisting_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
}

TEST_FIXTURE(FileTest, OpenReadOpenExisting_NonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    Core::Time::DateTime dummyTime;
    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
}

TEST_FIXTURE(FileTest, OpenReadCreateNew_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
}

TEST_FIXTURE(FileTest, OpenReadCreateOrOverwrite_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
}

TEST_FIXTURE(FileTest, OpenReadOpenOrCreate_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{6}, file.Size());
}

TEST_FIXTURE(FileTest, OpenReadTruncateExisting_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{6}, file.Size());
}

TEST_FIXTURE(FileTest, OpenWriteExisting_Existing)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteExisting_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteCreateNew_Existing)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteCreateNew_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteOpenOrCreate_Existing)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{6}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteOpenOrCreate_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteCreateOrOverwrite_Existing)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteCreateOrOverwrite_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteTruncateExisting_Existing)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(FileTest, OpenWriteTruncateExisting_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;

    File file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(FileTest, DeleteExisting)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    File file(path);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Delete());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(FileTest, DeleteNonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    File file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Delete());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(FileTest, DeletePathExisting)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    EXPECT_TRUE(File::Exists(path));
    EXPECT_TRUE(File::Delete(path));
    EXPECT_FALSE(File::Exists(path));
}

TEST_FIXTURE(FileTest, DeletePathNonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    EXPECT_FALSE(File::Exists(path));
    EXPECT_FALSE(File::Delete(path));
    EXPECT_FALSE(File::Exists(path));
}

TEST_FIXTURE(FileTest, CopyToExisting)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CopyTo(pathDst));
    EXPECT_TRUE(File::Compare(pathSrc, pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CopyToNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CopyTo(pathDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, MoveToExisting)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFileNonExisting2Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.MoveTo(pathDst));
    EXPECT_TRUE(File::Compare(Core::Test::Data::RegularFilePath(), pathDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, MoveToNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.MoveTo(pathDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CopyToFileExisting)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CopyTo(fileDst));
    EXPECT_TRUE(File::Compare(pathSrc, pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CopyToFileNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CopyTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, MoveToFileExisting)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFileNonExisting2Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.MoveTo(fileDst));
    EXPECT_TRUE(File::Compare(Core::Test::Data::RegularFilePath(), pathDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, MoveToFileNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.MoveTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CopyPathToPathExisting)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
    EXPECT_TRUE(File::Copy(pathSrc, pathDst));
    EXPECT_TRUE(File::Compare(pathSrc, pathDst));
    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, CopyPathToPathNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
    EXPECT_FALSE(File::Copy(pathSrc, pathDst));
    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, MovePathToPathExisting)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFileNonExisting2Path();

    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
    EXPECT_TRUE(File::Move(pathSrc, pathDst));
    EXPECT_TRUE(File::Compare(Core::Test::Data::RegularFilePath(), pathDst));
    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, MovePathToPathNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
    EXPECT_FALSE(File::Move(pathSrc, pathDst));
    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, CompareToExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile2Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CompareTo(pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile3Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToFileExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile2Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToFileExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile3Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToFileExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, CompareToFileNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(FileTest, ComparePathToPathExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile2Path();

    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
    EXPECT_TRUE(File::Compare(pathSrc, pathDst));
    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, ComparePathToPathExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile3Path();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
    EXPECT_FALSE(File::Compare(pathSrc, pathDst));
    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, ComparePathToPathExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
    EXPECT_FALSE(File::Compare(pathSrc, pathDst));
    EXPECT_TRUE(File::Exists(pathSrc));
    EXPECT_FALSE(File::Exists(pathDst));
}

TEST_FIXTURE(FileTest, ComparePathToPathNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    File fileSrc(pathSrc);
    File fileDst(pathDst);

    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
    EXPECT_FALSE(File::Compare(pathSrc, pathDst));
    EXPECT_FALSE(File::Exists(pathSrc));
    EXPECT_TRUE(File::Exists(pathDst));
}

} // namespace Test
} // namespace Files
} // namespace Core
