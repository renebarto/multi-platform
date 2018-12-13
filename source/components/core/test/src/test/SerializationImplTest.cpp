#include <unittest-cpp/UnitTestC++.h>

#include "core/serialization/SerializationImpl.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core)
{

class SerializationImplTest : public UnitTestCpp::TestFixture
{
};

TEST_FIXTURE(SerializationImplTest, SerializeBool)
{
    bool value = true;
    EXPECT_EQ("true", Serialize(value, 0));
    value = false;
    EXPECT_EQ("false", Serialize(value, 0));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt8)
{
    int8_t value = 0x55;
    EXPECT_EQ("01010101", Serialize(value, 0, 2));
    EXPECT_EQ("125", Serialize(value, 0, 8));
    EXPECT_EQ("85", Serialize(value, 0, 10));
    EXPECT_EQ("55", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt8)
{
    uint8_t value = 0xAA;
    EXPECT_EQ("10101010", Serialize(value, 0, 2));
    EXPECT_EQ("252", Serialize(value, 0, 8));
    EXPECT_EQ("170", Serialize(value, 0, 10));
    EXPECT_EQ("AA", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt16)
{
    int16_t value = 0x5555;
    EXPECT_EQ("0101010101010101", Serialize(value, 0, 2));
    EXPECT_EQ("052525", Serialize(value, 0, 8));
    EXPECT_EQ("21845", Serialize(value, 0, 10));
    EXPECT_EQ("5555", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt16)
{
    uint16_t value = 0xAAAA;
    EXPECT_EQ("1010101010101010", Serialize(value, 0, 2));
    EXPECT_EQ("125252", Serialize(value, 0, 8));
    EXPECT_EQ("43690", Serialize(value, 0, 10));
    EXPECT_EQ("AAAA", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt32)
{
    int32_t value = 0x55555555;
    EXPECT_EQ("01010101010101010101010101010101", Serialize(value, 0, 2));
    EXPECT_EQ("12525252525", Serialize(value, 0, 8));
    EXPECT_EQ("1431655765", Serialize(value, 0, 10));
    EXPECT_EQ("55555555", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt32)
{
    uint32_t value = 0xAAAAAAAA;
    EXPECT_EQ("10101010101010101010101010101010", Serialize(value, 0, 2));
    EXPECT_EQ("25252525252", Serialize(value, 0, 8));
    EXPECT_EQ("2863311530", Serialize(value, 0, 10));
    EXPECT_EQ("AAAAAAAA", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt64)
{
    int64_t value = 0x5555555555555555;
    EXPECT_EQ("0101010101010101010101010101010101010101010101010101010101010101", Serialize(value, 0, 2));
    EXPECT_EQ("0525252525252525252525", Serialize(value, 0, 8));
    EXPECT_EQ("6148914691236517205", Serialize(value, 0, 10));
    EXPECT_EQ("5555555555555555", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt64)
{
    uint64_t value = 0xAAAAAAAAAAAAAAAA;
    EXPECT_EQ("1010101010101010101010101010101010101010101010101010101010101010", Serialize(value, 0, 2));
    EXPECT_EQ("1252525252525252525252", Serialize(value, 0, 8));
    EXPECT_EQ("12297829382473034410", Serialize(value, 0, 10));
    EXPECT_EQ("AAAAAAAAAAAAAAAA", Serialize(value, 0, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeFloat)
{
    float value = 1.0002e4F;
    EXPECT_EQ("10002", Serialize(value));
    EXPECT_EQ("   10002", Serialize(value, 8));
    EXPECT_EQ("10002.0000", Serialize(value, 0, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ("0010002.0000", Serialize(value, 12, 4, FloatingPointRepresentation::Fixed));
#if defined(WIN_MINGW)
    EXPECT_EQ("1.0002e+004", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
#else
    EXPECT_EQ("1.0002e+04", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
#endif
    EXPECT_EQ("10002", Serialize(value, 0, 8, FloatingPointRepresentation::Mixed));
    value = 5.0E-37F;
#if defined(LINUX) || defined(APPLE)
    EXPECT_EQ("5.000000179695649e-37", Serialize(value));
    EXPECT_EQ("5e-37", Serialize(value, 0, 1));
#elif defined(WIN_MSVC)
    EXPECT_EQ("5.000000179695649e-37", Serialize(value));
    EXPECT_EQ("5e-37", Serialize(value, 0, 1));
#else
    EXPECT_EQ("5.000000179695649e-037", Serialize(value));
    EXPECT_EQ("5e-037", Serialize(value, 0, 1));
#endif
}

TEST_FIXTURE(SerializationImplTest, SerializeDouble)
{
    double value = 1.00002E05;
    EXPECT_EQ("100002", Serialize(value));
    EXPECT_EQ("  100002", Serialize(value, 8));
    EXPECT_EQ("100002.0000", Serialize(value, 0, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ("0100002.0000", Serialize(value, 12, 4, FloatingPointRepresentation::Fixed));
#if defined(WIN_MINGW)
    EXPECT_EQ("1.0000e+005", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
#else
    EXPECT_EQ("1.0000e+05", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
#endif
    EXPECT_EQ("100002", Serialize(value, 0, 8, FloatingPointRepresentation::Mixed));
    value = 1e-200;
    EXPECT_EQ("1e-200", Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ("1.001e-200", Serialize(value));
    EXPECT_EQ("1e-200", Serialize(value, 0, 2));
}

TEST_FIXTURE(SerializationImplTest, SerializeLongDouble)
{
    long double value = 1.00002E05;
    EXPECT_EQ("100002", Serialize(value));
    EXPECT_EQ("  100002", Serialize(value, 8));
    EXPECT_EQ("100002.0000", Serialize(value, 0, 4, FloatingPointRepresentation::Fixed));
    EXPECT_EQ("0100002.0000", Serialize(value, 12, 4, FloatingPointRepresentation::Fixed));
#if defined(WIN_MINGW)
    EXPECT_EQ("1.0000e+005", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
#else
    EXPECT_EQ("1.0000e+05", Serialize(value, 0, 4, FloatingPointRepresentation::Exponential));
#endif
    EXPECT_EQ("100002", Serialize(value, 0, 8, FloatingPointRepresentation::Mixed));
    value = 1e-200;
    EXPECT_EQ("1e-200", Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ("1.001e-200", Serialize(value));
    EXPECT_EQ("1e-200", Serialize(value, 0, 2));
}

TEST_FIXTURE(SerializationImplTest, SerializeStringNarrow)
{
    std::string value = "Hello";
    std::string expected = "Hello";
    EXPECT_EQ(expected, Serialize(value));
}

TEST_FIXTURE(SerializationImplTest, SerializeStringNarrowQuoted)
{
    std::string value = "Hello";
    std::string expected = "\"Hello\"";
    EXPECT_EQ(expected, Serialize(value, 0, true));
}

TEST_FIXTURE(SerializationImplTest, SerializeStringWide)
{
    std::wstring value = L"Hello";
    std::string expected = "Hello";
    EXPECT_EQ(expected, Serialize(value));
}

TEST_FIXTURE(SerializationImplTest, SerializeStringWideQuoted)
{
    std::wstring value = L"Hello";
    std::string expected = "\"Hello\"";
    EXPECT_EQ(expected, Serialize(value, 0, true));
}

TEST_FIXTURE(SerializationImplTest, SerializeCharPtrNarrow)
{
    const char * value = "Hello";
    std::string expected = "Hello";
    EXPECT_EQ(expected, Serialize(value));
}

TEST_FIXTURE(SerializationImplTest, SerializeCharPtrNarrowQuoted)
{
    const char * value = "Hello";
    std::string expected = "\"Hello\"";
    EXPECT_EQ(expected, Serialize(value, 0, true));
}

TEST_FIXTURE(SerializationImplTest, SerializeCharPtrWide)
{
    const wchar_t * value = L"Hello";
    std::string expected = "Hello";
    EXPECT_EQ(expected, Serialize(value));
}

TEST_FIXTURE(SerializationImplTest, SerializeCharPtrWideQuoted)
{
    const wchar_t * value = L"Hello";
    std::string expected = "\"Hello\"";
    EXPECT_EQ(expected, Serialize(value, 0, true));
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
