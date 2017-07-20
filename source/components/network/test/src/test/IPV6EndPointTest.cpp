#include "unit-test-c++/UnitTestC++.h"
#include "network/IPV6EndPoint.h"
#include "core/Core.h"

using namespace std;

namespace Network
{

namespace Test
{

class IPV6EndPointTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void IPV6EndPointTest::SetUp()
{

}

void IPV6EndPointTest::TearDown()
{

}

TEST_FIXTURE(IPV6EndPointTest, Constructor)
{
    IPV6EndPoint target;
    IPV6Address referenceAddress;
    uint16_t referencePort = IPV6EndPoint::AnyPort;
    const string expected = "::";
    EXPECT_TRUE(referenceAddress == target.GetIPAddress());
    EXPECT_TRUE(referencePort == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorCopy)
{
    IPV6EndPoint ipEndPoint(IPV6Address({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }), 1234);
    IPV6EndPoint target(ipEndPoint);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_TRUE(ipEndPoint.GetIPAddress() == target.GetIPAddress());
    EXPECT_TRUE(ipEndPoint.GetPort() == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddress)
{
    IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    IPV6EndPoint target(ipAddress);
    const string expected = "102:304:506:708:90a:b0c:d0e:f10";
    EXPECT_TRUE(ipAddress == target.GetIPAddress());
    EXPECT_TRUE(IPV6EndPoint::AnyPort == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressPort)
{
    IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    IPV6EndPoint target(ipAddress, port);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_TRUE(ipAddress == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressUInt8Array)
{
    uint8_t ipAddress[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    uint16_t port = 1234;
    IPV6EndPoint target(ipAddress, port);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_TRUE(Core::ByteArray(ipAddress, sizeof(ipAddress)) == target.GetIPAddress().GetData());
    EXPECT_TRUE(port == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorPort)
{
    uint16_t port = 1234;
    IPV6EndPoint target(port);
    const string expected = "[::]:1234";
    EXPECT_TRUE(IPV6Address::None == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(IPV6EndPointTest, ParseIPAddressOnly)
{
    const string text = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    IPV6EndPoint expected(ipAddress);
    const IPV6EndPoint & actual = IPV6EndPoint::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6EndPointTest, Parse)
{
    const string text = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:12345";
    IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    uint16_t port = 12345;
    IPV6EndPoint expected(ipAddress, port);
    const IPV6EndPoint & actual = IPV6EndPoint::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6EndPointTest, ParseInvalid)
{
    const string text1 = "[ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:12345";
    const string text2 = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0";
    const string text3 = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:123456";

    EXPECT_THROW(IPV6EndPoint::Parse(text1), Core::ArgumentException);
    EXPECT_THROW(IPV6EndPoint::Parse(text2), Core::ArgumentException);
    EXPECT_THROW(IPV6EndPoint::Parse(text3), Core::ArgumentException);
}

TEST_FIXTURE(IPV6EndPointTest, TryParseIPAddressOnly)
{
    const string text = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    IPV6EndPoint expected(ipAddress);
    IPV6EndPoint actual;
    EXPECT_TRUE(IPV6EndPoint::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6EndPointTest, TryParse)
{
    const string text = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:8080";
    IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    uint16_t port = 8080;
    IPV6EndPoint actual;
    IPV6EndPoint expected(ipAddress, port);
    EXPECT_TRUE(IPV6EndPoint::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6EndPointTest, TryParseInvalid)
{
    const string text1 = "[ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:12345";
    const string text2 = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0";
    const string text3 = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:123456";
    IPV6EndPoint ipAddress;
    EXPECT_FALSE(IPV6EndPoint::TryParse(text1, ipAddress));
    EXPECT_FALSE(IPV6EndPoint::TryParse(text2, ipAddress));
    EXPECT_FALSE(IPV6EndPoint::TryParse(text3, ipAddress));
}

TEST_FIXTURE(IPV6EndPointTest, OperatorNotEqual)
{
    IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    IPV6EndPoint target(ipAddress, port);
    IPV6EndPoint ref1;
    IPV6EndPoint ref2(ipAddress);
    IPV6EndPoint ref3(ipAddress, port);
    IPV6EndPoint ref4(port);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(target != ref4);
    EXPECT_TRUE(ref1 != target);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref3 != target);
    EXPECT_TRUE(ref4 != target);
}

TEST_FIXTURE(IPV6EndPointTest, OperatorEqual)
{
    IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    IPV6EndPoint target(ipAddress, port);
    IPV6EndPoint ref1;
    IPV6EndPoint ref2(ipAddress);
    IPV6EndPoint ref3(ipAddress, port);
    IPV6EndPoint ref4(port);
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
