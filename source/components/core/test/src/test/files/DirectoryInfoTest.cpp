#include <unittest-cpp/UnitTestC++.h>

#include "core/files/DirectoryInfo.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class DirectoryInfoTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void DirectoryInfoTest::SetUp()
{
}

void DirectoryInfoTest::TearDown()
{
}

//TEST_FIXTURE(DirectoryInfoTest, ConstructFileExisting)
//{
//    string path = Core::Test::Data::RegularFilePath();
//    Core::Time::DateTime dummyTime;
//
//    DirectoryInfo info(path);
//
//    EXPECT_EQ(path, info.FullPath());
//    EXPECT_EQ(Core::Test::Data::RegularFileName(), info.Name());
//    EXPECT_NE(dummyTime, info.CreationTime());
//    EXPECT_NE(dummyTime, info.LastAccessTime());
//    EXPECT_NE(dummyTime, info.LastWriteTime());
//    EXPECT_TRUE((info.Attributes() & FileAttributes::Normal) != 0);
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
//    EXPECT_TRUE(info.IsRegularFile());
//    EXPECT_FALSE(info.IsSymlink());
//    EXPECT_FALSE(info.IsDirectory());
//    EXPECT_EQ(0x06, info.GetRights());
//    EXPECT_TRUE(info.IsReadable());
//    EXPECT_TRUE(info.IsWritable());
//    EXPECT_FALSE(info.IsExecutable());
//    EXPECT_FALSE(info.IsReadOnly());
//    EXPECT_EQ(Core::Test::Data::RegularFilePath(), info.GetPath());
//    EXPECT_EQ(Core::Test::Data::FilledDirPath(), info.GetDirectoryName());
//    EXPECT_EQ(Core::Test::Data::RegularFileName(), info.GetFilename());
//    EXPECT_EQ(Core::Test::Data::RegularFileNameNoExtension(), info.GetFilenameWithoutExtension());
//    EXPECT_EQ(Core::Test::Data::RegularFileExtension(), info.GetExtension());
//    EXPECT_EQ(nullptr, info.GetDirectory());
//}
//
//TEST_FIXTURE(DirectoryInfoTest, ConstructFileNonExisting)
//{
//    string path = Core::Test::Data::DummyFilePath();
//    Core::Time::DateTime dummyTime;
//
//    DirectoryInfo info(path);
//
//    EXPECT_EQ(path, info.FullPath());
//    EXPECT_EQ(Core::Test::Data::DummyFileName(), info.Name());
//    EXPECT_EQ(dummyTime, info.CreationTime());
//    EXPECT_EQ(dummyTime, info.LastAccessTime());
//    EXPECT_EQ(dummyTime, info.LastWriteTime());
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
//    EXPECT_FALSE(info.IsRegularFile());
//    EXPECT_FALSE(info.IsSymlink());
//    EXPECT_FALSE(info.IsDirectory());
//    EXPECT_EQ(0x00, info.GetRights());
//    EXPECT_FALSE(info.IsReadable());
//    EXPECT_FALSE(info.IsWritable());
//    EXPECT_FALSE(info.IsExecutable());
//    EXPECT_FALSE(info.IsReadOnly());
//    EXPECT_EQ(Core::Test::Data::DummyFilePath(), info.GetPath());
//    EXPECT_EQ(Core::Test::Data::FilledDirPath(), info.GetDirectoryName());
//    EXPECT_EQ(Core::Test::Data::DummyFileName(), info.GetFilename());
//    EXPECT_EQ(Core::Test::Data::DummyFileNameNoExtension(), info.GetFilenameWithoutExtension());
//    EXPECT_EQ(Core::Test::Data::DummyFileExtension(), info.GetExtension());
//    EXPECT_EQ(nullptr, info.GetDirectory());
//}
//
//TEST_FIXTURE(DirectoryInfoTest, ConstructFileHidden)
//{
//    string path = Core::Test::Data::HiddenFilePath();
//    Core::Time::DateTime dummyTime;
//
//    DirectoryInfo info(path);
//
//    EXPECT_EQ(path, info.FullPath());
//    EXPECT_EQ(Core::Test::Data::HiddenFileName(), info.Name());
//    EXPECT_NE(dummyTime, info.CreationTime());
//    EXPECT_NE(dummyTime, info.LastAccessTime());
//    EXPECT_NE(dummyTime, info.LastWriteTime());
//    EXPECT_TRUE((info.Attributes() & FileAttributes::Normal) != 0);
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
//    EXPECT_TRUE(info.IsRegularFile());
//    EXPECT_FALSE(info.IsSymlink());
//    EXPECT_FALSE(info.IsDirectory());
//    EXPECT_EQ(0x06, info.GetRights());
//    EXPECT_TRUE(info.IsReadable());
//    EXPECT_TRUE(info.IsWritable());
//    EXPECT_FALSE(info.IsExecutable());
//    EXPECT_FALSE(info.IsReadOnly());
//    EXPECT_EQ(Core::Test::Data::HiddenFilePath(), info.GetPath());
//    EXPECT_EQ(Core::Test::Data::FilledDirPath(), info.GetDirectoryName());
//    EXPECT_EQ(Core::Test::Data::HiddenFileName(), info.GetFilename());
//    EXPECT_EQ(Core::Test::Data::HiddenFileNameNoExtension(), info.GetFilenameWithoutExtension());
//    EXPECT_EQ(Core::Test::Data::HiddenFileExtension(), info.GetExtension());
//    EXPECT_EQ(nullptr, info.GetDirectory());
//}
//
//TEST_FIXTURE(DirectoryInfoTest, ConstructFileSymlink)
//{
//    string path = Core::Test::Data::SymlinkFilePath();
//    Core::Time::DateTime dummyTime;
//
//    DirectoryInfo info(path);
//
//    EXPECT_EQ(path, info.FullPath());
//    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), info.Name());
//    EXPECT_NE(dummyTime, info.CreationTime());
//    EXPECT_NE(dummyTime, info.LastAccessTime());
//    EXPECT_NE(dummyTime, info.LastWriteTime());
//    EXPECT_TRUE((info.Attributes() & FileAttributes::Normal) != 0);
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
//    EXPECT_FALSE(info.IsRegularFile());
//    EXPECT_TRUE(info.IsSymlink());
//    EXPECT_FALSE(info.IsDirectory());
//    EXPECT_EQ(0x07, info.GetRights());
//    EXPECT_TRUE(info.IsReadable());
//    EXPECT_TRUE(info.IsWritable());
//    EXPECT_TRUE(info.IsExecutable());
//    EXPECT_FALSE(info.IsReadOnly());
//    EXPECT_EQ(Core::Test::Data::SymlinkFilePath(), info.GetPath());
//    EXPECT_EQ(Core::Test::Data::FilledDirPath(), info.GetDirectoryName());
//    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), info.GetFilename());
//    EXPECT_EQ(Core::Test::Data::SymlinkFileNameNoExtension(), info.GetFilenameWithoutExtension());
//    EXPECT_EQ(Core::Test::Data::SymlinkFileExtension(), info.GetExtension());
//    EXPECT_EQ(nullptr, info.GetDirectory());
//}
//
//TEST_FIXTURE(DirectoryInfoTest, ConstructDirectoryExisting)
//{
//    string path = Core::Test::Data::FilledDirPath();
//    Core::Time::DateTime dummyTime;
//
//    DirectoryInfo info(path);
//
//    EXPECT_EQ(path, info.FullPath());
//    EXPECT_EQ(Core::Test::Data::FilledDirName(), info.Name());
//    EXPECT_NE(dummyTime, info.CreationTime());
//    EXPECT_NE(dummyTime, info.LastAccessTime());
//    EXPECT_NE(dummyTime, info.LastWriteTime());
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
//    EXPECT_TRUE((info.Attributes() & FileAttributes::Directory) != 0);
//    EXPECT_FALSE(info.IsRegularFile());
//    EXPECT_FALSE(info.IsSymlink());
//    EXPECT_TRUE(info.IsDirectory());
//    EXPECT_EQ(0x07, info.GetRights());
//    EXPECT_TRUE(info.IsReadable());
//    EXPECT_TRUE(info.IsWritable());
//    EXPECT_TRUE(info.IsExecutable());
//    EXPECT_FALSE(info.IsReadOnly());
//}
//
//TEST_FIXTURE(DirectoryInfoTest, ConstructDirectoryNonExisting)
//{
//    string path = Core::Test::Data::DummyDirPath();
//    Core::Time::DateTime dummyTime;
//
//    DirectoryInfo info(path);
//
//    EXPECT_EQ(path, info.FullPath());
//    EXPECT_EQ(Core::Test::Data::DummyDirName(), info.Name());
//    EXPECT_EQ(dummyTime, info.CreationTime());
//    EXPECT_EQ(dummyTime, info.LastAccessTime());
//    EXPECT_EQ(dummyTime, info.LastWriteTime());
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
//    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
//    EXPECT_FALSE(info.IsRegularFile());
//    EXPECT_FALSE(info.IsSymlink());
//    EXPECT_FALSE(info.IsDirectory());
//    EXPECT_EQ(0x00, info.GetRights());
//    EXPECT_FALSE(info.IsReadable());
//    EXPECT_FALSE(info.IsWritable());
//    EXPECT_FALSE(info.IsExecutable());
//    EXPECT_FALSE(info.IsReadOnly());
//}

} // namespace Test
} // namespace File
} // namespace Core
