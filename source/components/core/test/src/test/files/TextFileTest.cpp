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

TEST_FIXTURE(TextFileTest, ReadCharWithBoolResultClosedFile)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path);

    EXPECT_TRUE(file.Exists());
    char ch;
    EXPECT_FALSE(file.ReadChar(ch));
    EXPECT_EQ('\0', ch);
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(TextFileTest, ReadCharWithBoolResult)
{
    string path = Core::Test::Data::RegularFilePath();

    TextFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    char ch;
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('H', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('l', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('l', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('o', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('H', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('r', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ(' ', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('I', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ(' ', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('a', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('m', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_TRUE(file.ReadChar(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_FALSE(file.ReadChar(ch));
    EXPECT_EQ('\n', ch);
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

TEST_FIXTURE(TextFileTest, WriteCharClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.WriteChar('X'));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(TextFileTest, WriteChar)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.WriteChar('H'));
    EXPECT_TRUE(file.WriteChar('e'));
    EXPECT_TRUE(file.WriteChar('l'));
    EXPECT_TRUE(file.WriteChar('l'));
    EXPECT_TRUE(file.WriteChar('o'));
    EXPECT_TRUE(file.WriteChar('\n'));
    EXPECT_TRUE(file.WriteChar('H'));
    EXPECT_TRUE(file.WriteChar('e'));
    EXPECT_TRUE(file.WriteChar('r'));
    EXPECT_TRUE(file.WriteChar('e'));
    EXPECT_TRUE(file.WriteChar(' '));
    EXPECT_TRUE(file.WriteChar('I'));
    EXPECT_TRUE(file.WriteChar(' '));
    EXPECT_TRUE(file.WriteChar('a'));
    EXPECT_TRUE(file.WriteChar('m'));
    EXPECT_TRUE(file.WriteChar('\n'));
    EXPECT_TRUE(file.WriteChar('\n'));
    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFilePath()));
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

} // namespace Test
} // namespace Files
} // namespace Core
