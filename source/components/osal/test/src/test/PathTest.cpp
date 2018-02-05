#include <unittest-cpp/UnitTestC++.h>

#include "osal/Path.h"
#include "osal/TestData.h"

using namespace std;

namespace OSAL
{
namespace Test
{

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

TEST_SUITE(osal)
{

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

TEST_FIXTURE(PathTest, HomePath)
{
    EXPECT_NE("", Path::HomePath());
}

TEST_FIXTURE(PathTest, ResolveTilde)
{
    EXPECT_EQ(".", Path::ResolveTilde("."));
    EXPECT_EQ(Test::Data::FilledDirPath(), Path::ResolveTilde(Test::Data::FilledDirPath()));
    string expected = Path::CombinePath(Path::HomePath(), "data");
    EXPECT_EQ(expected, Path::ResolveTilde(Path::CombinePath("~", "data")));
}

TEST_FIXTURE(PathTest, SplitPath)
{
    string path = Test::Data::RegularFilePath();
    string directoryActual;
    string fileNameActual;
    string directoryExpected = Test::Data::FilledDirPath();
    string fileNameExpected  = Test::Data::RegularFileName();

    Path::SplitPath(path, directoryActual, fileNameActual);
    EXPECT_EQ(directoryExpected, directoryActual);
    EXPECT_EQ(fileNameExpected, fileNameActual);
}

TEST_FIXTURE(PathTest, Extension)
{
    string path     = Test::Data::RegularFilePath();
    string expected = Test::Data::RegularFileExtension();

    EXPECT_EQ(expected, Path::Extension(path));
}

TEST_FIXTURE(PathTest, StripExtension)
{
    string path     = Test::Data::RegularFilePath();
    string expected = Path::CombinePath(Test::Data::FilledDirPath(),
                                        Test::Data::RegularFileNameNoExtension());

    EXPECT_EQ(expected, Path::StripExtension(path));
}

TEST_FIXTURE(PathTest, HasNoPath)
{
    EXPECT_FALSE(Path::HasNoPath(Test::Data::RegularFilePath()));
    EXPECT_TRUE(Path::HasNoPath(Test::Data::RegularFileName()));
}

TEST_FIXTURE(PathTest, CreatePathWithDefault)
{
    EXPECT_EQ(Test::Data::RegularFilePath(),
              Path::CreatePathWithDefault(Test::Data::RegularFilePath(),
                                          Test::Data::FilledDirPath()));
    EXPECT_EQ(Test::Data::RegularFilePath(),
              Path::CreatePathWithDefault(Test::Data::RegularFileName(),
                                          Test::Data::FilledDirPath()));
}

TEST_FIXTURE(PathTest, CombinePath)
{
    string basePath     = string(1, Path::PathSeparator()) + "Home" + Path::PathSeparator() + "User";
    string subDirectory = "Subdirectory";
    string expected     = basePath + Path::PathSeparator() + subDirectory;

    EXPECT_EQ(expected, Path::CombinePath(basePath, subDirectory));
}

TEST_FIXTURE(PathTest, FullPath)
{
    string home = Path::AddSeparatorIfNeeded(Path::HomePath());
    EXPECT_EQ(home, Path::AddSeparatorIfNeeded(Path::FullPath(string("~") + Path::PathSeparator())));
}

TEST_FIXTURE(PathTest, StripPathToSubDirectory)
{
    string currentDirectory = Test::Data::RegularFilePath();
    string stripToDirectory = Test::Data::ProjectName();
    string result           = currentDirectory;
    size_t pathDelimiterPos = result.find_last_of(Path::PathSeparator());
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    pathDelimiterPos = result.find_last_of(Path::PathSeparator());
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    string expected = result;
    EXPECT_EQ(expected, Path::StripPathToSubDirectory(currentDirectory, stripToDirectory));
}

TEST_FIXTURE(PathTest, RelativePath1)
{
    string currentDirectory = Path::CurrentDir();
    string rootDir          = Path::StripPathToSubDirectory(Test::Data::RegularFilePath(),
                                                   Test::Data::ProjectName());
    string newPath = rootDir;
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(newPath));
    string subDirectory = Test::Data::FilledDirName();
    string expected     = string(".") + Path::PathSeparator() + subDirectory;
    string fullPath     = Path::CombinePath(newPath, subDirectory);
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(currentDirectory));
}

TEST_FIXTURE(PathTest, RelativePath2)
{
    string currentDirectory = Path::CurrentDir();
    string rootDir          = Path::StripPathToSubDirectory(Test::Data::RegularFilePath(),
                                                            Test::Data::ProjectName());
    string newPath = Path::CombinePath(rootDir, Test::Data::FilledDirName());
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(newPath));
    string subDirectory = "xml";
    string expected     = Path::CombinePath("..", Path::CombinePath("..", subDirectory));
    string fullPath     = Path::CombinePath(rootDir, Path::CombinePath("..", subDirectory));
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(currentDirectory));
}

TEST_FIXTURE(PathTest, RelativePath3)
{
    string currentDirectory = Path::CurrentDir();
    string rootDir          = Path::StripPathToSubDirectory(Test::Data::RegularFilePath(),
                                                            Test::Data::ProjectName());
    string newPath = rootDir;
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(newPath));
    string expected = ".";
    string fullPath = newPath;
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(currentDirectory));
}

TEST_FIXTURE(PathTest, RelativePath4)
{
#if defined(WIN_MSVC) || defined(WIN_MINGW)
    string expected = "A:\\";
#else
    string expected = Path::SystemBinariesPath();
#endif
    string fullPath = expected;
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
}

TEST_FIXTURE(PathTest, CurrentDir)
{
    string currentDirectory = Path::CurrentDir();
    string newPath          = Path::StripPathToSubDirectory(Test::Data::RegularFilePath(),
                                                            Test::Data::ProjectName());
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(newPath));
    string newCurrentDirectory = Path::CurrentDir();
    EXPECT_EQ(0, Path::ChangeCurrentDirectory(currentDirectory));
    EXPECT_EQ(newPath, newCurrentDirectory);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
