#include "unit-test-c++/UnitTestC++.h"

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
    EXPECT_EQ(_("."), Path::ResolveTilde(_(".")));
    EXPECT_EQ(Test::Data::FilledDirPath(), Path::ResolveTilde(Test::Data::FilledDirPath()));
	OSAL::String home = OSAL::System::getenv(_("HOME"));
    OSAL::String expected = Path::CombinePath(home, _("data"));
    EXPECT_EQ(expected, Path::ResolveTilde(_("~/data")));
}

TEST_FIXTURE(PathTest, SplitPath)
{
	OSAL::String path = Test::Data::RegularFilePath();
	OSAL::String directoryActual;
	OSAL::String fileNameActual;
	OSAL::String directoryExpected = Test::Data::FilledDirPath();
	OSAL::String fileNameExpected = Test::Data::RegularFileName();

    Path::SplitPath(path, directoryActual, fileNameActual);
    EXPECT_EQ(directoryExpected, directoryActual);
    EXPECT_EQ(fileNameExpected, fileNameActual);
}

TEST_FIXTURE(PathTest, Extension)
{
	OSAL::String path = Test::Data::RegularFilePath();
	OSAL::String expected = Test::Data::RegularFileExtension();

    EXPECT_EQ(expected, Path::Extension(path));
}

TEST_FIXTURE(PathTest, StripExtension)
{
	OSAL::String path = Test::Data::RegularFilePath();
	OSAL::String expected = Path::CombinePath(Test::Data::FilledDirPath(), Test::Data::RegularFileNameNoExtension());

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
	OSAL::String basePath = _("/Home/User");
	OSAL::String subDirectory = _("/Subdirectory");
	OSAL::String expected = basePath + Path::PathSeparator() + subDirectory;

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
	OSAL::String home = OSAL::System::getenv(_("HOME"));
    EXPECT_EQ(home, Path::FullPath(_("~/")));
}

TEST_FIXTURE(PathTest, StripPathToSubDirectory)
{
	OSAL::String currentDirectory = Test::Data::RegularFilePath();
	OSAL::String stripToDirectory = Test::Data::TestDirName();
	OSAL::String result = currentDirectory;
    size_t pathDelimiterPos = result.find_last_of('/');
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    pathDelimiterPos = result.find_last_of('/');
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
    pathDelimiterPos = result.find_last_of('/');
    if (pathDelimiterPos != string::npos)
        result = result.substr(0, pathDelimiterPos);
	OSAL::String expected = result;
    EXPECT_EQ(expected, Path::StripPathToSubDirectory(currentDirectory, stripToDirectory));
}

TEST_FIXTURE(PathTest, RelativePath1)
{
	OSAL::String currentDirectory = Path::CurrentDir();
	OSAL::String newPath = _("/usr");
    EXPECT_EQ(0, OSAL::System::chdir(newPath.c_str()));
	OSAL::String subDirectory = _("lib");
	OSAL::String expected = OSAL::String(_(".")) + Path::PathSeparator() + subDirectory;
	OSAL::String fullPath = Path::CombinePath(newPath, subDirectory);
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, OSAL::System::chdir(currentDirectory.c_str()));
}

TEST_FIXTURE(PathTest, RelativePath2)
{
	OSAL::String currentDirectory = Path::CurrentDir();
	OSAL::String rootDir = _("/usr");
	OSAL::String newPath = Path::CombinePath(rootDir, _("lib"));
    EXPECT_EQ(0, OSAL::System::chdir(newPath.c_str()));
	OSAL::String subDirectory = _("bin");
	OSAL::String expected = OSAL::String(_("..")) + Path::PathSeparator() + subDirectory;
	OSAL::String fullPath = Path::CombinePath(rootDir, subDirectory);
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, OSAL::System::chdir(currentDirectory.c_str()));
}

TEST_FIXTURE(PathTest, RelativePath3)
{
	OSAL::String currentDirectory = Path::CurrentDir();
	OSAL::String newPath = _("/usr");
    EXPECT_EQ(0, OSAL::System::chdir(newPath.c_str()));
	OSAL::String subDirectory = _("lib");
	OSAL::String expected = _(".");
	OSAL::String fullPath = newPath;
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
    EXPECT_EQ(0, OSAL::System::chdir(currentDirectory.c_str()));
}

TEST_FIXTURE(PathTest, RelativePath4)
{
	OSAL::String expected = _("/usr/lib");
	OSAL::String fullPath = _("/usr/lib");
    EXPECT_EQ(expected, Path::RelativePath(fullPath));
}

TEST_FIXTURE(PathTest, CurrentDir)
{
	OSAL::String currentDirectory = Path::CurrentDir();
	OSAL::String newPath = _("/usr/lib");
    EXPECT_EQ(0, OSAL::System::chdir(newPath.c_str()));
	OSAL::String newCurrentDirectory = Path::CurrentDir();
    EXPECT_EQ(0, OSAL::System::chdir(currentDirectory.c_str()));
    EXPECT_EQ(newPath, newCurrentDirectory);
}

} // namespace Test
} // namespace OSAL
