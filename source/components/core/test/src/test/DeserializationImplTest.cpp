#include "unit-test-c++/UnitTestC++.h"
#include "core/serialization/DeserializationImpl.h"

using namespace std;

namespace Core
{
namespace Test
{

TEST_SUITE(core)
{

class DeserializationImplTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void DeserializationImplTest::SetUp()
{
}

void DeserializationImplTest::TearDown()
{
}

TEST_FIXTURE(DeserializationImplTest, DeserializeBool)
{
    bool expected = true;
    bool actual;
    EXPECT_TRUE(Deserialize(_("true"), actual));
    EXPECT_EQ(expected, actual);

    expected = false;
    EXPECT_TRUE(Deserialize(_("FaLsE"), actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("abcd"), actual));
    EXPECT_FALSE(Deserialize(_("1234"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeInt8)
{
    int8_t expected = 127;
    int8_t actual;
    EXPECT_TRUE(Deserialize(_("127"), actual));
    EXPECT_EQ(expected, actual);

    expected = 0x55;
    EXPECT_TRUE(Deserialize(_("55"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("1234"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeUInt8)
{
    uint8_t expected = 254;
    uint8_t actual;
    EXPECT_TRUE(Deserialize(_("254"), actual));
    EXPECT_EQ(expected, actual);

    expected = 0xab;
    EXPECT_TRUE(Deserialize(_("aB"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("1234"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeInt16)
{
    int16_t expected = 12345;
    int16_t actual;
    EXPECT_TRUE(Deserialize(_("12345"), actual));
    EXPECT_EQ(expected, actual);

    WARNING_PUSH
    WARNING_DISABLE(4309)
    expected = static_cast<int16_t>(0xabcd);
    WARNING_POP
    EXPECT_TRUE(Deserialize(_("aBcD"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("54321"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeUInt16)
{
    uint16_t expected = 54321;
    uint16_t actual;
    EXPECT_TRUE(Deserialize(_("54321"), actual));
    EXPECT_EQ(expected, actual);

    expected = 0xabcd;
    EXPECT_TRUE(Deserialize(_("aBcD"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("654321"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeInt32)
{
    int32_t expected = 12345678;
    int32_t actual;
    EXPECT_TRUE(Deserialize(_("12345678"), actual));
    EXPECT_EQ(expected, actual);

    expected = (int32_t) 0xabcdefef;
    EXPECT_TRUE(Deserialize(_("aBcDEfeF"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("FFFFFFFFF"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeUInt32)
{
    uint32_t expected = 4000000000u;
    uint32_t actual;
    EXPECT_TRUE(Deserialize(_("4000000000"), actual));
    EXPECT_EQ(expected, actual);

    expected = 0xabcdefef;
    EXPECT_TRUE(Deserialize(_("aBcDeFeF"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("FFFFFFFFF"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeInt64)
{
    int64_t expected = 987654321;
    int64_t actual;
    EXPECT_TRUE(Deserialize(_("987654321"), actual));
    EXPECT_EQ(expected, actual);

    expected = 0xfedcba9876543210;
    EXPECT_TRUE(Deserialize(_("fEdCbA9876543210"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("FFFFFFFFFFFFFFFFF"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeUInt64)
{
    uint64_t expected = 9223372036854775808ULL;
    uint64_t actual{};
    EXPECT_TRUE(Deserialize(_("9223372036854775808"), actual));
    EXPECT_EQ(expected, actual);

    expected = 0xfedcba9876543210;
    EXPECT_TRUE(Deserialize(_("fEdCbA9876543210"), actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
    EXPECT_FALSE(Deserialize(_("FFFFFFFFFFFFFFFFF"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeFloat)
{
    float expected = 1002;
    float actual;
    EXPECT_TRUE(Deserialize(_("1.002E+03"), actual));
    EXPECT_EQ(expected, actual);

    expected = 5.0E-37F;
    EXPECT_TRUE(Deserialize(_("5e-37"), actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeDouble)
{
    double expected = 1000002;
    double actual;
    EXPECT_TRUE(Deserialize(_("1.000002E+06"), actual));
    EXPECT_EQ(expected, actual);

    expected = 1e-200;
    EXPECT_TRUE(Deserialize(_("1e-200"), actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
}

TEST_FIXTURE(DeserializationImplTest, DeserializeLongDouble)
{
    long double expected = 1000002;
    long double actual;
    EXPECT_TRUE(Deserialize(_("1.000002E+06"), actual));
    EXPECT_EQ(expected, actual);

    expected = 1e-200;
    EXPECT_TRUE(Deserialize(_("1e-200"), actual));
    EXPECT_NEAR(expected, actual, 1e-10);

    EXPECT_FALSE(Deserialize(_("xyz"), actual));
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
