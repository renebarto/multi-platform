#include <unittest-cpp/UnitTestC++.h>

#include "core/files/TextFile.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class TextFileTest : public UnitTestCpp::TestFixture
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

static std::ios::openmode Closed = static_cast<std::ios::openmode>(0);

TEST_FIXTURE(TextFileTest, ConstructFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    FileInfo info(path);

    TextFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromFileInfoNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    FileInfo info(path);

    TextFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromFileInfoHidden)
{
    string path = Core::Test::Data::HiddenFilePath();
    FileInfo info(path);

    TextFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromFileInfoSymlink)
{
    string path = Core::Test::Data::SymlinkFilePath();
    FileInfo info(path);

    TextFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromFileInfoWithOpenParametersFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    FileInfo info(path);

    TextFile file(info, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromFileInfoWithOpenParametersNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    FileInfo info(path);

    TextFile file(info, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathExisting)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathHidden)
{
    string path = Core::Test::Data::HiddenFilePath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathSymlink)
{
    string path = Core::Test::Data::SymlinkFilePath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathWithOpenParametersFromFileInfoExistingDefault)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathWithOpenParametersFromFileInfoExistingBinary)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathWithOpenParametersFromFileInfoExistingText)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly | DesiredAccess::Text, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructFromPathWithOpenParametersNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();

    TextFile file(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructCopyExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    TextFile other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    TextFile file(other);

    EXPECT_TRUE(other.IsOpen());
    EXPECT_EQ(std::ios::in, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, ConstructMoveExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    TextFile other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    TextFile file(std::move(other));

    EXPECT_FALSE(other.IsOpen());
    EXPECT_EQ(Closed, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, AssignCopyExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    TextFile other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    TextFile file(Core::Test::Data::DummyFilePath());
    file = other;

    EXPECT_TRUE(other.IsOpen());
    EXPECT_EQ(std::ios::in, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, AssignMoveExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    TextFile other(path, DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    TextFile file(Core::Test::Data::DummyFilePath());
    file = std::move(other);

    EXPECT_FALSE(other.IsOpen());
    EXPECT_EQ(Closed, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(TextFileTest, OpenReadOpenExisting_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_TRUE(file.IsOpen());
}

TEST_FIXTURE(TextFileTest, OpenReadOpenExisting_NonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
}

TEST_FIXTURE(TextFileTest, OpenReadCreateNew_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
}

TEST_FIXTURE(TextFileTest, OpenReadCreateOrOverwrite_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
}

TEST_FIXTURE(TextFileTest, OpenReadOpenOrCreate_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
}

TEST_FIXTURE(TextFileTest, OpenReadTruncateExisting_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadOnly, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
}

TEST_FIXTURE(TextFileTest, OpenWriteExisting_Existing)
{
    TextFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteExisting_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteCreateNew_Existing)
{
    TextFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteCreateNew_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteOpenOrCreate_Existing)
{
    TextFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteOpenOrCreate_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteCreateOrOverwrite_Existing)
{
    TextFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteCreateOrOverwrite_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteTruncateExisting_Existing)
{
    TextFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, OpenWriteTruncateExisting_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFile2Path();
    string pathDst = Core::Test::Data::RegularFile3Path();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CompareTo(pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFile2Path();
    string pathDst = Core::Test::Data::RegularFile4Path();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToFileExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFile2Path();
    string pathDst = Core::Test::Data::RegularFile3Path();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToFileExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFile2Path();
    string pathDst = Core::Test::Data::RegularFile4Path();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToFileExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, CompareToFileNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(TextFileTest, ComparePathToPathExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFile2Path();
    string pathDst = Core::Test::Data::RegularFile3Path();

    EXPECT_TRUE(TextFile::Exists(pathSrc));
    EXPECT_TRUE(TextFile::Exists(pathDst));
    EXPECT_TRUE(TextFile::Compare(pathSrc, pathDst));
    EXPECT_TRUE(TextFile::Exists(pathSrc));
    EXPECT_TRUE(TextFile::Exists(pathDst));
}

TEST_FIXTURE(TextFileTest, ComparePathToPathExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFile2Path();
    string pathDst = Core::Test::Data::RegularFile4Path();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(TextFile::Exists(pathSrc));
    EXPECT_TRUE(TextFile::Exists(pathDst));
    EXPECT_FALSE(TextFile::Compare(pathSrc, pathDst));
    EXPECT_TRUE(TextFile::Exists(pathSrc));
    EXPECT_TRUE(TextFile::Exists(pathDst));
}

TEST_FIXTURE(TextFileTest, ComparePathToPathExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_TRUE(TextFile::Exists(pathSrc));
    EXPECT_FALSE(TextFile::Exists(pathDst));
    EXPECT_FALSE(TextFile::Compare(pathSrc, pathDst));
    EXPECT_TRUE(TextFile::Exists(pathSrc));
    EXPECT_FALSE(TextFile::Exists(pathDst));
}

TEST_FIXTURE(TextFileTest, ComparePathToPathNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    TextFile fileSrc(pathSrc);
    TextFile fileDst(pathDst);

    EXPECT_FALSE(TextFile::Exists(pathSrc));
    EXPECT_TRUE(TextFile::Exists(pathDst));
    EXPECT_FALSE(TextFile::Compare(pathSrc, pathDst));
    EXPECT_FALSE(TextFile::Exists(pathSrc));
    EXPECT_TRUE(TextFile::Exists(pathDst));
}

TEST_FIXTURE(TextFileTest, ReadCharClosedFile)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    EXPECT_EQ('\0', file.ReadChar());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, ReadChar)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    EXPECT_EQ('H', file.ReadChar());
    EXPECT_EQ('e', file.ReadChar());
    EXPECT_EQ('l', file.ReadChar());
    EXPECT_EQ('l', file.ReadChar());
    EXPECT_EQ('o', file.ReadChar());
    EXPECT_EQ('\n', file.ReadChar());
    EXPECT_EQ('H', file.ReadChar());
    EXPECT_EQ('e', file.ReadChar());
    EXPECT_EQ('r', file.ReadChar());
    EXPECT_EQ('e', file.ReadChar());
    EXPECT_EQ(' ', file.ReadChar());
    EXPECT_EQ('I', file.ReadChar());
    EXPECT_EQ(' ', file.ReadChar());
    EXPECT_EQ('a', file.ReadChar());
    EXPECT_EQ('m', file.ReadChar());
    EXPECT_EQ('\n', file.ReadChar());
    EXPECT_EQ('\n', file.ReadChar());
    EXPECT_EQ('\0', file.ReadChar());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, ReadLineClosedFile)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    EXPECT_EQ("", file.ReadLine());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, ReadLine)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    EXPECT_EQ("Hello", file.ReadLine());
    EXPECT_EQ("Here I am", file.ReadLine());
    EXPECT_EQ("", file.ReadLine());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, ReadLineWithBoolResultClosedFile)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    std::string line;
    EXPECT_FALSE(file.ReadLine(line));
    EXPECT_EQ("", line);
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, ReadLineWithBoolResult)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    std::string line;
    EXPECT_TRUE(file.ReadLine(line));
    EXPECT_EQ("Hello", line);
    EXPECT_TRUE(file.ReadLine(line));
    EXPECT_EQ("Here I am", line);
    EXPECT_TRUE(file.ReadLine(line));
    EXPECT_EQ("", line);
    EXPECT_FALSE(file.ReadLine(line));
    EXPECT_EQ("", line);
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, WriteLineClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.WriteLine("X"));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, WriteLine)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.WriteLine("Hello"));
    EXPECT_TRUE(file.WriteLine("Here I am"));
    EXPECT_TRUE(file.WriteLine(""));
    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFilePath()));
}

