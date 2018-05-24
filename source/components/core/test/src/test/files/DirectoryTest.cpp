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
        Directory::DeleteRecursive(Core::Test::Data::DummyDirPath());
        Directory::DeleteRecursive(Core::Test::Data::NonExistingDirPath());
    }
    void TearDown() override
    {
        Directory::DeleteRecursive(Core::Test::Data::DummyDirPath());
        Directory::DeleteRecursive(Core::Test::Data::NonExistingDirPath());
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

TEST_FIXTURE(DirectoryTest, DeleteExisting)
{
    string path = Core::Test::Data::DummyDirPath();
    Directory::Create(path);

    Directory directory(path);

    EXPECT_TRUE(directory.Exists());
    EXPECT_TRUE(directory.Delete());
    EXPECT_FALSE(directory.Exists());
}

TEST_FIXTURE(DirectoryTest, DeleteNonExisting)
{
    string path = Core::Test::Data::DummyDirPath();

    Directory directory(path);

    EXPECT_FALSE(directory.Exists());
    EXPECT_FALSE(directory.Delete());
    EXPECT_FALSE(directory.Exists());
}

TEST_FIXTURE(DirectoryTest, DeleteFromPathExisting)
{
    string path = Core::Test::Data::DummyDirPath();
    Directory::Create(path);

    EXPECT_TRUE(Directory::Exists(path));
    EXPECT_TRUE(Directory::Delete(path));
    EXPECT_FALSE(Directory::Exists(path));
}

TEST_FIXTURE(DirectoryTest, DeleteFromPathNonExisting)
{
    string path = Core::Test::Data::DummyDirPath();

    EXPECT_FALSE(Directory::Exists(path));
    EXPECT_FALSE(Directory::Delete(path));
    EXPECT_FALSE(Directory::Exists(path));
}

TEST_FIXTURE(DirectoryTest, MoveToExisting)
{
    string pathSrc = Core::Test::Data::DummyDirPath();
    string pathDst = Core::Test::Data::NonExistingDirPath();
    Directory::Create(pathSrc);

    Directory directorySrc(pathSrc);
    Directory directoryDst(pathDst);

    EXPECT_TRUE(directorySrc.Exists());
    EXPECT_FALSE(directoryDst.Exists());
    EXPECT_TRUE(directorySrc.MoveTo(pathDst));
    EXPECT_FALSE(directorySrc.Exists());
    EXPECT_TRUE(directoryDst.Exists());
}

TEST_FIXTURE(DirectoryTest, MoveToNonExisting)
{
    string pathSrc = Core::Test::Data::DummyDirPath();
    string pathDst = Core::Test::Data::NonExistingDirPath();

    Directory directorySrc(pathSrc);
    Directory directoryDst(pathDst);

    EXPECT_FALSE(directorySrc.Exists());
    EXPECT_FALSE(directoryDst.Exists());
    EXPECT_FALSE(directorySrc.MoveTo(pathDst));
    EXPECT_FALSE(directorySrc.Exists());
    EXPECT_FALSE(directoryDst.Exists());
}

TEST_FIXTURE(DirectoryTest, MoveToDirectoryExisting)
{
    string pathSrc = Core::Test::Data::DummyDirPath();
    string pathDst = Core::Test::Data::NonExistingDirPath();
    Directory::Create(pathSrc);

    Directory directorySrc(pathSrc);
    Directory directoryDst(pathDst);

    EXPECT_TRUE(directorySrc.Exists());
    EXPECT_FALSE(directoryDst.Exists());
    EXPECT_TRUE(directorySrc.MoveTo(directoryDst));
    EXPECT_FALSE(directorySrc.Exists());
    EXPECT_TRUE(directoryDst.Exists());
}

TEST_FIXTURE(DirectoryTest, MoveToDirectoryNonExisting)
{
    string pathSrc = Core::Test::Data::RegularFileNonExisting2Path();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    Directory directorySrc(pathSrc);
    Directory directoryDst(pathDst);

    EXPECT_FALSE(directorySrc.Exists());
    EXPECT_FALSE(directoryDst.Exists());
    EXPECT_FALSE(directorySrc.MoveTo(directoryDst));
    EXPECT_FALSE(directorySrc.Exists());
    EXPECT_FALSE(directoryDst.Exists());
}

TEST_FIXTURE(DirectoryTest, MovePathToPathExisting)
{
    string pathSrc = Core::Test::Data::DummyDirPath();
    string pathDst = Core::Test::Data::NonExistingDirPath();
    Directory::Create(pathSrc);

    EXPECT_TRUE(Directory::Exists(pathSrc));
    EXPECT_FALSE(Directory::Exists(pathDst));
    EXPECT_TRUE(Directory::Move(pathSrc, pathDst));
    EXPECT_FALSE(Directory::Exists(pathSrc));
    EXPECT_TRUE(Directory::Exists(pathDst));
}

TEST_FIXTURE(DirectoryTest, MovePathToPathNonExisting)
{
    string pathSrc = Core::Test::Data::DummyDirPath();
    string pathDst = Core::Test::Data::NonExistingDirPath();

    Directory directorySrc(pathSrc);
    Directory directoryDst(pathDst);

    EXPECT_FALSE(Directory::Exists(pathSrc));
    EXPECT_FALSE(Directory::Exists(pathDst));
    EXPECT_FALSE(Directory::Move(pathSrc, pathDst));
    EXPECT_FALSE(Directory::Exists(pathSrc));
    EXPECT_FALSE(Directory::Exists(pathDst));
}

TEST_FIXTURE(DirectoryTest, CreateSubdirectoryExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);
    Directory::Create(path);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_TRUE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_TRUE(directory.CreateSubdirectory(subDirectory));
    EXPECT_TRUE(directory.Exists());
    EXPECT_TRUE(directoryComplete.Exists());
    EXPECT_TRUE(directoryComplete.Delete());
}

