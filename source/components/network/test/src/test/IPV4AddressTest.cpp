#include "unit-test-c++/UnitTestC++.h"
#include "network/IPV4Address.h"
#include "core/Core.h"

using namespace std;

namespace Network
{

namespace Test
{

class IPV4AddressTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void IPV4AddressTest::SetUp()
{

}

void IPV4AddressTest::TearDown()
{

}

TEST_FIXTURE(IPV4AddressTest, Constructor)
{
    IPV4Address target;
    Core::ByteArray reference({0, 0, 0, 0});
    const string expected = "0.0.0.0";
    EXPECT_TRUE(reference == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorCopy)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    IPV4Address target(ipAddress);
    const string expected = "1.2.3.4";
    EXPECT_TRUE(ipAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArray)
{
    Core::ByteArray ipAddress({ 1, 2, 3, 4 });
    IPV4Address target({ 1, 2, 3, 4 });
    const string expected = "1.2.3.4";
    EXPECT_TRUE(ipAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArrayInvalid)
{
    EXPECT_THROW(IPV4Address({ 1, 2, 3 }), Core::ArgumentOutOfRangeException);
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArrayOffset)
{
    Core::ByteArray reference({ 1, 2, 3, 4 });
    IPV4Address target({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 1);
    const string expected = "1.2.3.4";
    EXPECT_TRUE(reference == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4AddressTest, CreateByteArray)
{
    Core::ByteArray ipAddress({ 1, 2, 3, 4 });
    IPV4Address target({ 1, 2, 3, 4 });
    const string expected = "1.2.3.4";
    EXPECT_TRUE(ipAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4AddressTest, CreateByteArrayOffset)
{
    Core::ByteArray ipAddress({ 1, 2, 3, 4 });
    IPV4Address target({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 1);
    const string expected = "1.2.3.4";
    EXPECT_TRUE(ipAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4AddressTest, CreateUInt32)
{
    Core::ByteArray expected({ 255, 254, 253, 252 });
    IPV4Address target(0xFCFDFEFF);
    Core::ByteArray actual = target.GetBytes();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, OperatorIndex)
{
    Core::ByteArray ipAddress({ 255, 254, 253, 252 });
    IPV4Address target(ipAddress);
    EXPECT_EQ(ipAddress[0], target[0]);
    EXPECT_EQ(ipAddress[1], target[1]);
    EXPECT_EQ(ipAddress[2], target[2]);
    EXPECT_EQ(ipAddress[3], target[3]);
}

TEST_FIXTURE(IPV4AddressTest, GetUInt32)
{
    const string text = "255.254.253.252";
    IPV4Address ipAddress({ 255, 254, 253, 252 });
    uint32_t expected = 0xFCFDFEFF;
    uint32_t actual = ipAddress.GetUInt32();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, SetUInt32)
{
    IPV4Address ipAddress;
    uint32_t expected = 0xFFFEFDFC;
    ipAddress.SetUInt32(expected);
    uint32_t actual = ipAddress.GetUInt32();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseNumeric)
{
    const string text = "255.254.253.252";
    Core::ByteArray ipAddress({ 255, 254, 253, 252 });
    IPV4Address expected(ipAddress);
    const IPV4Address & actual = IPV4Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseHostName)
{
    const string text = "localhost";
    Core::ByteArray ipAddress({ 127, 0, 0, 1 });
    IPV4Address expected(ipAddress);
    const IPV4Address & actual = IPV4Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseBroadcast)
{
    const string text1 = "255.255.255.255";

    EXPECT_EQ(IPV4Address({255, 255, 255, 255}), IPV4Address::Parse(text1));
}

TEST_FIXTURE(IPV4AddressTest, ParseInvalid)
{
    const string text1 = "256.255.255.255";
    const string text2 = "255.255.255.255.255";

    EXPECT_THROW(IPV4Address::Parse(text1), Core::ArgumentException);
    EXPECT_THROW(IPV4Address::Parse(text2), Core::ArgumentException);
}

TEST_FIXTURE(IPV4AddressTest, TryParse)
{
    const string text = "255.254.253.252";
    Core::ByteArray ipAddress({ 255, 254, 253, 252 });
    IPV4Address actual;
    IPV4Address expected(ipAddress);
    EXPECT_TRUE(IPV4Address::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, TryParseInvalid)
{
    const string text1 = "256.255.255.255";
    const string text2 = "255.255.255.255.255";
    IPV4Address ipAddress;
    EXPECT_FALSE(IPV4Address::TryParse(text1, ipAddress));
    EXPECT_FALSE(IPV4Address::TryParse(text2, ipAddress));
}

TEST_FIXTURE(IPV4AddressTest, OperatorNotEqual)
{
    Core::ByteArray ipAddress({ 1, 2, 3, 4 });
    IPV4Address target(ipAddress);
    IPV4Address ref1;
    IPV4Address ref2({ 0, 0, 0, 0 });
    IPV4Address ref3(ipAddress);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(ref1 != target);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref3 != target);
}

TEST_FIXTURE(IPV4AddressTest, OperatorEqual)
{
    Core::ByteArray ipAddress({ 1, 2, 3, 4 });
    IPV4Address target(ipAddress);
    IPV4Address ref1;
    IPV4Address ref2({ 0, 0, 0, 0 });
    IPV4Address ref3(ipAddress);
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(ref1 == target);
    EXPECT_FALSE(ref2 == target);
    EXPECT_TRUE(ref3 == target);
}

TEST_FIXTURE(IPV4AddressTest, ToString)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    EXPECT_EQ("1.2.3.4", ipAddress.ToString());
}

TEST_FIXTURE(IPV4AddressTest, PrintTo)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("1.2.3.4", stream.str());
}

} // namespace Test

} // namespace Network