TEST_FIXTURE(TextFileTest, ReadString)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    string value;
    file.ReadString(value, " \n");
    EXPECT_EQ("Hello", value);
    file.ReadString(value, " \n");
    EXPECT_EQ("Here", value);
    file.ReadString(value, " \n");
    EXPECT_EQ("I", value);
    file.ReadString(value, " \n");
    EXPECT_EQ("am", value);
    file.ReadString(value, " \n");
    EXPECT_EQ("", value);
}

TEST_FIXTURE(TextFileTest, ReadAll)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    string value;
    file.ReadAll(value);
    EXPECT_EQ("Hello\nHere I am\n\n", value);
}

TEST_FIXTURE(TextFileTest, Read_CharClosedFile)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    char ch {};
    EXPECT_FALSE(file.Read(ch));
    EXPECT_EQ('\0', ch);
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Read_Char)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    char ch;
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('H', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('l', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('l', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('o', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('H', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('r', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ(' ', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('I', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ(' ', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('a', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('m', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_TRUE(file.Read(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_FALSE(file.Read(ch));
    EXPECT_EQ('\0', ch);
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Read_Int8ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    int8_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int8_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int8)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int8_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int8_t {123}, value);
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int8_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt8ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    uint8_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint8_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt8)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint8_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint8_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint8_t {234}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int16ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int16_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int16)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int16_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int16_t {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int16_t {12345}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int16LittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {12345}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int16BigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int16_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt16ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint16_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt16)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint16_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint16_t {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint16_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint16_t {34567}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt16LittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {12345}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {34567}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt16BigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint16_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int32ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int32_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int32)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {12345678}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int32LittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {12345678}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int32BigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int32_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt32ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint32_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt32)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {34567890}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt32LittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {12345}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {34567890}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt32BigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint32_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int64ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int64_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int64)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {34567890}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {9223372036854775807LL}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int64LittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {34567890}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {9223372036854775807LL}, value);
}

TEST_FIXTURE(TextFileTest, Read_Int64BigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int64_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt64ClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint64_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt64)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {34567890}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {9223372036854775807LL}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {18446744073709551615ULL}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt64LittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {12345}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {34567890}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {9223372036854775807LL}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {18446744073709551615ULL}, value);
}

TEST_FIXTURE(TextFileTest, Read_UInt64BigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint64_t {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_FloatClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    float value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(float {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Float)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    float value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {3.4567890E7F}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {9.223372036854775807E18F}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {1.8446744073709551615E19F}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {1.0002e4F}, value);
}

TEST_FIXTURE(TextFileTest, Read_FloatLittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    float value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {3.4567890E7F}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {9.223372036854775807E18F}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {1.8446744073709551615E19F}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {1.0002e4F}, value);
}

TEST_FIXTURE(TextFileTest, Read_FloatBigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    float value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(float {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_DoubleClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    double value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(double {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_Double)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    double value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {123}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {234}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {34567890}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_NEAR(double {9.223372036854775807E18}, value, 5E5);
    EXPECT_TRUE(file.Read(value));
    EXPECT_NEAR(double {1.8446744073709551615E19}, value, 5E5);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {1.0002e4}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {1.00002E05}, value);
}

TEST_FIXTURE(TextFileTest, Read_DoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    double value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {123}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {234}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {12345}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {34567}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {12345678}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {34567890}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_NEAR(double {9.223372036854775807E18}, value, 5E5);
    EXPECT_TRUE(file.Read(value));
    EXPECT_NEAR(double {1.8446744073709551615E19}, value, 5E5);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {1.0002e4}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {1.00002E05}, value);
}

TEST_FIXTURE(TextFileTest, Read_DoubleBigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    double value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(double {0}, value);
}

TEST_FIXTURE(TextFileTest, Read_LongDoubleClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    long double value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_TRUE(0.0L == value);
}

TEST_FIXTURE(TextFileTest, Read_LongDouble)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    long double value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(123L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(234L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(12345L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(34567L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(12345678L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(34567890L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(Util::Compare(9.223372036854775807E18L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(Util::Compare(1.8446744073709551615E19L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.0002e4L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
}

TEST_FIXTURE(TextFileTest, Read_LongDoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    long double value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(123L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(234L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(12345L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(34567L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(12345678L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(34567890L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(Util::Compare(9.223372036854775807E18L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(Util::Compare(1.8446744073709551615E19L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.0002e4L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
}

TEST_FIXTURE(TextFileTest, Read_LongDoubleBigEndian)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    long double value {};
    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(0.0L == value);
}

TEST_FIXTURE(TextFileTest, Read_StringClosedFile)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    string value;
    EXPECT_FALSE(file.Read(value, 1));
    EXPECT_EQ("", value);
}

TEST_FIXTURE(TextFileTest, Read_String)
{
    string path = Core::Test::Data::RegularFile4Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    string value;
    EXPECT_TRUE(file.Read(value, 118));
    EXPECT_EQ("123 234 12345 34567 12345678 34567890 9223372036854775807 18446744073709551615 1.0002e4 1.00002E05 1.00002E05 abc def\n", value);
}

TEST_FIXTURE(TextFileTest, Read_ByteArrayClosedFile)
{
    string path = Core::Test::Data::RegularFile6Path();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    OSAL::ByteArray value;
    EXPECT_FALSE(file.Read(value, 1));
    EXPECT_EQ(size_t {0}, value.Size());
}

TEST_FIXTURE(TextFileTest, Read_ByteArray)
{
    string path = Core::Test::Data::RegularFile6Path();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    OSAL::ByteArray expected({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF });
    OSAL::ByteArray actual;
    EXPECT_TRUE(file.Read(actual, 16));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TextFileTest, Write_CharClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write('X'));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Char)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write('H'));
    EXPECT_TRUE(file.Write('e'));
    EXPECT_TRUE(file.Write('l'));
    EXPECT_TRUE(file.Write('l'));
    EXPECT_TRUE(file.Write('o'));
    EXPECT_TRUE(file.Write('\n'));
    EXPECT_TRUE(file.Write('H'));
    EXPECT_TRUE(file.Write('e'));
    EXPECT_TRUE(file.Write('r'));
    EXPECT_TRUE(file.Write('e'));
    EXPECT_TRUE(file.Write(' '));
    EXPECT_TRUE(file.Write('I'));
    EXPECT_TRUE(file.Write(' '));
    EXPECT_TRUE(file.Write('a'));
    EXPECT_TRUE(file.Write('m'));
    EXPECT_TRUE(file.Write('\n'));
    EXPECT_TRUE(file.Write('\n'));
    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFilePath()));
}

TEST_FIXTURE(TextFileTest, Write_Int8ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    int8_t value {123};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Int8)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int8_t value {123};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("123", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt8ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    uint8_t value {234};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_UInt8)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint8_t value {234};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("234", text);
}

TEST_FIXTURE(TextFileTest, Write_Int16ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    int16_t value {12345};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Int16)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int16_t value {12345};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("12345", text);
}

TEST_FIXTURE(TextFileTest, Write_Int16LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int16_t value {12345};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("12345", text);
}

TEST_FIXTURE(TextFileTest, Write_Int16BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int16_t value {12345};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt16ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    uint16_t value {34567};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_UInt16)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint16_t value {34567};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("34567", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt16LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint16_t value {34567};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("34567", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt16BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint16_t value {34567};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_Int32ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    int32_t value {12345678};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Int32)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int32_t value {12345678};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("12345678", text);
}

TEST_FIXTURE(TextFileTest, Write_Int32LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int32_t value {12345678};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("12345678", text);
}

TEST_FIXTURE(TextFileTest, Write_Int32BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int32_t value {12345678};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt32ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    uint32_t value {34567890};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_UInt32)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint32_t value {34567890};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("34567890", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt32LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint32_t value {34567890};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("34567890", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt32BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint32_t value {34567890};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_Int64ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    int64_t value {9223372036854775807LL};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Int64)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int64_t value {9223372036854775807LL};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("9223372036854775807", text);
}

TEST_FIXTURE(TextFileTest, Write_Int64LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int64_t value {9223372036854775807LL};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("9223372036854775807", text);
}

TEST_FIXTURE(TextFileTest, Write_Int64BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    int64_t value {9223372036854775807LL};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt64ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    uint64_t value {18446744073709551615ULL};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_UInt64)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint64_t value {18446744073709551615ULL};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("18446744073709551615", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt64LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint64_t value {18446744073709551615ULL};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("18446744073709551615", text);
}

TEST_FIXTURE(TextFileTest, Write_UInt64BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    uint64_t value {18446744073709551615ULL};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_FloatClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    float value {1.0002e4F};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Float)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    float value {1.0002e4F};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("10002", text);
}

TEST_FIXTURE(TextFileTest, Write_FloatLittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    float value {1.0002e4F};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("10002", text);
}

TEST_FIXTURE(TextFileTest, Write_FloatBigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    float value {1.0002e4F};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_DoubleClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    double value {1.0002e5};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_Double)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    double value {1.00002E05};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("100002", text);
}

TEST_FIXTURE(TextFileTest, Write_DoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    double value {1.00002E05};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("100002", text);
}

TEST_FIXTURE(TextFileTest, Write_DoubleBigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    double value {1.00002E05};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_LongDoubleClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    long double value {1.0002e5};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_LongDouble)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    long double value {1.00002E05L};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("100002", text);
}

TEST_FIXTURE(TextFileTest, Write_LongDoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    long double value {1.00002E05L};
    EXPECT_TRUE(file.Write(value, OSAL::Endianness::LittleEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("100002", text);
}

TEST_FIXTURE(TextFileTest, Write_LongDoubleBigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    long double value {1.00002E05L};
    EXPECT_FALSE(file.Write(value, OSAL::Endianness::BigEndian));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("", text);
}

TEST_FIXTURE(TextFileTest, Write_StringClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    string value {"123"};
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_String)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    string value {"abcdef"};
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("abcdef", text);
}

TEST_FIXTURE(TextFileTest, Write_ByteArrayClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    OSAL::ByteArray value({123});
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, Write_ByteArray)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    OSAL::ByteArray value({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF });
    EXPECT_TRUE(file.Write(value));
    file.Close();
    EXPECT_TRUE(file.Open(DesiredAccess::ReadOnly));
    string text;
    EXPECT_TRUE(file.ReadAll(text));
    EXPECT_EQ("00 01 02 03 04 05 06 07 F8 F9 FA FB FC FD FE FF", text);
}

} // namespace Test
} // namespace Files
} // namespace Core
