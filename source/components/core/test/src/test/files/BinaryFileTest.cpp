#include <unittest-cpp/UnitTestC++.h>

#include "core/files/BinaryFile.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class BinaryFileTest : public UnitTestCpp::TestFixture
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

TEST_FIXTURE(BinaryFileTest, ConstructFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    FileInfo info(path);

    BinaryFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromFileInfoNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    FileInfo info(path);

    BinaryFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromFileInfoHidden)
{
    string path = Core::Test::Data::HiddenFilePath();
    FileInfo info(path);

    BinaryFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromFileInfoSymlink)
{
    string path = Core::Test::Data::SymlinkFilePath();
    FileInfo info(path);

    BinaryFile file(info);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromFileInfoWithOpenParametersFromFileInfoExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    FileInfo info(path);

    BinaryFile file(info, DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromFileInfoWithOpenParametersNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    FileInfo info(path);

    BinaryFile file(info,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathExisting)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathHidden)
{
    string path = Core::Test::Data::HiddenFilePath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::HiddenFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathSymlink)
{
    string path = Core::Test::Data::SymlinkFilePath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::SymlinkFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathWithOpenParametersFromFileInfoExistingDefault)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathWithOpenParametersFromFileInfoExistingBinary)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path, DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathWithOpenParametersFromFileInfoExistingText)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path, DesiredAccess::ReadOnly | DesiredAccess::Text, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructFromPathWithOpenParametersNonExisting)
{
    string path = Core::Test::Data::DummyFilePath();

    BinaryFile file(path,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::DummyFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructCopyExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    BinaryFile other(path,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    BinaryFile file(other);

    EXPECT_TRUE(other.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, ConstructMoveExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    BinaryFile other(path,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    BinaryFile file(std::move(other));

    EXPECT_FALSE(other.IsOpen());
    EXPECT_EQ(Closed, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, AssignCopyExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    BinaryFile other(path,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    BinaryFile file(Core::Test::Data::DummyFilePath());
    file = other;

    EXPECT_TRUE(other.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, AssignMoveExisting)
{
    string path = Core::Test::Data::RegularFilePath();
    Core::Time::DateTime dummyTime;
    BinaryFile other(path,  DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting);

    BinaryFile file(Core::Test::Data::DummyFilePath());
    file = std::move(other);

    EXPECT_FALSE(other.IsOpen());
    EXPECT_EQ(Closed, other.GetOpenMode());
    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::binary, file.GetOpenMode());
    EXPECT_EQ(path, file.FullPath());
    EXPECT_EQ(Core::Test::Data::RegularFileName(), file.Name());
}

TEST_FIXTURE(BinaryFileTest, OpenReadOpenExisting_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open( DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_TRUE(file.IsOpen());
}

TEST_FIXTURE(BinaryFileTest, OpenReadOpenExisting_NonExisting)
{
    string path = Core::Test::Data::DummyFilePath();
    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open( DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
}

TEST_FIXTURE(BinaryFileTest, OpenReadCreateNew_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open( DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
}

TEST_FIXTURE(BinaryFileTest, OpenReadCreateOrOverwrite_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open( DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
}

TEST_FIXTURE(BinaryFileTest, OpenReadOpenOrCreate_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open( DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
}

TEST_FIXTURE(BinaryFileTest, OpenReadTruncateExisting_Existing)
{
    string path = Core::Test::Data::RegularFilePath();
    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open( DesiredAccess::ReadOnly | DesiredAccess::Binary, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteExisting_Existing)
{
    BinaryFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteExisting_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteCreateNew_Existing)
{
    BinaryFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteCreateNew_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateNew));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteOpenOrCreate_Existing)
{
    BinaryFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out, file.GetOpenMode());
    EXPECT_EQ(size_t{17}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteOpenOrCreate_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::OpenOrCreate));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteCreateOrOverwrite_Existing)
{
    BinaryFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteCreateOrOverwrite_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::CreateOrOverwrite));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteTruncateExisting_Existing)
{
    BinaryFile::Copy(Core::Test::Data::RegularFilePath(), Core::Test::Data::RegularFileNonExistingPath());
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_TRUE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_TRUE(file.IsOpen());
    EXPECT_EQ(std::ios::in | std::ios::out | std::ios::trunc, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, OpenWriteTruncateExisting_NonExisting)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());

    EXPECT_FALSE(file.Open(DesiredAccess::ReadWrite, ShareMode::ShareReadWrite, CreationFlags::TruncateExisting));

    EXPECT_FALSE(file.IsOpen());
    EXPECT_EQ(Closed, file.GetOpenMode());
    EXPECT_EQ(size_t{0}, file.Size());
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile2Path();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CompareTo(pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile3Path();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(pathDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToFileExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile2Path();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_TRUE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToFileExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile3Path();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToFileExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_TRUE(fileSrc.Exists());
    EXPECT_FALSE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, CompareToFileNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
    EXPECT_FALSE(fileSrc.Exists());
    EXPECT_TRUE(fileDst.Exists());
}

TEST_FIXTURE(BinaryFileTest, ComparePathToPathExistingSourceAndDestinationEqual)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile2Path();

    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
    EXPECT_TRUE(BinaryFile::Exists(pathDst));
    EXPECT_TRUE(BinaryFile::Compare(pathSrc, pathDst));
    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
    EXPECT_TRUE(BinaryFile::Exists(pathDst));
}

TEST_FIXTURE(BinaryFileTest, ComparePathToPathExistingSourceAndDestinationInequal)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFile3Path();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
    EXPECT_TRUE(BinaryFile::Exists(pathDst));
    EXPECT_FALSE(BinaryFile::Compare(pathSrc, pathDst));
    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
    EXPECT_TRUE(BinaryFile::Exists(pathDst));
}

TEST_FIXTURE(BinaryFileTest, ComparePathToPathExistingSourceNonExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFilePath();
    string pathDst = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
    EXPECT_FALSE(BinaryFile::Exists(pathDst));
    EXPECT_FALSE(BinaryFile::Compare(pathSrc, pathDst));
    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
    EXPECT_FALSE(BinaryFile::Exists(pathDst));
}

TEST_FIXTURE(BinaryFileTest, ComparePathToPathNonExistingSourceExistingDestination)
{
    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
    string pathDst = Core::Test::Data::RegularFilePath();

    BinaryFile fileSrc(pathSrc);
    BinaryFile fileDst(pathDst);

    EXPECT_FALSE(BinaryFile::Exists(pathSrc));
    EXPECT_TRUE(BinaryFile::Exists(pathDst));
    EXPECT_FALSE(BinaryFile::Compare(pathSrc, pathDst));
    EXPECT_FALSE(BinaryFile::Exists(pathSrc));
    EXPECT_TRUE(BinaryFile::Exists(pathDst));
}

TEST_FIXTURE(BinaryFileTest, Read_CharClosedFile)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    char ch {};
    EXPECT_FALSE(file.Read(ch));
    EXPECT_EQ('\0', ch);
    EXPECT_TRUE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Read_Char)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path, DesiredAccess::ReadOnly);

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

TEST_FIXTURE(BinaryFileTest, Read_Int8ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    int8_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int8_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int8)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int8_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int8_t {0x46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int8_t {0x1C}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt8ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    uint8_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint8_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt8)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint8_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint8_t {0x46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint8_t {0x1C}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int16ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int16_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int16)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int16_t {0x1C46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int16_t {0x0048}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int16LittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {0x1C46}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {0x0048}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int16BigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int16_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int16_t {0x461C}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int16_t {0x4800}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt16ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint16_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt16)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint16_t {0x1C46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint16_t {0x0048}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt16LittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {0x1C46}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {0x0048}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt16BigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint16_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint16_t {0x461C}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint16_t {0x4800}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int32ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int32_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int32)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {0x00481C46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int32_t {0x005AC347}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int32LittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {0x00481C46}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {0x005AC347}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int32BigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int32_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int32_t {0x461C4800}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int32_t {0x47C35A00}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt32ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint32_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt32)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {0x00481C46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint32_t {0x005AC347}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt32LittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {0x00481C46}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {0x005AC347}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt32BigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint32_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint32_t {0x461C4800}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint32_t {0x47C35A00}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int64ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(int64_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int64)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {0x005AC34700481C46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(int64_t {0x000000000089C340}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int64LittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {0x005AC34700481C46}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {0x000000000089C340}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Int64BigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    int64_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int64_t {0x461C480047C35A00}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int64_t {0x40C3890000000000}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt64ClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(uint64_t {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt64)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {0x005AC34700481C46}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(uint64_t {0x000000000089C340}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt64LittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {0x005AC34700481C46}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {0x000000000089C340}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_UInt64BigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    uint64_t value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint64_t {0x461C480047C35A00}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint64_t {0x40C3890000000000}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_FloatClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    float value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(float {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Float)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    float value {};
    float dummyFloat {};
    double dummyDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {1.0002E4}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(float {1.0002E5}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_FloatLittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    float value {};
    float dummyFloat {};
    double dummyDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {1.0002E4}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {1.0002E5}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_FloatBigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    float value {};
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(float {1.0002E4}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(float {1.0002E5}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_DoubleClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    double value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_EQ(double {0}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_Double)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    double value {};
    float dummyFloat {};
    double dummyDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first LittleEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second LittleEndian float
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {1.0002E4}, value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_EQ(double {1.0002E5}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_DoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    double value {};
    float dummyFloat {};
    double dummyDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first LittleEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second LittleEndian float
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {1.0002E4}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {1.0002E5}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_DoubleBigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    double value {};
    float dummyFloat {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(double {1.0002E4}, value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(double {1.0002E5}, value);
}

TEST_FIXTURE(BinaryFileTest, Read_LongDoubleClosedFile)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path);

    EXPECT_TRUE(file.Exists());
    long double value {};
    EXPECT_FALSE(file.Read(value));
    EXPECT_TRUE(0.0L == value);
}

TEST_FIXTURE(BinaryFileTest, Read_LongDouble)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadWrite);

    EXPECT_TRUE(file.Exists());
    long double value {};
    float dummyFloat {};
    double dummyDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first LittleEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second LittleEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first LittleEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second LittleEndian double
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.0002E4L == value);
    EXPECT_TRUE(file.Read(value));
    EXPECT_TRUE(1.0002E5L == value);
}

TEST_FIXTURE(BinaryFileTest, Read_LongDoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    long double value {};
    float dummyFloat {};
    double dummyDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first LittleEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second LittleEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first LittleEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second LittleEndian double
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(1.0002E4L == value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(1.0002E5L == value);
}

TEST_FIXTURE(BinaryFileTest, Read_LongDoubleBigEndian)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    long double value {};
    float dummyFloat {};
    double dummyDouble {};
    long double dummyLongDouble {};
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first BigEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second BigEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first BigEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second BigEndian double
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip first LittleEndian float
    EXPECT_TRUE(file.Read(dummyFloat)); // Skip second LittleEndian float
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip first LittleEndian double
    EXPECT_TRUE(file.Read(dummyDouble)); // Skip second LittleEndian double
    EXPECT_TRUE(file.Read(dummyLongDouble)); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Read(dummyLongDouble)); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(1.0002E4L == value);
    EXPECT_TRUE(file.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(1.0002E5L == value);
}

TEST_FIXTURE(BinaryFileTest, Read_StringClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    string value;
    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Read(value, 1));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Read_String)
{
    string path = Core::Test::Data::RegularFilePath();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    string value;
    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Read(value, 17));
    EXPECT_EQ("Hello\nHere I am\n\n", value);
}

TEST_FIXTURE(BinaryFileTest, Read_ByteArrayClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    OSAL::ByteArray value;
    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Read(value, 1));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Read_ByteArray)
{
    string path = Core::Test::Data::RegularFile5Path();

    BinaryFile file(path, DesiredAccess::ReadOnly);

    EXPECT_TRUE(file.Exists());
    OSAL::ByteArray expected({ 0x46, 0x1C, 0x48, 0x00, 0x47, 0xC3, 0x5A, 0x00,
                               0x40, 0xC3, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x40, 0xF8, 0x6B, 0x40, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x48, 0x1C, 0x46, 0x00, 0x5A, 0xC3, 0x47,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0xC3, 0x40,
                               0x00, 0x00, 0x00, 0x00, 0x40, 0x6B, 0xF8, 0x40,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x9C,
                               0x0C, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xC3,
                               0x0F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x40, 0x0C, 0x9C, 0x48, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x40, 0x0F, 0xC3, 0x5A, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
    OSAL::ByteArray actual;
    EXPECT_TRUE(file.Read(actual, 112));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(BinaryFileTest, Write_CharClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write('X'));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Char)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

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

TEST_FIXTURE(BinaryFileTest, Write_Int8ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(int8_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Int8)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int8_t {0x46}));
    EXPECT_TRUE(file.Write(int8_t {0x1C}));
    EXPECT_TRUE(file.Write(int8_t {0x48}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x47}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xC3)}));
    EXPECT_TRUE(file.Write(int8_t {0x5A}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xC3)}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0x89)}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xF8)}));
    EXPECT_TRUE(file.Write(int8_t {0x6B}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x48}));
    EXPECT_TRUE(file.Write(int8_t {0x1C}));
    EXPECT_TRUE(file.Write(int8_t {0x46}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x5A}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xC3)}));
    EXPECT_TRUE(file.Write(int8_t {0x47}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0x89)}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xC3)}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x6B}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xF8)}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x48}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0x9C)}));
    EXPECT_TRUE(file.Write(int8_t {0x0C}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x5A}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xC3)}));
    EXPECT_TRUE(file.Write(int8_t {0x0F}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x0C}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0x9C)}));
    EXPECT_TRUE(file.Write(int8_t {0x48}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x40}));
    EXPECT_TRUE(file.Write(int8_t {0x0F}));
    EXPECT_TRUE(file.Write(int8_t {static_cast<int8_t>(0xC3)}));
    EXPECT_TRUE(file.Write(int8_t {0x5A}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));
    EXPECT_TRUE(file.Write(int8_t {0x00}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt8ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(uint8_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_UInt8)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint8_t {0x46}));
    EXPECT_TRUE(file.Write(uint8_t {0x1C}));
    EXPECT_TRUE(file.Write(uint8_t {0x48}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x47}));
    EXPECT_TRUE(file.Write(uint8_t {0xC3}));
    EXPECT_TRUE(file.Write(uint8_t {0x5A}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0xC3}));
    EXPECT_TRUE(file.Write(uint8_t {0x89}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0xF8}));
    EXPECT_TRUE(file.Write(uint8_t {0x6B}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x48}));
    EXPECT_TRUE(file.Write(uint8_t {0x1C}));
    EXPECT_TRUE(file.Write(uint8_t {0x46}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x5A}));
    EXPECT_TRUE(file.Write(uint8_t {0xC3}));
    EXPECT_TRUE(file.Write(uint8_t {0x47}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x89}));
    EXPECT_TRUE(file.Write(uint8_t {0xC3}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x6B}));
    EXPECT_TRUE(file.Write(uint8_t {0xF8}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x48}));
    EXPECT_TRUE(file.Write(uint8_t {0x9C}));
    EXPECT_TRUE(file.Write(uint8_t {0x0C}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x5A}));
    EXPECT_TRUE(file.Write(uint8_t {0xC3}));
    EXPECT_TRUE(file.Write(uint8_t {0x0F}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x0C}));
    EXPECT_TRUE(file.Write(uint8_t {0x9C}));
    EXPECT_TRUE(file.Write(uint8_t {0x48}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x40}));
    EXPECT_TRUE(file.Write(uint8_t {0x0F}));
    EXPECT_TRUE(file.Write(uint8_t {0xC3}));
    EXPECT_TRUE(file.Write(uint8_t {0x5A}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));
    EXPECT_TRUE(file.Write(uint8_t {0x00}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int16ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(int16_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Int16)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int16_t {0x1C46}));
    EXPECT_TRUE(file.Write(int16_t {0x0048}));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC347)}));
    EXPECT_TRUE(file.Write(int16_t {0x005A}));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC340)}));
    EXPECT_TRUE(file.Write(int16_t {0x0089}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xF840)}));
    EXPECT_TRUE(file.Write(int16_t {0x406B}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x4800}));
    EXPECT_TRUE(file.Write(int16_t {0x461C}));
    EXPECT_TRUE(file.Write(int16_t {0x5A00}));
    EXPECT_TRUE(file.Write(int16_t {0x47C3}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0x8900)}));
    EXPECT_TRUE(file.Write(int16_t {0x40C3}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x6B40}));
    EXPECT_TRUE(file.Write(int16_t {0x40F8}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0x9C48)}));
    EXPECT_TRUE(file.Write(int16_t {0x400C}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC35A)}));
    EXPECT_TRUE(file.Write(int16_t {0x400F}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0C40}));
    EXPECT_TRUE(file.Write(int16_t {0x489C}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0F40}));
    EXPECT_TRUE(file.Write(int16_t {0x5AC3}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));
    EXPECT_TRUE(file.Write(int16_t {0x0000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int16LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int16_t {0x1C46}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0048}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC347)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x005A}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC340)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0089}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xF840)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x406B}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x4800}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x461C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x5A00}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x47C3}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0x8900)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x40C3}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x6B40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x40F8}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0x9C48)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x400C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC35A)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x400F}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0C40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x489C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0F40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x5AC3}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int16BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int16_t {0x461C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x4800}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x47C3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x5A00}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x40C3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0x8900)}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x40F8}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x6B40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0048}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x1C46}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x005A}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC347)}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0089}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC340)}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x406B}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xF840)}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x489C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0C40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x5AC3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0F40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x400C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0x9C48)}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x400F}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {static_cast<int16_t>(0xC35A)}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt16ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(uint16_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_UInt16)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint16_t {0x1C46}));
    EXPECT_TRUE(file.Write(uint16_t {0x0048}));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xC347)}));
    EXPECT_TRUE(file.Write(uint16_t {0x005A}));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xC340)}));
    EXPECT_TRUE(file.Write(uint16_t {0x0089}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xF840)}));
    EXPECT_TRUE(file.Write(uint16_t {0x406B}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x4800}));
    EXPECT_TRUE(file.Write(uint16_t {0x461C}));
    EXPECT_TRUE(file.Write(uint16_t {0x5A00}));
    EXPECT_TRUE(file.Write(uint16_t {0x47C3}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0x8900)}));
    EXPECT_TRUE(file.Write(uint16_t {0x40C3}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x6B40}));
    EXPECT_TRUE(file.Write(uint16_t {0x40F8}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0x9C48)}));
    EXPECT_TRUE(file.Write(uint16_t {0x400C}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xC35A)}));
    EXPECT_TRUE(file.Write(uint16_t {0x400F}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0C40}));
    EXPECT_TRUE(file.Write(uint16_t {0x489C}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0F40}));
    EXPECT_TRUE(file.Write(uint16_t {0x5AC3}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt16LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint16_t {0x1C46}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0048}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xC347)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x005A}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xC340)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0089}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xF840)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x406B}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x4800}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x461C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x5A00}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x47C3}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0x8900)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x40C3}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x6B40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x40F8}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0x9C48)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x400C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {static_cast<uint16_t>(0xC35A)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x400F}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0C40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x489C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0F40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x5AC3}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt16BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint16_t {0x461C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x4800}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x47C3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x5A00}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x40C3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x8900}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x40F8}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x6B40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0048}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x1C46}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x005A}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0xC347}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0089}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0xC340}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x406B}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0xF840}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x489C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0C40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x5AC3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0F40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x400C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x9C48}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x400F}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0xC35A}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int32ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(int32_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Int32)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int32_t {0x00481C46}));
    EXPECT_TRUE(file.Write(int32_t {0x005AC347}));
    EXPECT_TRUE(file.Write(int32_t {0x0089C340}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x406BF840}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x461C4800}));
    EXPECT_TRUE(file.Write(int32_t {0x47C35A00}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x40C38900}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x40F86B40}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {static_cast<int32_t>(0x9C480000)}));
    EXPECT_TRUE(file.Write(int32_t {0x0000400C}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {static_cast<int32_t>(0xC35A0000)}));
    EXPECT_TRUE(file.Write(int32_t {0x0000400F}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x489C0C40}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x5AC30F40}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int32LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int32_t {0x00481C46}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x005AC347}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0089C340}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x406BF840}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x461C4800}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x47C35A00}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x40C38900}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x40F86B40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {static_cast<int32_t>(0x9C480000)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0000400C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {static_cast<int32_t>(0xC35A0000)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0000400F}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x489C0C40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x5AC30F40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int32BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int32_t {0x461C4800}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x47C35A00}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x40C38900}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x40F86B40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00481C46}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x005AC347}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0089C340}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x406BF840}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0000489C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0C400000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00005AC3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x0F400000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x400C9C48}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x400FC35A}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt32ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(uint32_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_UInt32)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46}));
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347}));
    EXPECT_TRUE(file.Write(uint32_t {0x0089C340}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x406BF840}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800}));
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x40C38900}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x40F86B40}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x9C480000}));
    EXPECT_TRUE(file.Write(uint32_t {0x0000400C}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0xC35A0000}));
    EXPECT_TRUE(file.Write(uint32_t {0x0000400F}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x489C0C40}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x5AC30F40}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt32LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0089C340}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x406BF840}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x40C38900}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x40F86B40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x9C480000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0000400C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0xC35A0000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0000400F}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x489C0C40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x5AC30F40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt32BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x40C38900}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x40F86B40}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0089C340}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x406BF840}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0000489C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0C400000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00005AC3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x0F400000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x400C9C48}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x400FC35A}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int64ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(int64_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Int64)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int64_t {0x005AC34700481C46}));
    EXPECT_TRUE(file.Write(int64_t {0x000000000089C340}));
    EXPECT_TRUE(file.Write(int64_t {0x00000000406BF840}));
    EXPECT_TRUE(file.Write(int64_t {0x47C35A00461C4800}));
    EXPECT_TRUE(file.Write(int64_t {0x40C3890000000000}));
    EXPECT_TRUE(file.Write(int64_t {0x40F86B4000000000}));
    EXPECT_TRUE(file.Write(int64_t {static_cast<int64_t>(0x9C48000000000000)}));
    EXPECT_TRUE(file.Write(int64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(int64_t {static_cast<int64_t>(0xC35A000000000000)}));
    EXPECT_TRUE(file.Write(int64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(int64_t {0x00000000489C0C40}));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(int64_t {0x000000005AC30F40}));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int64LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int64_t {0x005AC34700481C46}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x000000000089C340}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x00000000406BF840}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x47C35A00461C4800}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x40C3890000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x40F86B4000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {static_cast<int64_t>(0x9C48000000000000)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x000000000000400C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {static_cast<int64_t>(0xC35A000000000000)}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x000000000000400F}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x00000000489C0C40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x000000005AC30F40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_Int64BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(int64_t {0x461C480047C35A00}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x40C3890000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x40F86B4000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x00481C46005AC347}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x000000000089C340}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x00000000406BF840}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x000000000000489C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0C40000000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000005AC3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0F40000000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x400C9C4800000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x400FC35A00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(int64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt64ClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(uint64_t { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_UInt64)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint64_t {0x005AC34700481C46}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840}));
    EXPECT_TRUE(file.Write(uint64_t {0x47C35A00461C4800}));
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40}));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40}));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt64LittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint64_t {0x005AC34700481C46}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x47C35A00461C4800}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_UInt64BigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint64_t {0x461C480047C35A00}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x00481C46005AC347}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000489C}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0C40000000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000005AC3}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0F40000000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x400C9C4800000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x400FC35A00000000}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_FloatClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(float { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Float)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(float {1.0002E4}));
    EXPECT_TRUE(file.Write(float {1.0002E5}));
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_FloatLittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(float {1.0002E4}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(float {1.0002E5}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_FloatBigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(float {1.0002E4}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(float {1.0002E5}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_DoubleClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(double { 0x00 }));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_Double)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(double {1.0002E4}));
    EXPECT_TRUE(file.Write(double {1.0002E5}));
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_DoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(double {1.0002E4}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(double {1.0002E5}, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_DoubleBigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(double {1.0002E4}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(double {1.0002E5}, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_LongDoubleClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(0.0L));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_LongDouble)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(1.0002E4L));
    EXPECT_TRUE(file.Write(1.0002E5L));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_LongDoubleLittleEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(1.0002E4L, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(1.0002E5L, OSAL::Endianness::LittleEndian));
    EXPECT_TRUE(file.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));
    EXPECT_TRUE(file.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x0000000000000000}));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_LongDoubleBigEndian)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
    EXPECT_TRUE(file.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
    EXPECT_TRUE(file.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
    EXPECT_TRUE(file.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
    EXPECT_TRUE(file.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400C}));
    EXPECT_TRUE(file.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
    EXPECT_TRUE(file.Write(uint64_t {0x000000000000400F}));
    EXPECT_TRUE(file.Write(1.0002E4L, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(file.Write(1.0002E5L, OSAL::Endianness::BigEndian));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

TEST_FIXTURE(BinaryFileTest, Write_StringClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    string value("Hello");
    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_String)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    string value("Hello\nHere I am\n\n");
    EXPECT_TRUE(file.Exists());
    EXPECT_TRUE(file.Write(value));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFilePath()));
}

TEST_FIXTURE(BinaryFileTest, Write_ByteArrayClosedFile)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path);

    OSAL::ByteArray value({123});
    EXPECT_FALSE(file.Exists());
    EXPECT_FALSE(file.Write(value));
    EXPECT_FALSE(file.Exists());
}

TEST_FIXTURE(BinaryFileTest, Write_ByteArray)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);

    EXPECT_TRUE(file.Exists());
    OSAL::ByteArray value({ 0x46, 0x1C, 0x48, 0x00, 0x47, 0xC3, 0x5A, 0x00,
                            0x40, 0xC3, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x40, 0xF8, 0x6B, 0x40, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x48, 0x1C, 0x46, 0x00, 0x5A, 0xC3, 0x47,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0xC3, 0x40,
                            0x00, 0x00, 0x00, 0x00, 0x40, 0x6B, 0xF8, 0x40,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x9C,
                            0x0C, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xC3,
                            0x0F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x40, 0x0C, 0x9C, 0x48, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x40, 0x0F, 0xC3, 0x5A, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
    EXPECT_TRUE(file.Write(value));

    file.Close();
    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFile5Path()));
}

} // namespace Test
} // namespace Files
} // namespace Core
