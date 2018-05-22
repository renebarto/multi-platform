#include <unittest-cpp/UnitTestC++.h>

#include "core/files/Directory.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class DirectoryTest : public UnitTestCpp::TestFixture
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

TEST_FIXTURE(DirectoryTest, ConstructFromDirectoryInfoExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;
    DirectoryInfo info(path);

    Directory directory(info);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromDirectoryInfoNonExisting)
{
    string path = Core::Test::Data::DummyDirPath();
    Core::Time::DateTime dummyTime;
    DirectoryInfo info(path);

    Directory directory(info);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), directory.Name());
    EXPECT_EQ(dummyTime, directory.CreationTime());
    EXPECT_EQ(dummyTime, directory.LastAccessTime());
    EXPECT_EQ(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_FALSE(directory.IsDirectory());
    EXPECT_EQ(0x00, directory.GetRights());
    EXPECT_FALSE(directory.IsReadable());
    EXPECT_FALSE(directory.IsWritable());
    EXPECT_FALSE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::DummyDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromDirectoryInfoHidden)
{
    string path = Core::Test::Data::HiddenDirPath();
    Core::Time::DateTime dummyTime;
    DirectoryInfo info(path);

    Directory directory(info);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_TRUE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::HiddenDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromDirectoryInfoSymlink)
{
    string path = Core::Test::Data::SymlinkDirPath();
    Core::Time::DateTime dummyTime;
    DirectoryInfo info(path);

    Directory directory(info);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_TRUE(directory.IsSymlink());
    EXPECT_FALSE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_EQ(Core::Test::Data::SymlinkDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::SymlinkDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::SymlinkDirNameNoExtension(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::SymlinkDirExtension(), directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromPathExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;

    Directory directory(path);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromPathNonExisting)
{
    string path = Core::Test::Data::DummyDirPath();
    Core::Time::DateTime dummyTime;

    Directory directory(path);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), directory.Name());
    EXPECT_EQ(dummyTime, directory.CreationTime());
    EXPECT_EQ(dummyTime, directory.LastAccessTime());
    EXPECT_EQ(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_FALSE(directory.IsDirectory());
    EXPECT_EQ(0x00, directory.GetRights());
    EXPECT_FALSE(directory.IsReadable());
    EXPECT_FALSE(directory.IsWritable());
    EXPECT_FALSE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::DummyDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromPathHidden)
{
    string path = Core::Test::Data::HiddenDirPath();
    Core::Time::DateTime dummyTime;

    Directory directory(path);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_TRUE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::HiddenDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructFromPathSymlink)
{
    string path = Core::Test::Data::SymlinkDirPath();
    Core::Time::DateTime dummyTime;

    Directory directory(path);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_TRUE(directory.IsSymlink());
    EXPECT_FALSE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_EQ(Core::Test::Data::SymlinkDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::SymlinkDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::SymlinkDirNameNoExtension(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ(Core::Test::Data::SymlinkDirExtension(), directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructCopyExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;
    Directory other(path);

    Directory directory(other);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, ConstructMoveExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;
    Directory other(path);

    Directory directory(std::move(other));

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, AssignCopyExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;
    Directory other(path);

    Directory directory(Core::Test::Data::DummyFilePath());
    directory = other;

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

TEST_FIXTURE(DirectoryTest, AssignMoveExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;
    Directory other(path);

    Directory directory(Core::Test::Data::DummyFilePath());
    directory = std::move(other);

    EXPECT_EQ(path, directory.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.Name());
    EXPECT_NE(dummyTime, directory.CreationTime());
    EXPECT_NE(dummyTime, directory.LastAccessTime());
    EXPECT_NE(dummyTime, directory.LastWriteTime());
    EXPECT_FALSE((directory.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((directory.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(directory.IsRegularFile());
    EXPECT_FALSE(directory.IsSymlink());
    EXPECT_TRUE(directory.IsDirectory());
    EXPECT_EQ(0x07, directory.GetRights());
    EXPECT_TRUE(directory.IsReadable());
    EXPECT_TRUE(directory.IsWritable());
    EXPECT_TRUE(directory.IsExecutable());
    EXPECT_FALSE(directory.IsReadOnly());
    EXPECT_FALSE(directory.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(Core::Test::Data::FilledDirPath(), directory.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), directory.GetParentDirectoryPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), directory.GetFilenameWithoutExtension());
    EXPECT_EQ("", directory.GetExtension());
    EXPECT_EQ(nullptr, directory.GetParentDirectory());
}

//TEST_FIXTURE(DirectoryTest, DeleteExisting)
//{
//    Directory::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory directory(path);
//
//    EXPECT_TRUE(directory.Exists());
//    EXPECT_TRUE(directory.Delete());
//    EXPECT_FALSE(directory.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, DeleteNonExisting)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory directory(path);
//
//    EXPECT_FALSE(directory.Exists());
//    EXPECT_FALSE(directory.Delete());
//    EXPECT_FALSE(directory.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, DeletePathExisting)
//{
//    Directory::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    EXPECT_TRUE(Directory::Exists(path));
//    EXPECT_TRUE(Directory::Delete(path));
//    EXPECT_FALSE(Directory::Exists(path));
//}
//
//TEST_FIXTURE(DirectoryTest, DeletePathNonExisting)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    EXPECT_FALSE(Directory::Exists(path));
//    EXPECT_FALSE(Directory::Delete(path));
//    EXPECT_FALSE(Directory::Exists(path));
//}
//
//TEST_FIXTURE(DirectoryTest, CopyToExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.CopyTo(pathDst));
//    EXPECT_TRUE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CopyToNonExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CopyTo(pathDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, MoveToExisting)
//{
//    Directory::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFileNonExisting2Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.MoveTo(pathDst));
//    EXPECT_TRUE(Directory::Compare(Core::Test::Data::RegularFilePath(), pathDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, MoveToNonExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.MoveTo(pathDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CopyToFileExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.CopyTo(fileDst));
//    EXPECT_TRUE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CopyToFileNonExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CopyTo(fileDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, MoveToFileExisting)
//{
//    Directory::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFileNonExisting2Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.MoveTo(fileDst));
//    EXPECT_TRUE(Directory::Compare(Core::Test::Data::RegularFilePath(), pathDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, MoveToFileNonExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.MoveTo(fileDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CopyPathToPathExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//    EXPECT_TRUE(Directory::Copy(pathSrc, pathDst));
//    EXPECT_TRUE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, CopyPathToPathNonExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//    EXPECT_FALSE(Directory::Copy(pathSrc, pathDst));
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, MovePathToPathExisting)
//{
//    Directory::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFileNonExisting2Path();
//
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//    EXPECT_TRUE(Directory::Move(pathSrc, pathDst));
//    EXPECT_TRUE(Directory::Compare(Core::Test::Data::RegularFilePath(), pathDst));
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, MovePathToPathNonExisting)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//    EXPECT_FALSE(Directory::Move(pathSrc, pathDst));
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToExistingSourceAndDestinationEqual)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFile2Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.CompareTo(pathDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToExistingSourceAndDestinationInequal)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFile3Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(pathDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToExistingSourceNonExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToNonExistingSourceExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFilePath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToFileExistingSourceAndDestinationEqual)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFile2Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToFileExistingSourceAndDestinationInequal)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFile3Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToFileExistingSourceNonExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, CompareToFileNonExistingSourceExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFilePath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(DirectoryTest, ComparePathToPathExistingSourceAndDestinationEqual)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFile2Path();
//
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//    EXPECT_TRUE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, ComparePathToPathExistingSourceAndDestinationInequal)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFile3Path();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//    EXPECT_FALSE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, ComparePathToPathExistingSourceNonExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//    EXPECT_FALSE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(Directory::Exists(pathSrc));
//    EXPECT_FALSE(Directory::Exists(pathDst));
//}
//
//TEST_FIXTURE(DirectoryTest, ComparePathToPathNonExistingSourceExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFilePath();
//
//    Directory fileSrc(pathSrc);
//    Directory fileDst(pathDst);
//
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//    EXPECT_FALSE(Directory::Compare(pathSrc, pathDst));
//    EXPECT_FALSE(Directory::Exists(pathSrc));
//    EXPECT_TRUE(Directory::Exists(pathDst));
//}

} // namespace Test
} // namespace Files
} // namespace Core