TEST_FIXTURE(DirectoryTest, CreateSubdirectoryNonExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_FALSE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_FALSE(directory.CreateSubdirectory(subDirectory));
    EXPECT_FALSE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_FALSE(directoryComplete.Delete());
}

TEST_FIXTURE(DirectoryTest, CreateExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);
    Directory::Create(path);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_TRUE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_TRUE(directoryComplete.Create());
    EXPECT_TRUE(directory.Exists());
    EXPECT_TRUE(directoryComplete.Exists());
    EXPECT_TRUE(directoryComplete.Delete());
}

TEST_FIXTURE(DirectoryTest, CreateNonExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_FALSE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_FALSE(directoryComplete.Create());
    EXPECT_FALSE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_FALSE(directoryComplete.Delete());
}

TEST_FIXTURE(DirectoryTest, CreateRecursiveExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);
    Directory::Create(path);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_TRUE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_TRUE(directoryComplete.CreateRecursive());
    EXPECT_TRUE(directory.Exists());
    EXPECT_TRUE(directoryComplete.Exists());
    EXPECT_FALSE(directory.Delete());
    EXPECT_TRUE(directoryComplete.Delete());
    EXPECT_TRUE(directory.Delete());
}

TEST_FIXTURE(DirectoryTest, CreateRecursiveNonExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_FALSE(directory.Exists());
    EXPECT_FALSE(directoryComplete.Exists());
    EXPECT_TRUE(directoryComplete.CreateRecursive());
    EXPECT_TRUE(directory.Exists());
    EXPECT_TRUE(directoryComplete.Exists());
    EXPECT_FALSE(directory.Delete());
    EXPECT_TRUE(directoryComplete.Delete());
    EXPECT_TRUE(directory.Delete());
}

TEST_FIXTURE(DirectoryTest, CreateFromPathExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);
    Directory::Create(path);

    EXPECT_TRUE(Directory::Exists(path));
    EXPECT_FALSE(Directory::Exists(pathComplete));
    EXPECT_TRUE(Directory::Create(pathComplete));
    EXPECT_TRUE(Directory::Exists(path));
    EXPECT_TRUE(Directory::Exists(pathComplete));
    EXPECT_TRUE(Directory::Delete(pathComplete));
}

TEST_FIXTURE(DirectoryTest, CreateFromPathNonExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);

    EXPECT_FALSE(Directory::Exists(path));
    EXPECT_FALSE(Directory::Exists(pathComplete));
    EXPECT_FALSE(Directory::Create(pathComplete));
    EXPECT_FALSE(Directory::Exists(path));
    EXPECT_FALSE(Directory::Exists(pathComplete));
    EXPECT_FALSE(Directory::Delete(pathComplete));
}

TEST_FIXTURE(DirectoryTest, CreateRecursiveFromPathExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);
    Directory::Create(path);

    EXPECT_TRUE(Directory::Exists(path));
    EXPECT_FALSE(Directory::Exists(pathComplete));
    EXPECT_TRUE(Directory::CreateRecursive(pathComplete));
    EXPECT_TRUE(Directory::Exists(path));
    EXPECT_TRUE(Directory::Exists(pathComplete));
    EXPECT_FALSE(Directory::Delete(path));
    EXPECT_TRUE(Directory::Delete(pathComplete));
    EXPECT_TRUE(Directory::Delete(path));
}

TEST_FIXTURE(DirectoryTest, CreateRecursiveFromPathNonExistingParent)
{
    string path = Core::Test::Data::DummyDirPath();
    string subDirectory = Core::Test::Data::DummyDirName();
    string pathComplete = OSAL::Path::CombinePath(path, subDirectory);

    Directory directory(path);
    Directory directoryComplete(pathComplete);

    EXPECT_FALSE(Directory::Exists(path));
    EXPECT_FALSE(Directory::Exists(pathComplete));
    EXPECT_TRUE(Directory::CreateRecursive(pathComplete));
    EXPECT_TRUE(Directory::Exists(path));
    EXPECT_TRUE(Directory::Exists(pathComplete));
    EXPECT_FALSE(Directory::Delete(path));
    EXPECT_TRUE(Directory::Delete(pathComplete));
    EXPECT_TRUE(Directory::Delete(path));
}

// TODO: DeleteRecursive tests
// TODO: ScanForFiles tests
// TODO: ScanForDirectories tests

} // namespace Test
} // namespace Files
} // namespace Core
