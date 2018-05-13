#include <unittest-cpp/UnitTestC++.h>

#include "core/files/FileSystemInfo.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class FileSystemInfoAccessor : public FileSystemInfo
{
public:
    explicit FileSystemInfoAccessor(const string & path) :
        FileSystemInfo(path)
    {
    }
};

class FileSystemInfoTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void FileSystemInfoTest::SetUp()
{
}

void FileSystemInfoTest::TearDown()
{
}

TEST_FIXTURE(FileSystemInfoTest, ConstructFileExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;

    FileSystemInfoAccessor info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), info.Name());
    EXPECT_NE(dummyTime, info.CreationTime());
    EXPECT_NE(dummyTime, info.LastAccessTime());
    EXPECT_NE(dummyTime, info.LastWriteTime());
    EXPECT_TRUE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(info.IsRegularFile());
    EXPECT_FALSE(info.IsDirectory());
    EXPECT_EQ(0x06, info.GetRights());
    EXPECT_TRUE(info.IsReadable());
    EXPECT_TRUE(info.IsWritable());
    EXPECT_FALSE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
}

TEST_FIXTURE(FileSystemInfoTest, ConstructFileNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    Core::Time::DateTime dummyTime;

    FileSystemInfoAccessor info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), info.Name());
    EXPECT_EQ(dummyTime, info.CreationTime());
    EXPECT_EQ(dummyTime, info.LastAccessTime());
    EXPECT_EQ(dummyTime, info.LastWriteTime());
    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(info.IsRegularFile());
    EXPECT_FALSE(info.IsDirectory());
    EXPECT_EQ(0x00, info.GetRights());
    EXPECT_FALSE(info.IsReadable());
    EXPECT_FALSE(info.IsWritable());
    EXPECT_FALSE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
}

TEST_FIXTURE(FileSystemInfoTest, ConstructFileHidden)
{
    string path = Core::Test::Data::HiddenFilePath();
    Core::Time::DateTime dummyTime;

    FileSystemInfoAccessor info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), info.Name());
    EXPECT_NE(dummyTime, info.CreationTime());
    EXPECT_NE(dummyTime, info.LastAccessTime());
    EXPECT_NE(dummyTime, info.LastWriteTime());
    EXPECT_TRUE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_TRUE(info.IsRegularFile());
    EXPECT_FALSE(info.IsDirectory());
    EXPECT_EQ(0x06, info.GetRights());
    EXPECT_TRUE(info.IsReadable());
    EXPECT_TRUE(info.IsWritable());
    EXPECT_FALSE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
}

TEST_FIXTURE(FileSystemInfoTest, ConstructDirectoryExisting)
{
    string path = Core::Test::Data::FilledDirPath();
    Core::Time::DateTime dummyTime;

    FileSystemInfoAccessor info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::FilledDirName(), info.Name());
    EXPECT_NE(dummyTime, info.CreationTime());
    EXPECT_NE(dummyTime, info.LastAccessTime());
    EXPECT_NE(dummyTime, info.LastWriteTime());
    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_TRUE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(info.IsRegularFile());
    EXPECT_TRUE(info.IsDirectory());
    EXPECT_EQ(0x07, info.GetRights());
    EXPECT_TRUE(info.IsReadable());
    EXPECT_TRUE(info.IsWritable());
    EXPECT_TRUE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
}

TEST_FIXTURE(FileSystemInfoTest, ConstructDirectoryNonExisting)
{
    string path = Core::Test::Data::DummyDirPath();
    Core::Time::DateTime dummyTime;

    FileSystemInfoAccessor info(path);

    EXPECT_EQ(path, info.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyDirName(), info.Name());
    EXPECT_EQ(dummyTime, info.CreationTime());
    EXPECT_EQ(dummyTime, info.LastAccessTime());
    EXPECT_EQ(dummyTime, info.LastWriteTime());
    EXPECT_FALSE((info.Attributes() & FileAttributes::Normal) != 0);
    EXPECT_FALSE((info.Attributes() & FileAttributes::Directory) != 0);
    EXPECT_FALSE(info.IsRegularFile());
    EXPECT_FALSE(info.IsDirectory());
    EXPECT_EQ(0x00, info.GetRights());
    EXPECT_FALSE(info.IsReadable());
    EXPECT_FALSE(info.IsWritable());
    EXPECT_FALSE(info.IsExecutable());
    EXPECT_FALSE(info.IsReadOnly());
}

} // namespace Test
} // namespace File
} // namespace Core
