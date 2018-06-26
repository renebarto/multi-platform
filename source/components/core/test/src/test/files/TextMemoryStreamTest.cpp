#include <unittest-cpp/UnitTestC++.h>

#include "core/files/TextMemoryStream.h"
#include "core/TestData.h"

using namespace std;

namespace Core {
namespace Files {
namespace Test {

class TextMemoryStreamTest : public UnitTestCpp::TestFixture
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

static const std::string Text = "Hello\nHere I am\n\n";
static const std::string NumString = "123 234 12345 34567 12345678 34567890 9223372036854775807 18446744073709551615 1.0002e4 1.00002E05 1.00002E05 abc def\n";
static const std::string HexStringWithLF = "00 01 02 03 04 05 06 07 F8 F9 Fa fB FC fd fe ff\n";
static const std::string HexString = "00 01 02 03 04 05 06 07 F8 F9 Fa fB FC fd fe ff";

TEST_FIXTURE(TextMemoryStreamTest, ConstructDefault)
{
    TextMemoryStream stream;

    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Construct)
{
    TextMemoryStream stream(Text);

    EXPECT_EQ(Text, stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, GetSetContents)
{
    TextMemoryStream stream;
    stream.SetContents(Text);

    EXPECT_EQ(Text, stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, CompareTo)
{
    TextMemoryStream src(Text);
    TextMemoryStream dst(Text);

    EXPECT_TRUE(src.CompareTo(dst));
}

TEST_FIXTURE(TextMemoryStreamTest, ReadCharEmptyStream)
{
    TextMemoryStream stream;

    EXPECT_EQ('\0', stream.ReadChar());
}

TEST_FIXTURE(TextMemoryStreamTest, ReadChar)
{
    TextMemoryStream stream(Text);

    EXPECT_EQ('H', stream.ReadChar());
    EXPECT_EQ('e', stream.ReadChar());
    EXPECT_EQ('l', stream.ReadChar());
    EXPECT_EQ('l', stream.ReadChar());
    EXPECT_EQ('o', stream.ReadChar());
    EXPECT_EQ('\n', stream.ReadChar());
    EXPECT_EQ('H', stream.ReadChar());
    EXPECT_EQ('e', stream.ReadChar());
    EXPECT_EQ('r', stream.ReadChar());
    EXPECT_EQ('e', stream.ReadChar());
    EXPECT_EQ(' ', stream.ReadChar());
    EXPECT_EQ('I', stream.ReadChar());
    EXPECT_EQ(' ', stream.ReadChar());
    EXPECT_EQ('a', stream.ReadChar());
    EXPECT_EQ('m', stream.ReadChar());
    EXPECT_EQ('\n', stream.ReadChar());
    EXPECT_EQ('\n', stream.ReadChar());
    EXPECT_EQ('\0', stream.ReadChar());
}

TEST_FIXTURE(TextMemoryStreamTest, ReadLineEmptyStream)
{
    TextMemoryStream stream;

    EXPECT_EQ("", stream.ReadLine());
}

TEST_FIXTURE(TextMemoryStreamTest, ReadLine)
{
    TextMemoryStream stream(Text);

    EXPECT_EQ("Hello", stream.ReadLine());
    EXPECT_EQ("Here I am", stream.ReadLine());
    EXPECT_EQ("", stream.ReadLine());
}

TEST_FIXTURE(TextMemoryStreamTest, ReadLineWithBoolResultEmptyStream)
{
    TextMemoryStream stream;

    std::string line;
    EXPECT_FALSE(stream.ReadLine(line));
    EXPECT_EQ("", line);
}

TEST_FIXTURE(TextMemoryStreamTest, ReadLineWithBoolResult)
{
    TextMemoryStream stream(Text);

    std::string line;
    EXPECT_TRUE(stream.ReadLine(line));
    EXPECT_EQ("Hello", line);
    EXPECT_TRUE(stream.ReadLine(line));
    EXPECT_EQ("Here I am", line);
    EXPECT_TRUE(stream.ReadLine(line));
    EXPECT_EQ("", line);
    EXPECT_FALSE(stream.ReadLine(line));
    EXPECT_EQ("", line);
}

TEST_FIXTURE(TextMemoryStreamTest, WriteLineEmptyStream)
{
    TextMemoryStream stream;

    EXPECT_TRUE(stream.WriteLine("X"));
    stream.Flush();
    EXPECT_EQ("X\n", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, WriteLine)
{
    TextMemoryStream stream;

    EXPECT_TRUE(stream.WriteLine("Hello"));
    EXPECT_TRUE(stream.WriteLine("Here I am"));
    EXPECT_TRUE(stream.WriteLine(""));
    stream.Flush();
    EXPECT_EQ(Text, stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, ReadString)
{
    TextMemoryStream stream(Text);

    string value;
    stream.ReadString(value, " \n");
    EXPECT_EQ("Hello", value);
    stream.ReadString(value, " \n");
    EXPECT_EQ("Here", value);
    stream.ReadString(value, " \n");
    EXPECT_EQ("I", value);
    stream.ReadString(value, " \n");
    EXPECT_EQ("am", value);
    stream.ReadString(value, " \n");
    EXPECT_EQ("", value);
}

TEST_FIXTURE(TextMemoryStreamTest, ReadAll)
{
    TextMemoryStream stream(Text);

    string value;
    stream.ReadAll(value);
    EXPECT_EQ("Hello\nHere I am\n\n", value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_CharEmptyStream)
{
    TextMemoryStream stream;

    char ch {};
    EXPECT_FALSE(stream.Read(ch));
    EXPECT_EQ('\0', ch);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Char)
{
    TextMemoryStream stream(Text);

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

TEST_FIXTURE(TextMemoryStreamTest, Read_Int8EmptyStream)
{
    TextMemoryStream stream;

    int8_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(int8_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int8)
{
    TextMemoryStream stream(NumString);

    int8_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int8_t {123}, value);
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(int8_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt8EmptyStream)
{
    TextMemoryStream stream;

    uint8_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(uint8_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt8)
{
    TextMemoryStream stream(NumString);

    uint8_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint8_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint8_t {234}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int16EmptyStream)
{
    TextMemoryStream stream;

    int16_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(int16_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int16)
{
    TextMemoryStream stream(NumString);

    int16_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int16_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int16_t {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int16_t {12345}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int16LittleEndian)
{
    TextMemoryStream stream(NumString);

    int16_t value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int16_t {12345}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int16BigEndian)
{
    TextMemoryStream stream(NumString);

    int16_t value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int16_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt16EmptyStream)
{
    TextMemoryStream stream;

    uint16_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(uint16_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt16)
{
    TextMemoryStream stream(NumString);

    uint16_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint16_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint16_t {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint16_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint16_t {34567}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt16LittleEndian)
{
    TextMemoryStream stream(NumString);

    uint16_t value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {12345}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint16_t {34567}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt16BigEndian)
{
    TextMemoryStream stream(NumString);

    uint16_t value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint16_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int32EmptyStream)
{
    TextMemoryStream stream;

    int32_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(int32_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int32)
{
    TextMemoryStream stream(NumString);

    int32_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {12345678}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int32LittleEndian)
{
    TextMemoryStream stream(NumString);

    int32_t value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int32_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int32_t {12345678}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int32BigEndian)
{
    TextMemoryStream stream(NumString);

    int32_t value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int32_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt32EmptyStream)
{
    TextMemoryStream stream;

    uint32_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(uint32_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt32)
{
    TextMemoryStream stream(NumString);

    uint32_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {34567890}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt32LittleEndian)
{
    TextMemoryStream stream(NumString);

    uint32_t value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {12345}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint32_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint32_t {34567890}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt32BigEndian)
{
    TextMemoryStream stream(NumString);

    uint32_t value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint32_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int64EmptyStream)
{
    TextMemoryStream stream;

    int64_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(int64_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int64)
{
    TextMemoryStream stream(NumString);

    int64_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {34567890}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {9223372036854775807LL}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int64LittleEndian)
{
    TextMemoryStream stream(NumString);

    int64_t value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(int64_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {34567890}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(int64_t {9223372036854775807LL}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Int64BigEndian)
{
    TextMemoryStream stream(NumString);

    int64_t value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(int64_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt64EmptyStream)
{
    TextMemoryStream stream;

    uint64_t value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(uint64_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt64)
{
    TextMemoryStream stream(NumString);

    uint64_t value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {34567890}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {9223372036854775807LL}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {18446744073709551615ULL}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt64LittleEndian)
{
    TextMemoryStream stream(NumString);

    uint64_t value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {12345}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(uint64_t {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {34567890}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {9223372036854775807LL}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(uint64_t {18446744073709551615ULL}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_UInt64BigEndian)
{
    TextMemoryStream stream(NumString);

    uint64_t value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(uint64_t {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_FloatEmptyStream)
{
    TextMemoryStream stream;

    float value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(float {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Float)
{
    TextMemoryStream stream(NumString);

    float value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {3.4567890E7F}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {9.223372036854775807E18F}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {1.8446744073709551615E19F}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {1.0002e4F}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_FloatLittleEndian)
{
    TextMemoryStream stream(NumString);

    float value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(float {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {3.4567890E7F}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {9.223372036854775807E18F}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {1.8446744073709551615E19F}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(float {1.0002e4F}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_FloatBigEndian)
{
    TextMemoryStream stream(NumString);

    float value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(float {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_DoubleEmptyStream)
{
    TextMemoryStream stream;

    double value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_EQ(double {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_Double)
{
    TextMemoryStream stream(NumString);

    double value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {123}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {234}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {34567890}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_NEAR(double {9.223372036854775807E18}, value, 5E5);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_NEAR(double {1.8446744073709551615E19}, value, 5E5);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {1.0002e4}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {1.00002E05}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_DoubleLittleEndian)
{
    TextMemoryStream stream(NumString);

    double value {};
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {123}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {234}, value);
    EXPECT_TRUE(stream.Read(value, OSAL::Endianness::LittleEndian));
    EXPECT_EQ(double {12345}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {34567}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {12345678}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {34567890}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_NEAR(double {9.223372036854775807E18}, value, 5E5);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_NEAR(double {1.8446744073709551615E19}, value, 5E5);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {1.0002e4}, value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_EQ(double {1.00002E05}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_DoubleBigEndian)
{
    TextMemoryStream stream(NumString);

    double value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_EQ(double {0}, value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_LongDoubleEmptyStream)
{
    TextMemoryStream stream;

    long double value {};
    EXPECT_FALSE(stream.Read(value));
    EXPECT_TRUE(0.0L == value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_LongDouble)
{
    TextMemoryStream stream(NumString);

    long double value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(123L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(234L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(12345L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(34567L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(12345678L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(34567890L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(Util::Compare(9.223372036854775807E18L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(Util::Compare(1.8446744073709551615E19L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(1.0002e4L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_LongDoubleLittleEndian)
{
    TextMemoryStream stream(NumString);

    long double value {};
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(123L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(234L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(12345L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(34567L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(12345678L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(34567890L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(Util::Compare(9.223372036854775807E18L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(Util::Compare(1.8446744073709551615E19L, value, 0.5L, 1E-16L));
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(1.0002e4L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
    EXPECT_TRUE(stream.Read(value));
    EXPECT_TRUE(1.00002E05L == value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_LongDoubleBigEndian)
{
    TextMemoryStream stream(NumString);

    long double value {};
    EXPECT_FALSE(stream.Read(value, OSAL::Endianness::BigEndian));
    EXPECT_TRUE(0.0L == value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_StringEmptyStream)
{
    TextMemoryStream stream;

    string value;
    EXPECT_FALSE(stream.Read(value, 1));
    EXPECT_EQ("", value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_String)
{
    TextMemoryStream stream(NumString);

    string value;
    EXPECT_TRUE(stream.Read(value, 118));
    EXPECT_EQ("123 234 12345 34567 12345678 34567890 9223372036854775807 18446744073709551615 1.0002e4 1.00002E05 1.00002E05 abc def\n", value);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_ByteArrayEmptyStream)
{
    TextMemoryStream stream;

    OSAL::ByteArray value;
    EXPECT_FALSE(stream.Read(value, 1));
    EXPECT_EQ(size_t {0}, value.Size());
}

TEST_FIXTURE(TextMemoryStreamTest, Read_ByteArray)
{
    TextMemoryStream stream(HexStringWithLF);

    OSAL::ByteArray expected({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF });
    OSAL::ByteArray actual;
    EXPECT_TRUE(stream.Read(actual, 16));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TextMemoryStreamTest, Read_ByteArrayUpToEOS)
{
    TextMemoryStream stream(HexString);

    OSAL::ByteArray expected({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF });
    OSAL::ByteArray actual;
    EXPECT_TRUE(stream.Read(actual, 16));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(TextMemoryStreamTest, Write_CharEmptyStream)
{
    TextMemoryStream stream;

    EXPECT_TRUE(stream.Write('X'));
    stream.Flush();
    EXPECT_EQ("X", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Char)
{
    TextMemoryStream stream;

    EXPECT_TRUE(stream.Write('H'));
    EXPECT_TRUE(stream.Write('e'));
    EXPECT_TRUE(stream.Write('l'));
    EXPECT_TRUE(stream.Write('l'));
    EXPECT_TRUE(stream.Write('o'));
    EXPECT_TRUE(stream.Write('\n'));
    EXPECT_TRUE(stream.Write('H'));
    EXPECT_TRUE(stream.Write('e'));
    EXPECT_TRUE(stream.Write('r'));
    EXPECT_TRUE(stream.Write('e'));
    EXPECT_TRUE(stream.Write(' '));
    EXPECT_TRUE(stream.Write('I'));
    EXPECT_TRUE(stream.Write(' '));
    EXPECT_TRUE(stream.Write('a'));
    EXPECT_TRUE(stream.Write('m'));
    EXPECT_TRUE(stream.Write('\n'));
    EXPECT_TRUE(stream.Write('\n'));
    stream.Flush();
    EXPECT_EQ(Text, stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int8EmptyStream)
{
    TextMemoryStream stream;

    int8_t value {123};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("123", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int8)
{
    TextMemoryStream stream;

    int8_t value {123};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("123", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt8EmptyStream)
{
    TextMemoryStream stream;

    uint8_t value {234};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("234", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt8)
{
    TextMemoryStream stream;

    uint8_t value {234};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("234", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int16EmptyStream)
{
    TextMemoryStream stream;

    int16_t value {12345};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("12345", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int16)
{
    TextMemoryStream stream;

    int16_t value {12345};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("12345", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int16LittleEndian)
{
    TextMemoryStream stream;

    int16_t value {12345};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("12345", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int16BigEndian)
{
    TextMemoryStream stream;

    int16_t value {12345};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt16EmptyStream)
{
    TextMemoryStream stream;

    uint16_t value {34567};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("34567", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt16)
{
    TextMemoryStream stream;

    uint16_t value {34567};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("34567", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt16LittleEndian)
{
    TextMemoryStream stream;

    uint16_t value {34567};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("34567", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt16BigEndian)
{
    TextMemoryStream stream;

    uint16_t value {34567};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int32EmptyStream)
{
    TextMemoryStream stream;

    int32_t value {12345678};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("12345678", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int32)
{
    TextMemoryStream stream;

    int32_t value {12345678};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("12345678", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int32LittleEndian)
{
    TextMemoryStream stream;

    int32_t value {12345678};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("12345678", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int32BigEndian)
{
    TextMemoryStream stream;

    int32_t value {12345678};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt32EmptyStream)
{
    TextMemoryStream stream;

    uint32_t value {34567890};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("34567890", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt32)
{
    TextMemoryStream stream;

    uint32_t value {34567890};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("34567890", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt32LittleEndian)
{
    TextMemoryStream stream;

    uint32_t value {34567890};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("34567890", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt32BigEndian)
{
    TextMemoryStream stream;

    uint32_t value {34567890};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int64EmptyStream)
{
    TextMemoryStream stream;

    int64_t value {9223372036854775807LL};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("9223372036854775807", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int64)
{
    TextMemoryStream stream;

    int64_t value {9223372036854775807LL};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("9223372036854775807", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int64LittleEndian)
{
    TextMemoryStream stream;

    int64_t value {9223372036854775807LL};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("9223372036854775807", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Int64BigEndian)
{
    TextMemoryStream stream;

    int64_t value {9223372036854775807LL};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt64EmptyStream)
{
    TextMemoryStream stream;

    uint64_t value {18446744073709551615ULL};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("18446744073709551615", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt64)
{
    TextMemoryStream stream;

    uint64_t value {18446744073709551615ULL};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("18446744073709551615", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt64LittleEndian)
{
    TextMemoryStream stream;

    uint64_t value {18446744073709551615ULL};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("18446744073709551615", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_UInt64BigEndian)
{
    TextMemoryStream stream;

    uint64_t value {18446744073709551615ULL};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_FloatEmptyStream)
{
    string path = Core::Test::Data::RegularFileNonExistingPath();

    TextMemoryStream stream;

    float value {1.0002e4F};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("10002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Float)
{
    TextMemoryStream stream;

    float value {1.0002e4F};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("10002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_FloatLittleEndian)
{
    TextMemoryStream stream;

    float value {1.0002e4F};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("10002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_FloatBigEndian)
{
    TextMemoryStream stream;

    float value {1.0002e4F};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_DoubleEmptyStream)
{
    TextMemoryStream stream;

    double value {1.00002e5};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("100002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_Double)
{
    TextMemoryStream stream;

    double value {1.00002E05};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("100002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_DoubleLittleEndian)
{
    TextMemoryStream stream;

    double value {1.00002E05};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("100002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_DoubleBigEndian)
{
    TextMemoryStream stream;

    double value {1.00002E05};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_LongDoubleEmptyStream)
{
    TextMemoryStream stream;

    long double value {1.00002e5};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("100002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_LongDouble)
{
    TextMemoryStream stream;

    long double value {1.00002E05L};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("100002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_LongDoubleLittleEndian)
{
    TextMemoryStream stream;

    long double value {1.00002E05L};
    EXPECT_TRUE(stream.Write(value, OSAL::Endianness::LittleEndian));
    stream.Flush();
    EXPECT_EQ("100002", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_LongDoubleBigEndian)
{
    TextMemoryStream stream;

    long double value {1.00002E05L};
    EXPECT_FALSE(stream.Write(value, OSAL::Endianness::BigEndian));
    stream.Flush();
    EXPECT_EQ("", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_StringEmptyStream)
{
    TextMemoryStream stream;

    string value {"123"};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("123", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_String)
{
    TextMemoryStream stream;

    string value {"abcdef"};
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("abcdef", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_ByteArrayEmptyStream)
{
    TextMemoryStream stream;

    OSAL::ByteArray value({123});
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("7B", stream.GetContents());
}

TEST_FIXTURE(TextMemoryStreamTest, Write_ByteArray)
{
    TextMemoryStream stream;

    OSAL::ByteArray value({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF });
    EXPECT_TRUE(stream.Write(value));
    stream.Flush();
    EXPECT_EQ("00 01 02 03 04 05 06 07 F8 F9 FA FB FC FD FE FF", stream.GetContents());
}

} // namespace Test
} // namespace Files
} // namespace Core
