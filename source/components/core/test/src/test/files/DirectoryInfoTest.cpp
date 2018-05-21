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
};

TEST_FIXTURE(DirectoryInfoTest, ConstructDirectoryExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;

    DirectoryInfo info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), info.Name());
    EXPECT_NE(dummyTime, info.CreationTime());
    EXPECT_NE(dummyTime, info.LastAccessTime());
    EXPECT_NE(dummyTime, info.LastWriteTime());
    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(info.IsRegularFile());
    EXPECT_FALSE(info.IsSymlink());
    EXPECT_TRUE(info.IsDirectory());
    EXPECT_EQ(0x07, info.GetRights());
    EXPECT_TRUE(info.IsReadable());
    EXPECT_TRUE(info.IsWritable());
    EXPECT_TRUE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
    EXPECT_FALSE(info.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(path, info.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), info.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), info.GetFilename());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), info.GetFilenameWithoutExtension());
    EXPECT_EQ("", info.GetExtension());
    EXPECT_EQ(nullptr, info.GetDirectory());
}

TEST_FIXTURE(DirectoryInfoTest, ConstructDirectoryNonExisting)
{
    string path = Core::Test::Data::DummyDirPath();
    Core::Time::DateTime dummyTime;

    DirectoryInfo info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), info.Name());
    EXPECT_EQ(dummyTime, info.CreationTime());
    EXPECT_EQ(dummyTime, info.LastAccessTime());
    EXPECT_EQ(dummyTime, info.LastWriteTime());
    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(info.IsRegularFile());
    EXPECT_FALSE(info.IsSymlink());
    EXPECT_FALSE(info.IsDirectory());
    EXPECT_EQ(0x00, info.GetRights());
    EXPECT_FALSE(info.IsReadable());
    EXPECT_FALSE(info.IsWritable());
    EXPECT_FALSE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
    EXPECT_FALSE(info.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(path, info.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), info.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), info.GetFilename());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), info.GetFilenameWithoutExtension());
    EXPECT_EQ("", info.GetExtension());
    EXPECT_EQ(nullptr, info.GetDirectory());
}

TEST_FIXTURE(DirectoryInfoTest, ConstructDirectoryHidden)
{
    string path = Core::Test::Data::HiddenDirPath();
    Core::Time::DateTime dummyTime;

    DirectoryInfo info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), info.Name());
    EXPECT_NE(dummyTime, info.CreationTime());
    EXPECT_NE(dummyTime, info.LastAccessTime());
    EXPECT_NE(dummyTime, info.LastWriteTime());
    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(info.IsRegularFile());
    EXPECT_FALSE(info.IsSymlink());
    EXPECT_TRUE(info.IsDirectory());
    EXPECT_EQ(0x07, info.GetRights());
    EXPECT_TRUE(info.IsReadable());
    EXPECT_TRUE(info.IsWritable());
    EXPECT_TRUE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
    EXPECT_TRUE(info.IsHidden());
    EXPECT_FALSE(FileSystemInfo::IsHidden(path));
    EXPECT_EQ(path, info.GetPath());
    EXPECT_EQ(Core::Test::Data::TestDirectoryCore(), info.GetDirectoryName());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), info.GetFilename());
    EXPECT_EQ(Core::Test::Data::HiddenDirName(), info.GetFilenameWithoutExtension());
    EXPECT_EQ("", info.GetExtension());
    EXPECT_EQ(nullptr, info.GetDirectory());
}

} // namespace Test
} // namespace File
} // namespace Core
