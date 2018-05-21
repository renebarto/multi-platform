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
    }
    void TearDown() override
    {
        OSAL::Path::MakeSureFileDoesNotExist(Core::Test::Data::RegularFileNonExistingPath());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::HiddenFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::SymlinkFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::SymlinkFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::HiddenFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::SymlinkFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::SymlinkFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), file.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.GetFilename());
    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), file.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), file.GetExtension());
    EXPECT_EQ(nullptr, file.GetDirectory());
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
    EXPECT_TRUE(file.Exists(path));
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
    EXPECT_FALSE(file.Exists(path));
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
    EXPECT_TRUE(file.Exists(path));
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
    EXPECT_TRUE(file.Exists(path));
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
    EXPECT_TRUE(file.Exists(path));
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
    EXPECT_TRUE(file.Exists(path));
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
    EXPECT_TRUE(file.Exists(path));
}

TEST_FIXTURE(FileTest, OpenWriteCreateOrOverwrite_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists(path));
}

TEST_FIXTURE(FileTest, OpenWriteTruncateExisting_Existing)
{
    File::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();
    Core::Time::DateTime dummyTime;
    FileInfo info(path);

    File file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_TRUE(file.GetStream().good());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists(path));
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
    EXPECT_FALSE(file.Exists(path));
}

// Todo: Add tests for Copy, Move, Delete

} // namespace Test
} // namespace File
} // namespace Core
