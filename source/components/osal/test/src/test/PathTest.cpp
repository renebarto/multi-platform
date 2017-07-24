#include "unit-test-c++/UnitTestC++.h"

#include <unistd.h>
#include "core/ByteArray.h"
#include "osal/Path.h"
#include "TestData.h"

using namespace std;

namespace OSAL {
namespace Test {

class PathTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void PathTest::SetUp()
{
}

void PathTest::TearDown()
{
}

TEST_FIXTURE(PathTest, FileExists)
{
    EXPECT_TRUE(Path::FileExists(Test::Data::RegularFilePath()));
    EXPECT_FALSE(Path::FileExists(Test::Data::FilledDirPath()));
    EXPECT_FALSE(Path::FileExists(Test::Data::DummyFilePath()));
}

TEST_FIXTURE(PathTest, DirectoryExists)
{
    EXPECT_FALSE(Path::DirectoryExists(Test::Data::RegularFilePath()));
    EXPECT_TRUE(Path::DirectoryExists(Test::Data::FilledDirPath()));
    EXPECT_FALSE(Path::DirectoryExists(Test::Data::DummyDirPath()));
}

TEST_FIXTURE(PathTest, ResolveTilde)
{
    EXPECT_EQ(".", Path::ResolveTilde("."));
    EXPECT_EQ(Test::Data::FilledDirPath(), Path::ResolveTilde(Test::Data::FilledDirPath()));
    string home = getenv("HOME");
    string expected = Path::CombinePath(home, "data");
    EXPECT_EQ(expected, Path::ResolveTilde("~/data"));
}

TEST_FIXTURE(PathTest, SplitPath)
{
    string path = Test::Data::RegularFilePath();
    string directoryActual;
    string fileNameActual;
    string directoryExpected = Test::Data::FilledDirPath();
    string fileNameExpected = Test::Data::RegularFileName();

    Path::SplitPath(path, directoryActual, fileNameActual);
    EXPECT_EQ(directoryExpected, directoryActual);
    EXPECT_EQ(fileNameExpected, fileNameActual);
}

TEST_FIXTURE(PathTest, Extension)
{
    string path = Test::Data::RegularFilePath();
    string expected = Test::Data::RegularFileExtension();

    EXPECT_EQ(expected, Path::Extension(path));
}

TEST_FIXTURE(PathTest, StripExtension)
{
    string path = Test::Data::RegularFilePath();
    string expected = Path::CombinePath(Test::Data::FilledDirPath(), Test::Data::RegularFileNameNoExtension());

    EXPECT_EQ(expected, Path::StripExtension(path));
}

TEST_FIXTURE(PathTest, HasNoPath)
{
    EXPECT_FALSE(Path::HasNoPath(Test::Data::RegularFilePath()));
    EXPECT_TRUE(Path::HasNoPath(Test::Data::RegularFileName()));
}

TEST_FIXTURE(PathTest, CreatePathWithDefault)
{
    EXPECT_EQ(Test::Data::RegularFilePath(), Path::CreatePathWithDefault(Test::Data::RegularFilePath(), Test::Data::FilledDirPath()));
    EXPECT_EQ(Test::Data::RegularFilePath(), Path::CreatePathWithDefault(Test::Data::RegularFileName(), Test::Data::FilledDirPath()));
}

TEST_FIXTURE(PathTest, CombinePath)
{
    string basePath = "/Home/User";
    string subDirectory = "/Subdirectory";
    string expected = basePath + "/" + subDirectory;

    EXPECT_EQ(expected, Path::CombinePath(basePath, subDirectory));
}

TEST_FIXTURE(PathTest, FullPath)
{
    // string currentDirectory = Path::CurrentDir();
    // string subDirectory = "test";
    // string relativePath = "../../" + subDirectory;
    // string result = currentDirectory;
    // size_t pathDelimiterPos = result.find_last_of('/');
    // if (pathDelimiterPos != string::npos)
    //     result = result.substr(0, pathDelimiterPos);
    // pathDelimiterPos = result.find_last_of('/');
    // if (pathDelimiterPos != string::npos)
    //     result = result.substr(0, pathDelimiterPos + 1) + subDirectory;
    // string expected = result;
    // EXPECT_EQ(expected, Path::FullPath(relativePath));
    string home = getenv("HOME");
    EXPECT_EQ(home, Path::FullPath("~/"));
}

TEST_FIXTURE(PathTest, StripPathToSubDirectory)
{
    string currentDirectory = Test::Data::RegularFilePath();
    string stripToDirectory = Test::Data::TestDirName();
    string result = currentDirectory;
    size_t pathDelimiterPos = result.find_last_of('/');
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    pathDelimiterPos = result.find_last_of('/');
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    pathDelimiterPos = result.find_last_of('/');
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    string expected = result;
    EXPECT_EQ(expected, Path::StripPathToSubDirectory(currentDirectory, stripToDirectory));
}

TEST_FIXTURE(PathTest, RelativePath1)
{
    string currentDirectory = Path::CurrentDir();
    string newPath = "/usr";
    EXPECT_EQ(0, chdir(newPath.c_str()));
    string subDirectory = "lib";
    string expected = "./" + subDirectory;
    string fullPath = Path::CombinePath(newPath, subDirectory);
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, chdir(currentDirectory.c_str()));
}

TEST_FIXTURE(PathTest, RelativePath2)
{
    string currentDirectory = Path::CurrentDir();
    string rootDir = "/usr";
    string newPath = Path::CombinePath(rootDir, "lib");
    EXPECT_EQ(0, chdir(newPath.c_str()));
    string subDirectory = "bin";
    string expected = "../" + subDirectory;
    string fullPath = Path::CombinePath(rootDir, subDirectory);
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, chdir(currentDirectory.c_str()));
}

TEST_FIXTURE(PathTest, RelativePath3)
{
    string currentDirectory = Path::CurrentDir();
    string newPath = "/usr";
    EXPECT_EQ(0, chdir(newPath.c_str()));
    string subDirectory = "lib";
    string expected = ".";
    string fullPath = newPath;
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, chdir(currentDirectory.c_str()));
}

TEST_FIXTURE(PathTest, RelativePath4)
{
    string expected = "/usr/lib";
    string fullPath = "/usr/lib";
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
}

TEST_FIXTURE(PathTest, CurrentDir)
{
    string currentDirectory = Path::CurrentDir();
    string newPath = "/usr/lib";
    EXPECT_EQ(0, chdir(newPath.c_str()));
    string newCurrentDirectory = Path::CurrentDir();
    EXPECT_EQ(0, chdir(currentDirectory.c_str()));
    EXPECT_EQ(newPath, newCurrentDirectory);
}

} // namespace Test
} // namespace OSAL
