#include "unit-test-c++/UnitTestC++.h"
#include "osal/IPV4EndPoint.h"
#include "osal/MACAddress.h"
#include "core/Core.h"

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

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

TEST_SUITE(network)
{

TEST_FIXTURE(IPV4EndPointTest, Constructor)
{
    IPV4EndPoint target;
    OSAL::Network::IPV4Address referenceAddress;
    uint16_t referencePort = IPV4EndPoint::AnyPort;
    const string expected = "0.0.0.0:0";
    EXPECT_TRUE(referenceAddress == target.GetIPAddress());
    EXPECT_TRUE(referencePort == target.GetPort());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorCopy)
{
    IPV4EndPoint ipEndPoint(OSAL::Network::IPV4Address({ 1, 2, 3, 4 }), 1234);
    IPV4EndPoint target(ipEndPoint);
    const string expected = "1.2.3.4:1234";
    EXPECT_TRUE(ipEndPoint.GetIPAddress() == target.GetIPAddress());
    EXPECT_TRUE(ipEndPoint.GetPort() == target.GetPort());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorIPAddress)
{
    OSAL::Network::IPV4Address ipAddress({ 1, 2, 3, 4 });
    IPV4EndPoint target(ipAddress);
    const string expected = "1.2.3.4:0";
    EXPECT_TRUE(ipAddress == target.GetIPAddress());
    EXPECT_TRUE(IPV4EndPoint::AnyPort == target.GetPort());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorIPAddressPort)
{
    OSAL::Network::IPV4Address ipAddress({ 1, 2, 3, 4 });
    uint16_t port = 1234;
    IPV4EndPoint target(ipAddress, port);
    const string expected = "1.2.3.4:1234";
    EXPECT_TRUE(ipAddress == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorIPAddressUInt32Port)
{
    uint32_t ipAddress = 0x04030201;
    uint16_t port = 1234;
    IPV4EndPoint target(ipAddress, port);
    const string expected = "1.2.3.4:1234";
    EXPECT_TRUE(ipAddress == target.GetIPAddress().GetUInt32());
    EXPECT_TRUE(port == target.GetPort());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, ConstructorPort)
{
    uint16_t port = 1234;
    IPV4EndPoint target(port);
    const string expected = "0.0.0.0:1234";
    EXPECT_TRUE(OSAL::Network::IPV4Address::None == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, ParseIPAddressOnly)
{
    const string text = "255.254.253.252";
    OSAL::Network::IPV4Address ipAddress({ 255, 254, 253, 252 });
    IPV4EndPoint expected(ipAddress);
    const IPV4EndPoint & actual = IPV4EndPoint::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4EndPointTest, Parse)
{
    const string text = "255.254.253.252:12345";
    OSAL::Network::IPV4Address ipAddress({ 255, 254, 253, 252 });
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

    EXPECT_THROW(IPV4EndPoint::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(IPV4EndPoint::Parse(text2), OSAL::ArgumentException);
    EXPECT_THROW(IPV4EndPoint::Parse(text3), OSAL::ArgumentException);
}

TEST_FIXTURE(IPV4EndPointTest, TryParseIPAddressOnly)
{
    const string text = "255.254.253.252";
    OSAL::Network::IPV4Address ipAddress({ 255, 254, 253, 252 });
    IPV4EndPoint expected(ipAddress);
    IPV4EndPoint actual;
    EXPECT_TRUE(IPV4EndPoint::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4EndPointTest, TryParse)
{
    const string text = "255.254.253.252:8080";
    OSAL::ByteArray ipAddress({ 255, 254, 253, 252 });
    uint16_t port = 8080;
    IPV4EndPoint actual;
    IPV4EndPoint expected(ipAddress, port);
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

TEST_FIXTURE(IPV4EndPointTest, OperatorEqualEndPoint)
{
    OSAL::Network::IPV4Address ipAddress({ 1, 2, 3, 4 });
    uint16_t port = 1234;
    OSAL::Network::IPV4EndPoint target(ipAddress, port);
    OSAL::Network::IPV4EndPoint ref1;
    OSAL::Network::IPV4EndPoint ref2(IPV4Address({0, 0, 0, 0}), 0);
    OSAL::Network::IPV4EndPoint ref3(ipAddress, port);
    OSAL::Network::MACAddress ref4({0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(target == ref4);
    EXPECT_FALSE(ref1 == target);
    EXPECT_TRUE(ref1 == ref2);
    EXPECT_FALSE(ref1 == ref3);
    EXPECT_FALSE(ref1 == ref4);
    EXPECT_FALSE(ref2 == target);
    EXPECT_TRUE(ref2 == ref1);
    EXPECT_FALSE(ref2 == ref3);
    EXPECT_FALSE(ref2 == ref4);
    EXPECT_TRUE(ref3 == target);
    EXPECT_FALSE(ref3 == ref1);
    EXPECT_FALSE(ref3 == ref2);
    EXPECT_FALSE(ref3 == ref4);
    EXPECT_FALSE(ref4 == target);
    EXPECT_FALSE(ref4 == ref1);
    EXPECT_FALSE(ref4 == ref2);
    EXPECT_FALSE(ref4 == ref3);
}

TEST_FIXTURE(IPV4EndPointTest, OperatorNotEqualEndPoint)
{
    OSAL::Network::IPV4Address ipAddress({ 1, 2, 3, 4 });
    uint16_t port = 1234;
    OSAL::Network::IPV4EndPoint target(ipAddress, port);
    OSAL::Network::IPV4EndPoint ref1;
    OSAL::Network::IPV4EndPoint ref2(IPV4Address({0, 0, 0, 0}), 0);
    OSAL::Network::IPV4EndPoint ref3(ipAddress, port);
    OSAL::Network::MACAddress ref4({0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(target != ref4);
    EXPECT_TRUE(ref1 != target);
    EXPECT_FALSE(ref1 != ref2);
    EXPECT_TRUE(ref1 != ref3);
    EXPECT_TRUE(ref1 != ref4);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref2 != ref1);
    EXPECT_TRUE(ref2 != ref3);
    EXPECT_TRUE(ref2 != ref4);
    EXPECT_FALSE(ref3 != target);
    EXPECT_TRUE(ref3 != ref1);
    EXPECT_TRUE(ref3 != ref2);
    EXPECT_TRUE(ref3 != ref4);
    EXPECT_TRUE(ref4 != target);
    EXPECT_TRUE(ref4 != ref1);
    EXPECT_TRUE(ref4 != ref2);
    EXPECT_TRUE(ref4 != ref3);
}

TEST_FIXTURE(IPV4EndPointTest, OperatorEqual)
{
    OSAL::Network::IPV4Address ipAddress({ 1, 2, 3, 4 });
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

TEST_FIXTURE(IPV4EndPointTest, OperatorNotEqual)
{
    OSAL::Network::IPV4Address ipAddress({ 1, 2, 3, 4 });
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

TEST_FIXTURE(IPV4EndPointTest, PrintTo)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4EndPoint endPoint(ipAddress, 1234);
    string expected = _("1.2.3.4:1234");
    basic_ostringstream<OSAL::Char> stream;
    endPoint.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4EndPointTest, PrintToNonMember)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4EndPoint endPoint(ipAddress, 1234);
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(endPoint, stream);
    EXPECT_EQ(_("1.2.3.4:1234"), stream.str());
}

} // TEST_SUITE(network)

} // namespace Test
} // namespace Network
} // namespace OSAL
