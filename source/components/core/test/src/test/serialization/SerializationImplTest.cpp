#include <unittest-cpp/UnitTestC++.h>
#include "core/serialization/SerializationImpl.h"

using namespace std;

namespace Core
{
namespace Test
{

TEST_SUITE(core)
{

class SerializationImplTest
    : public UnitTestCpp::TestFixture
{
public:
};

TEST_FIXTURE(SerializationImplTest, SerializeBool)
{
    bool value = true;
    EXPECT_EQ("true", Serialize(value));
    value = false;
    EXPECT_EQ("false", Serialize(value));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt8)
{
    int8_t value = 0x55;
    EXPECT_EQ("01010101", Serialize(value, 2));
    EXPECT_EQ("125", Serialize(value, 8));
    EXPECT_EQ("85", Serialize(value, 10));
    EXPECT_EQ("55", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt8)
{
    uint8_t value = 0xAA;
    EXPECT_EQ("10101010", Serialize(value, 2));
    EXPECT_EQ("252", Serialize(value, 8));
    EXPECT_EQ("170", Serialize(value, 10));
    EXPECT_EQ("AA", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt16)
{
    int16_t value = 0x5555;
    EXPECT_EQ("0101010101010101", Serialize(value, 2));
    EXPECT_EQ("052525", Serialize(value, 8));
    EXPECT_EQ("21845", Serialize(value, 10));
    EXPECT_EQ("5555", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt16)
{
    uint16_t value = 0xAAAA;
    EXPECT_EQ("1010101010101010", Serialize(value, 2));
    EXPECT_EQ("125252", Serialize(value, 8));
    EXPECT_EQ("43690", Serialize(value, 10));
    EXPECT_EQ("AAAA", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt32)
{
    int32_t value = 0x55555555;
    EXPECT_EQ("01010101010101010101010101010101", Serialize(value, 2));
    EXPECT_EQ("12525252525", Serialize(value, 8));
    EXPECT_EQ("1431655765", Serialize(value, 10));
    EXPECT_EQ("55555555", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt32)
{
    uint32_t value = 0xAAAAAAAA;
    EXPECT_EQ("10101010101010101010101010101010", Serialize(value, 2));
    EXPECT_EQ("25252525252", Serialize(value, 8));
    EXPECT_EQ("2863311530", Serialize(value, 10));
    EXPECT_EQ("AAAAAAAA", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt64)
{
    int64_t value = 0x5555555555555555;
    EXPECT_EQ("0101010101010101010101010101010101010101010101010101010101010101", Serialize(value, 2));
    EXPECT_EQ("0525252525252525252525", Serialize(value, 8));
    EXPECT_EQ("6148914691236517205", Serialize(value, 10));
    EXPECT_EQ("5555555555555555", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt64)
{
    uint64_t value = 0xAAAAAAAAAAAAAAAA;
    EXPECT_EQ("1010101010101010101010101010101010101010101010101010101010101010", Serialize(value, 2));
    EXPECT_EQ("1252525252525252525252", Serialize(value, 8));
    EXPECT_EQ("12297829382473034410", Serialize(value, 10));
    EXPECT_EQ("AAAAAAAAAAAAAAAA", Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeFloat)
{
    float value = 1.0002e4F;
    EXPECT_EQ("10002", Serialize(value));
    value = 5.0E-37F;
    EXPECT_EQ("5.000000179695649e-37", Serialize(value));
    EXPECT_EQ("5e-37", Serialize(value, 1));
}

TEST_FIXTURE(SerializationImplTest, SerializeDouble)
{
    double value = 1.00002E05;
    EXPECT_EQ("100002", Serialize(value));
    value = 1e-200;
    EXPECT_EQ("1e-200", Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ("1.001e-200", Serialize(value));
    EXPECT_EQ("1e-200", Serialize(value, 2));
}

TEST_FIXTURE(SerializationImplTest, SerializeLongDouble)
{
    long double value = 1.00002E05;
    EXPECT_EQ("100002", Serialize(value));
    value = 1e-200;
    EXPECT_EQ("1e-200", Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ("1.001e-200", Serialize(value));
    EXPECT_EQ("1e-200", Serialize(value, 2));
}

TEST_FIXTURE(SerializationImplTest, SerializeBoolBinary)
{
    bool value = true;
    OSAL::ByteArray expected {0x01};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);

    value = false;
    expected = {0x00};
    actual.Clear();
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeBoolBinaryLittleEndian)
{
    bool value = true;
    OSAL::ByteArray expected {0x01};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);

    value = false;
    expected = {0x00};
    actual.Clear();
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeBoolBinaryBigEndian)
{
    bool value = true;
    OSAL::ByteArray expected {0x01};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);

    value = false;
    expected = {0x00};
    actual.Clear();
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt8Binary)
{
    int8_t value = 0x55;
    OSAL::ByteArray expected {0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt8BinaryLittleEndian)
{
    int8_t value = 0x55;
    OSAL::ByteArray expected {0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt8BinaryBigEndian)
{
    int8_t value = 0x55;
    OSAL::ByteArray expected {0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt8Binary)
{
    uint8_t value = 0xAA;
    OSAL::ByteArray expected {0xAA};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt8BinaryLittleEndian)
{
    uint8_t value = 0xAA;
    OSAL::ByteArray expected {0xAA};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt8BinaryBigEndian)
{
    uint8_t value = 0xAA;
    OSAL::ByteArray expected {0xAA};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt16Binary)
{
    int16_t value = 0x5556;
    OSAL::ByteArray expected {0x56, 0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt16BinaryLittleEndian)
{
    int16_t value = 0x5556;
    OSAL::ByteArray expected {0x56, 0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt16BinaryBigEndian)
{
    int16_t value = 0x5556;
    OSAL::ByteArray expected {0x55, 0x56};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt16Binary)
{
    uint16_t value = 0x5556;
    OSAL::ByteArray expected {0x56, 0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt16BinaryLittleEndian)
{
    uint16_t value = 0x5556;
    OSAL::ByteArray expected {0x56, 0x55};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt16BinaryBigEndian)
{
    uint16_t value = 0x5556;
    OSAL::ByteArray expected {0x55, 0x56};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt32Binary)
{
    int32_t value = 0x12345678;
    OSAL::ByteArray expected {0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt32BinaryLittleEndian)
{
    int32_t value = 0x12345678;
    OSAL::ByteArray expected {0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt32BinaryBigEndian)
{
    int32_t value = 0x12345678;
    OSAL::ByteArray expected {0x12, 0x34, 0x56, 0x78};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt32Binary)
{
    uint32_t value = 0x12345678;
    OSAL::ByteArray expected {0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt32BinaryLittleEndian)
{
    uint32_t value = 0x12345678;
    OSAL::ByteArray expected {0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt32BinaryBigEndian)
{
    uint32_t value = 0x12345678;
    OSAL::ByteArray expected {0x12, 0x34, 0x56, 0x78};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt64Binary)
{
    int64_t value = 0x123456789ABCDEF0;
    OSAL::ByteArray expected {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt64BinaryLittleEndian)
{
    int64_t value = 0x123456789ABCDEF0;
    OSAL::ByteArray expected {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeInt64BinaryBigEndian)
{
    int64_t value = 0x123456789ABCDEF0;
    OSAL::ByteArray expected {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt64Binary)
{
    uint64_t value = 0x123456789ABCDEF0;
    OSAL::ByteArray expected {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt64BinaryLittleEndian)
{
    uint64_t value = 0x123456789ABCDEF0;
    OSAL::ByteArray expected {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt64BinaryBigEndian)
{
    uint64_t value = 0x123456789ABCDEF0;
    OSAL::ByteArray expected {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeFloatBinary)
{
    float value = 1.0002e4F;
    OSAL::ByteArray expected {0x00, 0x48, 0x1C, 0x46};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeFloatBinaryLittleEndian)
{
    float value = 1.0002e4F;
    OSAL::ByteArray expected {0x00, 0x48, 0x1C, 0x46};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeFloatBinaryBigEndian)
{
    float value = 1.0002e4F;
    OSAL::ByteArray expected {0x46, 0x1C, 0x48, 0x00};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeDoubleBinary)
{
    double value = 1.00002E05;
    OSAL::ByteArray expected {0x00, 0x00, 0x00, 0x00, 0x20, 0x6A, 0xF8, 0x40};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeDoubleBinaryLittleEndian)
{
    double value = 1.00002E05;
    OSAL::ByteArray expected {0x00, 0x00, 0x00, 0x00, 0x20, 0x6A, 0xF8, 0x40};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeDoubleBinaryBigEndian)
{
    double value = 1.00002E05;
    OSAL::ByteArray expected {0x40, 0xF8, 0x6A, 0x20, 0x00, 0x00, 0x00, 0x00};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeLongDoubleBinary)
{
    long double value = 1.00002E05L;
    OSAL::ByteArray expected {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0xC3, 0x0F, 0x40};
    OSAL::ByteArray actual;
    Serialize(actual, value);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeLongDoubleBinaryLittleEndian)
{
    long double value = 1.00002E05L;
    OSAL::ByteArray expected {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0xC3, 0x0F, 0x40};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::LittleEndian);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SerializationImplTest, SerializeLongDoubleBinaryBigEndian)
{
    long double value = 1.00002E05L;
    OSAL::ByteArray expected {0x40, 0x0F, 0xC3, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    OSAL::ByteArray actual;
    Serialize(actual, value, Util::Endianness::BigEndian);
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
