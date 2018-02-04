#include <unittest-cpp/UnitTestC++.h>

#include "osal/Exception.h"
#include "osal/Files.h"
#include "osal/TestData.h"

using namespace std;

namespace OSAL {
namespace Test {

class FilesTest : public UnitTestCpp::TestFixture
{
public:
    void TearDown() override
    {
        Files::Remove(Test::Data::DummyFilePath().c_str());
    }
};

TEST_SUITE(osal)
{

TEST_FIXTURE(FilesTest, OpenExisting)
{
    const std::string path = Test::Data::RegularFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDONLY);
    EXPECT_NE(-1, fd);
    Files::Close(fd);
}

TEST_FIXTURE(FilesTest, OpenNonExisting)
{
    const std::string path = Test::Data::DummyFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDONLY);
    EXPECT_EQ(-1, fd);
    EXPECT_EQ(ENOENT, errno);
}

TEST_FIXTURE(FilesTest, OpenNonExistingForWritingNoFilePermissions)
{
    const std::string path = Test::Data::DummyFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDWR | O_CREAT);
    EXPECT_NE(-1, fd);
    mode_t filePermissions = Files::GetFilePermissions(path.c_str());
    EXPECT_EQ(S_IRUSR | S_IWUSR, filePermissions & (S_IRWXU | S_IRWXG | S_IRWXO));
    Files::Close(fd);
}

TEST_FIXTURE(FilesTest, OpenNonExistingForWriting)
{
    const std::string path = Test::Data::DummyFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    EXPECT_NE(-1, fd);
    mode_t filePermissions = Files::GetFilePermissions(path.c_str());
    EXPECT_EQ(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, filePermissions & (S_IRWXU | S_IRWXG | S_IRWXO));
    Files::Close(fd);
}

TEST_FIXTURE(FilesTest, Close)
{
    const std::string path = Test::Data::RegularFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDONLY);
    EXPECT_NE(-1, fd);
    if (-1 == Files::Close(fd))
        ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

TEST_FIXTURE(FilesTest, Create)
{
    const std::string path = Test::Data::DummyFilePath();
    Files::FileDescriptor fd = Files::Create(path.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    EXPECT_NE(-1, fd);
    mode_t filePermissions = Files::GetFilePermissions(path.c_str());
    EXPECT_EQ(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, filePermissions & (S_IRWXU | S_IRWXG | S_IRWXO));
    Files::Close(fd);
}

TEST_FIXTURE(FilesTest, GetSize)
{
    const std::string path = Test::Data::RegularFilePath();

    auto fileSize = Files::GetSize(path.c_str());
#if defined(WIN_MSVC) || defined(WIN_MINGW)
    EXPECT_EQ(long{7}, fileSize);
#else
    EXPECT_EQ(long{6}, fileSize);
#endif
}

TEST_FIXTURE(FilesTest, Read)
{
    const std::string path = Test::Data::RegularFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDONLY);

    uint8_t ref[] { 'H', 'e', 'l', 'l', 'o', '\n' };
    uint8_t buffer[20];
    ssize_t bytesRead = Files::Read(fd, buffer, sizeof(buffer));
    EXPECT_EQ(sizeof(ref), bytesRead);

    EXPECT_EQ(0, memcmp(buffer, ref, sizeof(ref)));

    Files::Close(fd);
}

TEST_FIXTURE(FilesTest, Write)
{
    const std::string path = Test::Data::DummyFilePath();
    Files::FileDescriptor fd = Files::Open(path.c_str(), O_RDWR | O_CREAT);

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    uint8_t data[] { 'H', 'e', 'l', 'l', 'o', ' ', 'b', 'a', 'c', 'k', '\r', '\n' };
#else
    uint8_t data[] { 'H', 'e', 'l', 'l', 'o', ' ', 'b', 'a', 'c', 'k', '\n' };
#endif
    uint8_t buffer[20];
    ssize_t bytesWritten = Files::Write(fd, data, sizeof(data));
    EXPECT_EQ(sizeof(data), bytesWritten);
    Files::Close(fd);

    fd = Files::Open(path.c_str(), O_RDONLY);

    ssize_t bytesRead = Files::Read(fd, buffer, sizeof(buffer));
    EXPECT_EQ(sizeof(data), bytesRead);

    EXPECT_EQ(0, memcmp(buffer, data, sizeof(data)));

    Files::Close(fd);
}

TEST_FIXTURE(FilesTest, Stat)
{
    const std::string path = Test::Data::RegularFilePath();

    struct stat statInfo;
    if (-1 == Files::Stat(path.c_str(), &statInfo))
        ThrowOnError(__func__, __FILE__, __LINE__, errno);

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    EXPECT_EQ(long{7}, statInfo.st_size);
#else
    EXPECT_EQ(long{6}, statInfo.st_size);
#endif
    EXPECT_EQ(S_IRUSR | S_IWUSR, statInfo.st_mode & S_IRWXU);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
