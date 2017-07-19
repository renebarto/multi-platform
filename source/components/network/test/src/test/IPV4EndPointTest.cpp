#include "unit-test-c++/UnitTestC++.h"
#include "network/IPV4EndPoint.h"
#include "core/Core.h"

using namespace std;

namespace Network
{

namespace Test
{

class IPV4EndPointTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void IPV4EndPointTest::SetUp()
{

}

void IPV4EndPointTest::TearDown()
{

}

TEST_FIXTURE(IPV4EndPointTest, Constructor)
{
    IPV4EndPoint target;
    IPV4Address referenceAddress;
    uint16_t referencePort = IPV4EndPoint::AnyPort;
    const string expected = "0.0.0.0:0";
    EXPECT_TRUE(referenceAddress == target.GetIPAddress());
    EXPECT_TRUE(referencePort == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorCopy)
{
    IPV4EndPoint ipEndPoint(IPV4Address({ 1, 2, 3, 4 }), 1234);
    IPV4EndPoint target(ipEndPoint);
    const string expected = "1.2.3.4:1234";
    EXPECT_TRUE(ipEndPoint.GetIPAddress() == target.GetIPAddress());
    EXPECT_TRUE(ipEndPoint.GetPort() == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorIPAddress)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    IPV4EndPoint target(ipAddress);
    const string expected = "1.2.3.4:0";
    EXPECT_TRUE(ipAddress == target.GetIPAddress());
    EXPECT_TRUE(IPV4EndPoint::AnyPort == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorIPAddressPort)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    uint16_t port = 1234;
    IPV4EndPoint target(ipAddress, port);
    const string expected = "1.2.3.4:1234";
    EXPECT_TRUE(ipAddress == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorIPAddressUInt32Port)
{
    uint32_t ipAddress = 0x04030201;
    uint16_t port = 1234;
    IPV4EndPoint target(ipAddress, port);
    const string expected = "1.2.3.4:1234";
    EXPECT_TRUE(ipAddress == target.GetIPAddress().GetUInt32());
    EXPECT_TRUE(port == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorPort)
{
    uint16_t port = 1234;
    IPV4EndPoint target(port);
    const string expected = "0.0.0.0:1234";
    EXPECT_TRUE(IPV4Address::None == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV4EndPointTest, ParseIPAddressOnly)
{
    const string text = "255.254.253.252";
    IPV4Address ipAddress({ 255, 254, 253, 252 });
    IPV4EndPoint expected(ipAddress);
    const IPV4EndPoint & actual = IPV4EndPoint::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4EndPointTest, Parse)
{
    const string text = "255.254.253.252:12345";
    IPV4Address ipAddress({ 255, 254, 253, 252 });
    uint16_t port = 12345;
    IPV4EndPoint expected(ipAddress, port);
    const IPV4EndPoint & actual = IPV4EndPoint::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4EndPointTest, ParseInvalid)
{
    const string text1 = "256.255.255.255:12345";
    const string text2 = "255.255.255.255.255";
    const string text3 = "255.255.255.255:123456";

    EXPECT_THROW(IPV4EndPoint::Parse(text1), Core::ArgumentException);
    EXPECT_THROW(IPV4EndPoint::Parse(text2), Core::ArgumentException);
    EXPECT_THROW(IPV4EndPoint::Parse(text3), Core::ArgumentException);
}

TEST_FIXTURE(IPV4EndPointTest, TryParseIPAddressOnly)
{
    const string text = "255.254.253.252";
    IPV4Address ipAddress({ 255, 254, 253, 252 });
    IPV4EndPoint expected(ipAddress);
    IPV4EndPoint actual;
    EXPECT_TRUE(IPV4EndPoint::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4EndPointTest, TryParse)
{
    const string text = "255.254.253.252";
    Core::ByteArray ipAddress({ 255, 254, 253, 252 });
    IPV4EndPoint actual;
    IPV4EndPoint expected(ipAddress);
    EXPECT_TRUE(IPV4EndPoint::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4EndPointTest, TryParseInvalid)
{
    const string text1 = "256.255.255.255:12345";
    const string text2 = "255.255.255.255.255";
    const string text3 = "255.255.255.255:123456";
    IPV4EndPoint ipAddress;
    EXPECT_FALSE(IPV4EndPoint::TryParse(text1, ipAddress));
    EXPECT_FALSE(IPV4EndPoint::TryParse(text2, ipAddress));
    EXPECT_FALSE(IPV4EndPoint::TryParse(text3, ipAddress));
}

TEST_FIXTURE(IPV4EndPointTest, OperatorNotEqual)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    uint16_t port = 1234;
    IPV4EndPoint target(ipAddress, port);
    IPV4EndPoint ref1;
    IPV4EndPoint ref2(ipAddress);
    IPV4EndPoint ref3(ipAddress, port);
    IPV4EndPoint ref4(port);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(target != ref4);
    EXPECT_TRUE(ref1 != target);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref3 != target);
    EXPECT_TRUE(ref4 != target);
}

TEST_FIXTURE(IPV4EndPointTest, OperatorEqual)
{
    IPV4Address ipAddress({ 1, 2, 3, 4 });
    uint16_t port = 1234;
    IPV4EndPoint target(ipAddress, port);
    IPV4EndPoint ref1;
    IPV4EndPoint ref2(ipAddress);
    IPV4EndPoint ref3(ipAddress, port);
    IPV4EndPoint ref4(port);
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(target == ref4);
    EXPECT_FALSE(ref1 == target);
    EXPECT_FALSE(ref2 == target);
    EXPECT_TRUE(ref3 == target);
    EXPECT_FALSE(ref4 == target);
}

} // namespace Test

} // namespace Network
