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

#if !defined(WIN_MSVC)

TEST_FIXTURE(DeserializationImplTest, DeserializeDomainSocketAddress)
{
    OSAL::String text = _("/abc/def");
    OSAL::ByteArray domainAddress(text);
    OSAL::Network::DomainSocketAddress expected(domainAddress);
    OSAL::Network::DomainSocketAddress actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeDomainSocketAddressInvalid)
{
    const OSAL::String textTooLong = _("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678");

    OSAL::Network::DomainSocketAddress expected;
    OSAL::Network::DomainSocketAddress actual;
    EXPECT_FALSE(Deserialize(textTooLong, actual));
    EXPECT_EQ(expected, actual);
}

#endif

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV4AddressNumeric)
{
    const OSAL::String text = _("255.254.253.252");
    OSAL::Network::IPV4Address expected({255, 254, 253, 252});
    OSAL::Network::IPV4Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV4AddressHostName)
{
    const OSAL::String text = _("localhost");
    OSAL::Network::IPV4Address expected({127, 0, 0, 1});
    OSAL::Network::IPV4Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV4AddressBroadcast)
{
    const OSAL::String text = _("255.255.255.255");
    OSAL::Network::IPV4Address expected({255, 255, 255, 255});
    OSAL::Network::IPV4Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV4AddressInvalid)
{
    const OSAL::String text1 = _("256.255.255.255");
    const OSAL::String text2 = _("255.255.255.255.255");

    OSAL::Network::IPV4Address expected;
    OSAL::Network::IPV4Address actual;
    EXPECT_FALSE(Deserialize(text1, actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(text2, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV6AddressNumeric)
{
    const OSAL::String text = _(_("fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0"));
    OSAL::Network::IPV6Address expected({0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3, 0xF2, 0xF1, 0xF0});
    OSAL::Network::IPV6Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV6AddressHostName)
{
    const OSAL::String text = _("localhost");
    OSAL::Network::IPV6Address expected({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
    OSAL::Network::IPV6Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV6AddressLocalhost)
{
    const OSAL::String text = _("::1");
    OSAL::Network::IPV6Address expected({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
    OSAL::Network::IPV6Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV6AddressBroadcast)
{
    const OSAL::String text = _("ff02::1");
    OSAL::Network::IPV6Address expected({0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
    OSAL::Network::IPV6Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV6AddressShortened)
{
    const OSAL::String text = _("FD3D:4056:8978:0:E133:19D7:2C09:290E");
    OSAL::Network::IPV6Address expected({0xFD, 0x3D, 0x40, 0x56, 0x89, 0x78, 0x00, 0x00, 0xE1, 0x33, 0x19, 0xD7, 0x2C, 0x09, 0x29, 0x0E});
    OSAL::Network::IPV6Address actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeIPV6AddressInvalid)
{
    const OSAL::String text1 = _("ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0");
    const OSAL::String text2 = _("fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0");

    OSAL::Network::IPV6Address expected;
    OSAL::Network::IPV6Address actual;
    EXPECT_FALSE(Deserialize(text1, actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(text2, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeMACAddress)
{
    const OSAL::String text = _("FF-FE-FD-01-02-03");
    OSAL::Network::MACAddress expected({0xFF, 0xFE, 0xFD, 0x01, 0x02, 0x03});
    OSAL::Network::MACAddress actual;
    EXPECT_TRUE(Deserialize(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DeserializationImplTest, DeserializeMACAddressInvalid)
{
    const OSAL::String text1 = _("00-11-22-AA-BB");
    const OSAL::String text2 = _("XX-YY-ZZ-AA-BB-CC");
    OSAL::Network::MACAddress expected;
    OSAL::Network::MACAddress actual;
    EXPECT_FALSE(Deserialize(text1, actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Deserialize(text2, actual));
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
