#include <unittest-cpp/UnitTestC++.h>

#include "core/files/BinaryMemoryStream.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class BinaryMemoryStreamTest : public UnitTestCpp::TestFixture
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

static const OSAL::ByteArray Text({ 'H', 'e', 'l', 'l', 'o', '\n', 'H', 'e', 'r', 'e', ' ', 'I', ' ', 'a', 'm', '\n', '\n'});
static const OSAL::ByteArray Data(
    { 0x46, 0x1C, 0x48, 0x00, 0x47, 0xC3, 0x5A, 0x00,
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

TEST_FIXTURE(BinaryMemoryStreamTest, ConstructDefault)
{
    BinaryMemoryStream stream;

    OSAL::ByteArray expected;
    EXPECT_EQ(expected, stream.GetContents());
}

TEST_FIXTURE(BinaryMemoryStreamTest, Construct)
{
    BinaryMemoryStream stream(Data);

    OSAL::ByteArray expected(Data);
    EXPECT_EQ(expected, stream.GetContents());
}

TEST_FIXTURE(BinaryMemoryStreamTest, GetSetContents)
{
    BinaryMemoryStream stream;
    stream.SetContents(Data);

    EXPECT_EQ(Data, stream.GetContents());
}

TEST_FIXTURE(BinaryMemoryStreamTest, CompareTo)
{
    BinaryMemoryStream src(Data);
    BinaryMemoryStream dst(Data);

    EXPECT_TRUE(src.CompareTo(dst));
}

TEST_FIXTURE(BinaryMemoryStreamTest, Read_CharEmptyStream)
{
    BinaryMemoryStream stream;

    char ch {};
    EXPECT_FALSE(stream.Read(ch));
    EXPECT_EQ('\0', ch);
}

TEST_FIXTURE(BinaryMemoryStreamTest, Read_Char)
{
    BinaryMemoryStream stream(Text);

    char ch {};
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('H', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('l', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('l', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('o', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('H', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('r', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('e', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ(' ', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('I', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ(' ', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('a', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('m', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_TRUE(stream.Read(ch));
    EXPECT_EQ('\n', ch);
    EXPECT_FALSE(stream.Read(ch));
    EXPECT_EQ('\0', ch);
}

TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int8EmptyStream)
{
    BinaryMemoryStream stream;

    int8_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(int8_t {0}, value);
}

TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int8)
{
    BinaryMemoryStream stream(Data);

    int8_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int8_t {0x46}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int8_t {0x1C}, value);
}

TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt8EmptyStream)
{
    BinaryMemoryStream stream;

    uint8_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(uint8_t {0}, value);
}

TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt8)
{
    BinaryMemoryStream stream(Data);

    uint8_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint8_t {0x46}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint8_t {0x1C}, value);
}

//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int16EmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    int16_t value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(int16_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int16)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int16_t value {};
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(int16_t {0x1C46}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(int16_t {0x0048}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int16LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int16_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int16_t {0x1C46}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int16_t {0x0048}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int16BigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int16_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int16_t {0x461C}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int16_t {0x4800}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt16EmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    uint16_t value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(uint16_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt16)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint16_t value {};
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(uint16_t {0x1C46}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(uint16_t {0x0048}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt16LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint16_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint16_t {0x1C46}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint16_t {0x0048}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt16BigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint16_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint16_t {0x461C}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint16_t {0x4800}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int32EmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    int32_t value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(int32_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int32)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int32_t value {};
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(int32_t {0x00481C46}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(int32_t {0x005AC347}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int32LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int32_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int32_t {0x00481C46}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int32_t {0x005AC347}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int32BigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int32_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int32_t {0x461C4800}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int32_t {0x47C35A00}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt32EmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    uint32_t value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(uint32_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt32)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint32_t value {};
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(uint32_t {0x00481C46}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(uint32_t {0x005AC347}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt32LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint32_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint32_t {0x00481C46}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint32_t {0x005AC347}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt32BigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint32_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint32_t {0x461C4800}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint32_t {0x47C35A00}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int64EmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    int64_t value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(int64_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int64)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int64_t value {};
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(int64_t {0x005AC34700481C46}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(int64_t {0x000000000089C340}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int64LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int64_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int64_t {0x005AC34700481C46}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(int64_t {0x000000000089C340}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Int64BigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    int64_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int64_t {0x461C480047C35A00}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(int64_t {0x40C3890000000000}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt64EmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    uint64_t value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(uint64_t {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt64)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint64_t value {};
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(uint64_t {0x005AC34700481C46}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(uint64_t {0x000000000089C340}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt64LittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint64_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint64_t {0x005AC34700481C46}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(uint64_t {0x000000000089C340}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_UInt64BigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    uint64_t value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint64_t {0x461C480047C35A00}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(uint64_t {0x40C3890000000000}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_FloatEmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    float value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(float {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Float)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    float value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(float {1.0002E4}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(float {1.0002E5}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_FloatLittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    float value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(float {1.0002E4}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(float {1.0002E5}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_FloatBigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    float value {};
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(float {1.0002E4}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(float {1.0002E5}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_DoubleEmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    double value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_EQ(double {0}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_Double)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    double value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(double {1.0002E4}, value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_EQ(double {1.0002E5}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_DoubleLittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    double value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(double {1.0002E4}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_EQ(double {1.0002E5}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_DoubleBigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    double value {};
//    float dummyFloat {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(double {1.0002E4}, value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_EQ(double {1.0002E5}, value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_LongDoubleEmptyStream)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_TRUE(stream.Exists());
//    long double value {};
//    EXPECT_FALSE(stream.Read(value));
//    EXPECT_TRUE(0.0L == value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_LongDouble)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(path, DesiredAccess::ReadWrite);
//
//    EXPECT_TRUE(stream.Exists());
//    long double value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_TRUE(1.0002E4L == value);
//    EXPECT_TRUE(stream.Read(value));
//    EXPECT_TRUE(1.0002E5L == value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_LongDoubleLittleEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    long double value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(1.0002E4L == value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(1.0002E5L == value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_LongDoubleBigEndian)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    long double value {};
//    float dummyFloat {};
//    double dummyDouble {};
//    long double dummyLongDouble {};
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyFloat)); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Read(dummyDouble)); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Read(dummyLongDouble)); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Read(dummyLongDouble)); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(1.0002E4L == value);
//    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(1.0002E5L == value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_StringEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    string value;
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Read(value, 1));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_String)
//{
//    string path = Core::Test::Data::RegularFilePath();
//
//    BinaryMemoryStream stream(Data);
//
//    string value;
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Read(value, 17));
//    EXPECT_EQ("Hello\nHere I am\n\n", value);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_ByteArrayEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    OSAL::ByteArray value;
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Read(value, 1));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Read_ByteArray)
//{
//    string path = Core::Test::Data::RegularFile5Path();
//
//    BinaryMemoryStream stream(Data);
//
//    EXPECT_TRUE(stream.Exists());
//    OSAL::ByteArray expected({ 0x46, 0x1C, 0x48, 0x00, 0x47, 0xC3, 0x5A, 0x00,
//                               0x40, 0xC3, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00,
//                               0x40, 0xF8, 0x6B, 0x40, 0x00, 0x00, 0x00, 0x00,
//                               0x00, 0x48, 0x1C, 0x46, 0x00, 0x5A, 0xC3, 0x47,
//                               0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0xC3, 0x40,
//                               0x00, 0x00, 0x00, 0x00, 0x40, 0x6B, 0xF8, 0x40,
//                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x9C,
//                               0x0C, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xC3,
//                               0x0F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                               0x40, 0x0C, 0x9C, 0x48, 0x00, 0x00, 0x00, 0x00,
//                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                               0x40, 0x0F, 0xC3, 0x5A, 0x00, 0x00, 0x00, 0x00,
//                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
//    OSAL::ByteArray actual;
//    EXPECT_TRUE(stream.Read(actual, 112));
//    EXPECT_EQ(expected, actual);
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_CharEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write('X'));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Char)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write('H'));
//    EXPECT_TRUE(stream.Write('e'));
//    EXPECT_TRUE(stream.Write('l'));
//    EXPECT_TRUE(stream.Write('l'));
//    EXPECT_TRUE(stream.Write('o'));
//    EXPECT_TRUE(stream.Write('\n'));
//    EXPECT_TRUE(stream.Write('H'));
//    EXPECT_TRUE(stream.Write('e'));
//    EXPECT_TRUE(stream.Write('r'));
//    EXPECT_TRUE(stream.Write('e'));
//    EXPECT_TRUE(stream.Write(' '));
//    EXPECT_TRUE(stream.Write('I'));
//    EXPECT_TRUE(stream.Write(' '));
//    EXPECT_TRUE(stream.Write('a'));
//    EXPECT_TRUE(stream.Write('m'));
//    EXPECT_TRUE(stream.Write('\n'));
//    EXPECT_TRUE(stream.Write('\n'));
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFilePath()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int8EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(int8_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int8)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int8_t {0x46}));
//    EXPECT_TRUE(stream.Write(int8_t {0x1C}));
//    EXPECT_TRUE(stream.Write(int8_t {0x48}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x47}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xC3)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xC3)}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0x89)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xF8)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x6B}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x48}));
//    EXPECT_TRUE(stream.Write(int8_t {0x1C}));
//    EXPECT_TRUE(stream.Write(int8_t {0x46}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xC3)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x47}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0x89)}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xC3)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x6B}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xF8)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x48}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0x9C)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x0C}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xC3)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x0F}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x0C}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0x9C)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x48}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x40}));
//    EXPECT_TRUE(stream.Write(int8_t {0x0F}));
//    EXPECT_TRUE(stream.Write(int8_t {static_cast<int8_t>(0xC3)}));
//    EXPECT_TRUE(stream.Write(int8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//    EXPECT_TRUE(stream.Write(int8_t {0x00}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt8EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(uint8_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt8)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint8_t {0x46}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x1C}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x48}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x47}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xC3}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xC3}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x89}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xF8}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x6B}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x48}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x1C}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x46}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xC3}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x47}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x89}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xC3}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x6B}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xF8}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x48}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x9C}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x0C}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xC3}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x0F}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x0C}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x9C}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x48}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x40}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x0F}));
//    EXPECT_TRUE(stream.Write(uint8_t {0xC3}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x5A}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//    EXPECT_TRUE(stream.Write(uint8_t {0x00}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int16EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(int16_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int16)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int16_t {0x1C46}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0048}));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC347)}));
//    EXPECT_TRUE(stream.Write(int16_t {0x005A}));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC340)}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0089}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xF840)}));
//    EXPECT_TRUE(stream.Write(int16_t {0x406B}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x4800}));
//    EXPECT_TRUE(stream.Write(int16_t {0x461C}));
//    EXPECT_TRUE(stream.Write(int16_t {0x5A00}));
//    EXPECT_TRUE(stream.Write(int16_t {0x47C3}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0x8900)}));
//    EXPECT_TRUE(stream.Write(int16_t {0x40C3}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x6B40}));
//    EXPECT_TRUE(stream.Write(int16_t {0x40F8}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0x9C48)}));
//    EXPECT_TRUE(stream.Write(int16_t {0x400C}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC35A)}));
//    EXPECT_TRUE(stream.Write(int16_t {0x400F}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0C40}));
//    EXPECT_TRUE(stream.Write(int16_t {0x489C}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0F40}));
//    EXPECT_TRUE(stream.Write(int16_t {0x5AC3}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int16LittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int16_t {0x1C46}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0048}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC347)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x005A}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC340)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0089}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xF840)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x406B}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x4800}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x461C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x5A00}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x47C3}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0x8900)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x40C3}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x6B40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x40F8}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0x9C48)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x400C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC35A)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x400F}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0C40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x489C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0F40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x5AC3}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::LittleEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int16BigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int16_t {0x461C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x4800}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x47C3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x5A00}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x40C3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0x8900)}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x40F8}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x6B40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0048}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x1C46}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x005A}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC347)}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0089}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC340)}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x406B}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xF840)}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x489C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0C40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x5AC3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0F40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x400C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0x9C48)}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x400F}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {static_cast<int16_t>(0xC35A)}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int16_t {0x0000}, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt16EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(uint16_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt16)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint16_t {0x1C46}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0048}));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xC347)}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x005A}));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xC340)}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0089}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xF840)}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x406B}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x4800}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x461C}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x5A00}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x47C3}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0x8900)}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x40C3}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x6B40}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x40F8}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0x9C48)}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x400C}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xC35A)}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x400F}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0C40}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x489C}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0F40}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x5AC3}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt16LittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint16_t {0x1C46}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0048}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xC347)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x005A}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xC340)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0089}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xF840)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x406B}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x4800}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x461C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x5A00}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x47C3}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0x8900)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x40C3}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x6B40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x40F8}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0x9C48)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x400C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {static_cast<uint16_t>(0xC35A)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x400F}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0C40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x489C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0F40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x5AC3}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::LittleEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt16BigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint16_t {0x461C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x4800}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x47C3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x5A00}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x40C3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x8900}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x40F8}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x6B40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0048}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x1C46}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x005A}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0xC347}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0089}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0xC340}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x406B}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0xF840}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x489C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0C40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x5AC3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0F40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x400C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x9C48}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x400F}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0xC35A}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint16_t {0x0000}, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int32EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(int32_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int32)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int32_t {0x00481C46}));
//    EXPECT_TRUE(stream.Write(int32_t {0x005AC347}));
//    EXPECT_TRUE(stream.Write(int32_t {0x0089C340}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x406BF840}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x461C4800}));
//    EXPECT_TRUE(stream.Write(int32_t {0x47C35A00}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x40C38900}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x40F86B40}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {static_cast<int32_t>(0x9C480000)}));
//    EXPECT_TRUE(stream.Write(int32_t {0x0000400C}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {static_cast<int32_t>(0xC35A0000)}));
//    EXPECT_TRUE(stream.Write(int32_t {0x0000400F}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x489C0C40}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x5AC30F40}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int32LittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int32_t {0x00481C46}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x005AC347}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0089C340}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x406BF840}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x461C4800}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x47C35A00}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x40C38900}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x40F86B40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {static_cast<int32_t>(0x9C480000)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0000400C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {static_cast<int32_t>(0xC35A0000)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0000400F}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x489C0C40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x5AC30F40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int32BigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int32_t {0x461C4800}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x47C35A00}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x40C38900}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x40F86B40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00481C46}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x005AC347}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0089C340}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x406BF840}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0000489C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0C400000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00005AC3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x0F400000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x400C9C48}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x400FC35A}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int32_t {0x00000000}, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt32EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(uint32_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt32)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0089C340}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x406BF840}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x40C38900}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x40F86B40}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x9C480000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0000400C}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0xC35A0000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0000400F}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x489C0C40}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x5AC30F40}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt32LittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0089C340}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x406BF840}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x40C38900}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x40F86B40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x9C480000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0000400C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0xC35A0000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0000400F}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x489C0C40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x5AC30F40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::LittleEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt32BigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x40C38900}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x40F86B40}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0089C340}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x406BF840}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0000489C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0C400000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00005AC3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x0F400000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x400C9C48}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x400FC35A}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x00000000}, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int64EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(int64_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int64)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int64_t {0x005AC34700481C46}));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000089C340}));
//    EXPECT_TRUE(stream.Write(int64_t {0x00000000406BF840}));
//    EXPECT_TRUE(stream.Write(int64_t {0x47C35A00461C4800}));
//    EXPECT_TRUE(stream.Write(int64_t {0x40C3890000000000}));
//    EXPECT_TRUE(stream.Write(int64_t {0x40F86B4000000000}));
//    EXPECT_TRUE(stream.Write(int64_t {static_cast<int64_t>(0x9C48000000000000)}));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(int64_t {static_cast<int64_t>(0xC35A000000000000)}));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(int64_t {0x00000000489C0C40}));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000005AC30F40}));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int64LittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int64_t {0x005AC34700481C46}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000089C340}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x00000000406BF840}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x47C35A00461C4800}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x40C3890000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x40F86B4000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {static_cast<int64_t>(0x9C48000000000000)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000000400C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {static_cast<int64_t>(0xC35A000000000000)}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000000400F}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x00000000489C0C40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000005AC30F40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Int64BigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(int64_t {0x461C480047C35A00}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x40C3890000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x40F86B4000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x00481C46005AC347}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000089C340}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x00000000406BF840}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x000000000000489C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0C40000000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000005AC3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0F40000000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x400C9C4800000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x400FC35A00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(int64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt64EmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(uint64_t { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt64)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint64_t {0x005AC34700481C46}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x47C35A00461C4800}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt64LittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint64_t {0x005AC34700481C46}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x47C35A00461C4800}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::LittleEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_UInt64BigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint64_t {0x461C480047C35A00}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00481C46005AC347}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000489C}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0C40000000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000005AC3}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0F40000000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x400C9C4800000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x400FC35A00000000}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_FloatEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(float { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Float)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(float {1.0002E4}));
//    EXPECT_TRUE(stream.Write(float {1.0002E5}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_FloatLittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(float {1.0002E4}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(float {1.0002E5}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_FloatBigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(float {1.0002E4}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(float {1.0002E5}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_DoubleEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(double { 0x00 }));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_Double)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(double {1.0002E4}));
//    EXPECT_TRUE(stream.Write(double {1.0002E5}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_DoubleLittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(double {1.0002E4}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(double {1.0002E5}, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_DoubleBigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(double {1.0002E4}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(double {1.0002E5}, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_LongDoubleEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(0.0L));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_LongDouble)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(1.0002E4L));
//    EXPECT_TRUE(stream.Write(1.0002E5L));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_LongDoubleLittleEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(1.0002E4L, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(1.0002E5L, OSAL::Endianness::LittleEndian));
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000489C0C40})); // Skip first BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000005AC30F40})); // Skip second BigEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x0000000000000000}));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_LongDoubleBigEndian)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(uint32_t {0x00481C46})); // Skip first BigEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x005AC347})); // Skip second BigEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000089C340})); // Skip first BigEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x00000000406BF840})); // Skip second BigEndian double
//    EXPECT_TRUE(stream.Write(uint32_t {0x461C4800})); // Skip first LittleEndian float
//    EXPECT_TRUE(stream.Write(uint32_t {0x47C35A00})); // Skip second LittleEndian float
//    EXPECT_TRUE(stream.Write(uint64_t {0x40C3890000000000})); // Skip first LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x40F86B4000000000})); // Skip second LittleEndian double
//    EXPECT_TRUE(stream.Write(uint64_t {0x9C48000000000000})); // Skip first LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400C}));
//    EXPECT_TRUE(stream.Write(uint64_t {0xC35A000000000000})); // Skip second LittleEndian long double
//    EXPECT_TRUE(stream.Write(uint64_t {0x000000000000400F}));
//    EXPECT_TRUE(stream.Write(1.0002E4L, OSAL::Endianness::BigEndian));
//    EXPECT_TRUE(stream.Write(1.0002E5L, OSAL::Endianness::BigEndian));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_StringEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    string value("Hello");
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(value));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_String)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    string value("Hello\nHere I am\n\n");
//    EXPECT_TRUE(stream.Exists());
//    EXPECT_TRUE(stream.Write(value));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFilePath()));
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_ByteArrayEmptyStream)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream;
//
//    OSAL::ByteArray value({123});
//    EXPECT_FALSE(stream.Exists());
//    EXPECT_FALSE(stream.Write(value));
//    EXPECT_FALSE(stream.Exists());
//}
//
//TEST_FIXTURE(BinaryMemoryStreamTest, Write_ByteArray)
//{
//    string path = Core::Test::Data::RegularFileNonExistingPath();
//
//    BinaryMemoryStream stream(path, DesiredAccess::WriteOnly, ShareMode::ShareReadWrite, CreationFlags::CreateNew);
//
//    EXPECT_TRUE(stream.Exists());
//    OSAL::ByteArray value({ 0x46, 0x1C, 0x48, 0x00, 0x47, 0xC3, 0x5A, 0x00,
//                            0x40, 0xC3, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00,
//                            0x40, 0xF8, 0x6B, 0x40, 0x00, 0x00, 0x00, 0x00,
//                            0x00, 0x48, 0x1C, 0x46, 0x00, 0x5A, 0xC3, 0x47,
//                            0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0xC3, 0x40,
//                            0x00, 0x00, 0x00, 0x00, 0x40, 0x6B, 0xF8, 0x40,
//                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x9C,
//                            0x0C, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xC3,
//                            0x0F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                            0x40, 0x0C, 0x9C, 0x48, 0x00, 0x00, 0x00, 0x00,
//                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                            0x40, 0x0F, 0xC3, 0x5A, 0x00, 0x00, 0x00, 0x00,
//                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
//    EXPECT_TRUE(stream.Write(value));
//
//    stream.Close();
//    EXPECT_TRUE(stream.CompareTo(Core::Test::Data::RegularFile5Path()));
//}

} // namespace Test
} // namespace Files
} // namespace Core
