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

//TEST_FIXTURE(BinaryFileTest, CompareToExistingSourceAndDestinationEqual)
//{
//    string pathSrc = Core::Test::Data::RegularFile2Path();
//    string pathDst = Core::Test::Data::RegularFile3Path();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.CompareTo(pathDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToExistingSourceAndDestinationInequal)
//{
//    string pathSrc = Core::Test::Data::RegularFile2Path();
//    string pathDst = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(pathDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToExistingSourceNonExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToNonExistingSourceExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFilePath();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToFileExistingSourceAndDestinationEqual)
//{
//    string pathSrc = Core::Test::Data::RegularFile2Path();
//    string pathDst = Core::Test::Data::RegularFile3Path();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_TRUE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToFileExistingSourceAndDestinationInequal)
//{
//    string pathSrc = Core::Test::Data::RegularFile2Path();
//    string pathDst = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToFileExistingSourceNonExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_TRUE(fileSrc.Exists());
//    EXPECT_FALSE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, CompareToFileNonExistingSourceExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFilePath();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//    EXPECT_FALSE(fileSrc.CompareTo(fileDst));
//    EXPECT_FALSE(fileSrc.Exists());
//    EXPECT_TRUE(fileDst.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, ComparePathToPathExistingSourceAndDestinationEqual)
//{
//    string pathSrc = Core::Test::Data::RegularFile2Path();
//    string pathDst = Core::Test::Data::RegularFile3Path();
//
//    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
//    EXPECT_TRUE(BinaryFile::Exists(pathDst));
//    EXPECT_TRUE(BinaryFile::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
//    EXPECT_TRUE(BinaryFile::Exists(pathDst));
//}
//
//TEST_FIXTURE(BinaryFileTest, ComparePathToPathExistingSourceAndDestinationInequal)
//{
//    string pathSrc = Core::Test::Data::RegularFile2Path();
//    string pathDst = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
//    EXPECT_TRUE(BinaryFile::Exists(pathDst));
//    EXPECT_FALSE(BinaryFile::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
//    EXPECT_TRUE(BinaryFile::Exists(pathDst));
//}
//
//TEST_FIXTURE(BinaryFileTest, ComparePathToPathExistingSourceNonExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFilePath();
//    string pathDst = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
//    EXPECT_FALSE(BinaryFile::Exists(pathDst));
//    EXPECT_FALSE(BinaryFile::Compare(pathSrc, pathDst));
//    EXPECT_TRUE(BinaryFile::Exists(pathSrc));
//    EXPECT_FALSE(BinaryFile::Exists(pathDst));
//}
//
//TEST_FIXTURE(BinaryFileTest, ComparePathToPathNonExistingSourceExistingDestination)
//{
//    string pathSrc = Core::Test::Data::RegularFileNonExistingPath();
//    string pathDst = Core::Test::Data::RegularFilePath();
//
//    BinaryFile fileSrc(pathSrc);
//    BinaryFile fileDst(pathDst);
//
//    EXPECT_FALSE(BinaryFile::Exists(pathSrc));
//    EXPECT_TRUE(BinaryFile::Exists(pathDst));
//    EXPECT_FALSE(BinaryFile::Compare(pathSrc, pathDst));
//    EXPECT_FALSE(BinaryFile::Exists(pathSrc));
//    EXPECT_TRUE(BinaryFile::Exists(pathDst));
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadCharClosedFile)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    EXPECT_EQ('\0', file.ReadChar());
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadChar)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    EXPECT_EQ('H', file.ReadChar());
//    EXPECT_EQ('e', file.ReadChar());
//    EXPECT_EQ('l', file.ReadChar());
//    EXPECT_EQ('l', file.ReadChar());
//    EXPECT_EQ('o', file.ReadChar());
//    EXPECT_EQ('\n', file.ReadChar());
//    EXPECT_EQ('H', file.ReadChar());
//    EXPECT_EQ('e', file.ReadChar());
//    EXPECT_EQ('r', file.ReadChar());
//    EXPECT_EQ('e', file.ReadChar());
//    EXPECT_EQ(' ', file.ReadChar());
//    EXPECT_EQ('I', file.ReadChar());
//    EXPECT_EQ(' ', file.ReadChar());
//    EXPECT_EQ('a', file.ReadChar());
//    EXPECT_EQ('m', file.ReadChar());
//    EXPECT_EQ('\n', file.ReadChar());
//    EXPECT_EQ('\n', file.ReadChar());
//    EXPECT_EQ('\0', file.ReadChar());
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadLineClosedFile)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    EXPECT_EQ("", file.ReadLine());
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadLine)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    EXPECT_EQ("Hello", file.ReadLine());
//    EXPECT_EQ("Here I am", file.ReadLine());
//    EXPECT_EQ("", file.ReadLine());
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadLineWithBoolResultClosedFile)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    std::string line;
//    EXPECT_FALSE(file.ReadLine(line));
//    EXPECT_EQ("", line);
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadLineWithBoolResult)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    std::string line;
//    EXPECT_TRUE(file.ReadLine(line));
//    EXPECT_EQ("Hello", line);
//    EXPECT_TRUE(file.ReadLine(line));
//    EXPECT_EQ("Here I am", line);
//    EXPECT_TRUE(file.ReadLine(line));
//    EXPECT_EQ("", line);
//    EXPECT_FALSE(file.ReadLine(line));
//    EXPECT_EQ("", line);
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, WriteLineClosedFile)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile file(path);
//
//    EXPECT_FALSE(file.Exists());
//    EXPECT_FALSE(file.WriteLine("X"));
//    EXPECT_FALSE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, WriteLine)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(file.Exists());
//    EXPECT_TRUE(file.WriteLine("Hello"));
//    EXPECT_TRUE(file.WriteLine("Here I am"));
//    EXPECT_TRUE(file.WriteLine(""));
//    file.Close();
//    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFilePath()));
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadString)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    string value;
//    file.ReadString(value, " \n");
//    EXPECT_EQ("Hello", value);
//    file.ReadString(value, " \n");
//    EXPECT_EQ("Here", value);
//    file.ReadString(value, " \n");
//    EXPECT_EQ("I", value);
//    file.ReadString(value, " \n");
//    EXPECT_EQ("am", value);
//    file.ReadString(value, " \n");
//    EXPECT_EQ("", value);
//}
//
//TEST_FIXTURE(BinaryFileTest, ReadAll)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    string value;
//    file.ReadAll(value);
//    EXPECT_EQ("Hello\nHere I am\n\n", value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_CharClosedFile)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    char ch {};
//    EXPECT_FALSE(file.Read(ch));
//    EXPECT_EQ('\0', ch);
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Char)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    char ch;
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('H', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('e', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('l', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('l', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('o', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('\n', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('H', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('e', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('r', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('e', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ(' ', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('I', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ(' ', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('a', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('m', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('\n', ch);
//    EXPECT_TRUE(file.Read(ch));
//    EXPECT_EQ('\n', ch);
//    EXPECT_FALSE(file.Read(ch));
//    EXPECT_EQ('\0', ch);
//    EXPECT_TRUE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int8ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    int8_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(int8_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int8)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int8_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int8_t {123}, value);
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(int8_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt8ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    uint8_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(uint8_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt8)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint8_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint8_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint8_t {234}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int16ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    int16_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(int16_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int16)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int16_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int16_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int16_t {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int16_t {12345}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int16LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int16_t value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int16_t {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int16_t {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int16_t {12345}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int16BigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int16_t value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int16_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt16ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    uint16_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(uint16_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt16)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint16_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint16_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint16_t {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint16_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint16_t {34567}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt16LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint16_t value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint16_t {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint16_t {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint16_t {12345}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint16_t {34567}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt16BigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint16_t value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint16_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int32ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    int32_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(int32_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int32)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int32_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {12345678}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int32LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int32_t value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int32_t {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int32_t {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int32_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int32_t {12345678}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int32BigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int32_t value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int32_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt32ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    uint32_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(uint32_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt32)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint32_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {34567890}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt32LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint32_t value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint32_t {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint32_t {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint32_t {12345}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint32_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint32_t {34567890}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt32BigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint32_t value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint32_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int64ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    int64_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(int64_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int64)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int64_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {34567890}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {9223372036854775807LL}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int64LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int64_t value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int64_t {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int64_t {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int64_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {34567890}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(int64_t {9223372036854775807LL}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Int64BigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    int64_t value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int64_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt64ClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    uint64_t value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(uint64_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt64)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint64_t value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {34567890}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {9223372036854775807LL}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {18446744073709551615ULL}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt64LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint64_t value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint64_t {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint64_t {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint64_t {12345}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint64_t {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {34567890}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {9223372036854775807LL}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(uint64_t {18446744073709551615ULL}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_UInt64BigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    uint64_t value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint64_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_FloatClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    float value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(float {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Float)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    float value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {3.4567890E7F}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {9.223372036854775807E18F}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {1.8446744073709551615E19F}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {1.0002e4F}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_FloatLittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    float value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(float {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(float {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(float {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {3.4567890E7F}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {9.223372036854775807E18F}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {1.8446744073709551615E19F}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(float {1.0002e4F}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_FloatBigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    float value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(float {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_DoubleClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    double value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_EQ(double {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_Double)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    double value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {123}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {234}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {34567890}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_NEAR(double {9.223372036854775807E18}, value, 5E5);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_NEAR(double {1.8446744073709551615E19}, value, 5E5);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {1.0002e4}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {1.00002E05}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_DoubleLittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    double value;
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(double {123}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(double {234}, value);
//    EXPECT_TRUE(file.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(double {12345}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {34567}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {12345678}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {34567890}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_NEAR(double {9.223372036854775807E18}, value, 5E5);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_NEAR(double {1.8446744073709551615E19}, value, 5E5);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {1.0002e4}, value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_EQ(double {1.00002E05}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_DoubleBigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    double value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(double {0}, value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_LongDoubleClosedFile)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path);
//
//    EXPECT_TRUE(file.Exists());
//    long double value {};
//    EXPECT_FALSE(file.Read(value));
//    EXPECT_TRUE(0.0L == value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_LongDouble)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    long double value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(123L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(234L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(12345L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(34567L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(12345678L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(34567890L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(Util::Compare(9.223372036854775807E18L, value, 0.5L, 1E-16L));
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(Util::Compare(1.8446744073709551615E19L, value, 0.5L, 1E-16L));
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(1.0002e4L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(1.00002E05L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(1.00002E05L == value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_LongDoubleLittleEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    long double value;
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(123L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(234L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(12345L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(34567L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(12345678L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(34567890L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(Util::Compare(9.223372036854775807E18L, value, 0.5L, 1E-16L));
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(Util::Compare(1.8446744073709551615E19L, value, 0.5L, 1E-16L));
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(1.0002e4L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(1.00002E05L == value);
//    EXPECT_TRUE(file.Read(value));
//    EXPECT_TRUE(1.00002E05L == value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Read_LongDoubleBigEndian)
//{
//    string path = Core::Test::Data::RegularFile4Path();
//
//    BinaryFile file(path, DesiredAccess::ReadOnly);
//
//    EXPECT_TRUE(file.Exists());
//    long double value;
//    EXPECT_FALSE(file.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(0.0L == value);
//}
//
//TEST_FIXTURE(BinaryFileTest, Write_CharClosedFile)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile file(path);
//
//    EXPECT_FALSE(file.Exists());
//    EXPECT_FALSE(file.Write('X'));
//    EXPECT_FALSE(file.Exists());
//}
//
//TEST_FIXTURE(BinaryFileTest, Write_Char)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryFile file(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(file.Exists());
//    EXPECT_TRUE(file.Write('H'));
//    EXPECT_TRUE(file.Write('e'));
//    EXPECT_TRUE(file.Write('l'));
//    EXPECT_TRUE(file.Write('l'));
//    EXPECT_TRUE(file.Write('o'));
//    EXPECT_TRUE(file.Write('\n'));
//    EXPECT_TRUE(file.Write('H'));
//    EXPECT_TRUE(file.Write('e'));
//    EXPECT_TRUE(file.Write('r'));
//    EXPECT_TRUE(file.Write('e'));
//    EXPECT_TRUE(file.Write(' '));
//    EXPECT_TRUE(file.Write('I'));
//    EXPECT_TRUE(file.Write(' '));
//    EXPECT_TRUE(file.Write('a'));
//    EXPECT_TRUE(file.Write('m'));
//    EXPECT_TRUE(file.Write('\n'));
//    EXPECT_TRUE(file.Write('\n'));
//    file.Close();
//    EXPECT_TRUE(file.CompareTo(Core::Test::Data::RegularFilePath()));
//}
//

} // namespace Test
} // namespace Files
} // namespace Core
