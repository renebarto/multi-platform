#include <unittest-c++/UnitTestC++.h>
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
    virtual void SetUp();
    virtual void TearDown();
};

void SerializationImplTest::SetUp()
{
}

void SerializationImplTest::TearDown()
{
}

TEST_FIXTURE(SerializationImplTest, SerializeBool)
{
    bool value = true;
    EXPECT_EQ(_("true"), Serialize(value));
    value = false;
    EXPECT_EQ(_("false"), Serialize(value));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt8)
{
    int8_t value = 0x55;
    EXPECT_EQ(_("01010101"), Serialize(value, 2));
    EXPECT_EQ(_("125"), Serialize(value, 8));
    EXPECT_EQ(_("85"), Serialize(value, 10));
    EXPECT_EQ(_("55"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt8)
{
    uint8_t value = 0xAA;
    EXPECT_EQ(_("10101010"), Serialize(value, 2));
    EXPECT_EQ(_("252"), Serialize(value, 8));
    EXPECT_EQ(_("170"), Serialize(value, 10));
    EXPECT_EQ(_("AA"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt16)
{
    int16_t value = 0x5555;
    EXPECT_EQ(_("0101010101010101"), Serialize(value, 2));
    EXPECT_EQ(_("052525"), Serialize(value, 8));
    EXPECT_EQ(_("21845"), Serialize(value, 10));
    EXPECT_EQ(_("5555"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt16)
{
    uint16_t value = 0xAAAA;
    EXPECT_EQ(_("1010101010101010"), Serialize(value, 2));
    EXPECT_EQ(_("125252"), Serialize(value, 8));
    EXPECT_EQ(_("43690"), Serialize(value, 10));
    EXPECT_EQ(_("AAAA"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt32)
{
    int32_t value = 0x55555555;
    EXPECT_EQ(_("01010101010101010101010101010101"), Serialize(value, 2));
    EXPECT_EQ(_("12525252525"), Serialize(value, 8));
    EXPECT_EQ(_("1431655765"), Serialize(value, 10));
    EXPECT_EQ(_("55555555"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt32)
{
    uint32_t value = 0xAAAAAAAA;
    EXPECT_EQ(_("10101010101010101010101010101010"), Serialize(value, 2));
    EXPECT_EQ(_("25252525252"), Serialize(value, 8));
    EXPECT_EQ(_("2863311530"), Serialize(value, 10));
    EXPECT_EQ(_("AAAAAAAA"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeInt64)
{
    int64_t value = 0x5555555555555555;
    EXPECT_EQ(_("0101010101010101010101010101010101010101010101010101010101010101"), Serialize(value, 2));
    EXPECT_EQ(_("0525252525252525252525"), Serialize(value, 8));
    EXPECT_EQ(_("6148914691236517205"), Serialize(value, 10));
    EXPECT_EQ(_("5555555555555555"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeUInt64)
{
    uint64_t value = 0xAAAAAAAAAAAAAAAA;
    EXPECT_EQ(_("1010101010101010101010101010101010101010101010101010101010101010"), Serialize(value, 2));
    EXPECT_EQ(_("1252525252525252525252"), Serialize(value, 8));
    EXPECT_EQ(_("12297829382473034410"), Serialize(value, 10));
    EXPECT_EQ(_("AAAAAAAAAAAAAAAA"), Serialize(value, 16));
}

TEST_FIXTURE(SerializationImplTest, SerializeFloat)
{
    float value = 1.0002e4F;
    EXPECT_EQ(_("10002"), Serialize(value));
    value = 5.0E-37F;
    EXPECT_EQ(_("5.000000179695649e-37"), Serialize(value));
    EXPECT_EQ(_("5e-37"), Serialize(value, 1));
}

TEST_FIXTURE(SerializationImplTest, SerializeDouble)
{
    double value = 1.00002E05;
    EXPECT_EQ(_("100002"), Serialize(value));
    value = 1e-200;
    EXPECT_EQ(_("1e-200"), Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ(_("1.001e-200"), Serialize(value));
    EXPECT_EQ(_("1e-200"), Serialize(value, 2));
}

TEST_FIXTURE(SerializationImplTest, SerializeLongDouble)
{
    long double value = 1.00002E05;
    EXPECT_EQ(_("100002"), Serialize(value));
    value = 1e-200;
    EXPECT_EQ(_("1e-200"), Serialize(value));
    value = 1.001e-200;
    EXPECT_EQ(_("1.001e-200"), Serialize(value));
    EXPECT_EQ(_("1e-200"), Serialize(value, 2));
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
