#include <unittest-cpp/UnitTestC++.h>

#include "osal/IPV6EndPoint.h"
#include "osal/MACAddress.h"
#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

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

TEST_SUITE(network)
{

TEST_FIXTURE(IPV6EndPointTest, Constructor)
{
    IPV6EndPoint target;
    OSAL::Network::IPV6Address referenceAddress;
    uint16_t referencePort = IPV6EndPoint::AnyPort;
    const string expected = "::";
    EXPECT_EQ(referenceAddress, target.GetIPAddress());
    EXPECT_EQ(referencePort, target.GetPort());
    EXPECT_EQ(uint32_t{0}, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorCopy)
{
    IPV6EndPoint ipEndPoint(OSAL::Network::IPV6Address({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }), 1234);
    IPV6EndPoint target(ipEndPoint);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(ipEndPoint.GetIPAddress(), target.GetIPAddress());
    EXPECT_EQ(ipEndPoint.GetPort(), target.GetPort());
    EXPECT_EQ(uint32_t{0}, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddress)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    IPV6EndPoint target(ipAddress);
    const string expected = "102:304:506:708:90a:b0c:d0e:f10";
    EXPECT_EQ(ipAddress, target.GetIPAddress());
    // We compare directly here to not force creating storage for IPV6EndPoint::AnyPort
    EXPECT_TRUE(IPV6EndPoint::AnyPort == target.GetPort());
    EXPECT_EQ(uint32_t{0}, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressPort)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    IPV6EndPoint target(ipAddress, port);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(ipAddress, target.GetIPAddress());
    EXPECT_EQ(port, target.GetPort());
    EXPECT_EQ(uint32_t{0}, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressPortFlowInfo)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    uint32_t flowInfo = 12345678;
    IPV6EndPoint target(ipAddress, port, flowInfo);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(ipAddress, target.GetIPAddress());
    EXPECT_EQ(port, target.GetPort());
    EXPECT_EQ(flowInfo, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressPortFlowInfoScopeID)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    uint32_t flowInfo = 12345678;
    uint32_t scopeID = 87654321;
    IPV6EndPoint target(ipAddress, port, flowInfo, scopeID);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234%87654321";
    EXPECT_EQ(ipAddress, target.GetIPAddress());
    EXPECT_EQ(port, target.GetPort());
    EXPECT_EQ(flowInfo, target.GetFlowInfo());
    EXPECT_EQ(scopeID, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressUInt8ArrayPort)
{
    uint8_t ipAddress[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    uint16_t port = 1234;
    IPV6EndPoint target(ipAddress, port);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(OSAL::bytearray(ipAddress, sizeof(ipAddress)), target.GetIPAddress().GetData());
    EXPECT_EQ(port, target.GetPort());
    EXPECT_EQ(uint32_t{0}, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressUInt8ArrayPortFlowInfo)
{
    uint8_t ipAddress[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    uint16_t port = 1234;
    uint32_t flowInfo = 12345678;
    IPV6EndPoint target(ipAddress, port, flowInfo);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(OSAL::bytearray(ipAddress, sizeof(ipAddress)), target.GetIPAddress().GetData());
    EXPECT_EQ(port, target.GetPort());
    EXPECT_EQ(flowInfo, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorIPAddressUInt8ArrayPortFlowInfoScopeID)
{
    uint8_t ipAddress[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    uint16_t port = 1234;
    uint32_t flowInfo = 12345678;
    uint32_t scopeID = 87654321;
    IPV6EndPoint target(ipAddress, port, flowInfo, scopeID);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234%87654321";
    EXPECT_EQ(OSAL::bytearray(ipAddress, sizeof(ipAddress)), target.GetIPAddress().GetData());
    EXPECT_EQ(port, target.GetPort());
    EXPECT_EQ(flowInfo, target.GetFlowInfo());
    EXPECT_EQ(scopeID, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, ConstructorPort)
{
    uint16_t port = 1234;
    IPV6EndPoint target(port);
    const string expected = "[::]:1234";
    EXPECT_TRUE(OSAL::Network::IPV6Address::None == target.GetIPAddress());
    EXPECT_TRUE(port == target.GetPort());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, Assign)
{
    IPV6EndPoint ipEndPoint(OSAL::Network::IPV6Address({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }), 1234);
    IPV6EndPoint target;

    target = ipEndPoint;
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(ipEndPoint.GetIPAddress(), target.GetIPAddress());
    EXPECT_EQ(ipEndPoint.GetPort(), target.GetPort());
    EXPECT_EQ(uint32_t{0}, target.GetFlowInfo());
    EXPECT_EQ(uint32_t{0}, target.GetScopeID());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, CreateDefault)
{
    OSAL::Network::EndPointPtr target = OSAL::Network::IPV6EndPoint::Create();
    ASSERT_NOT_NULL(target);
    OSAL::Network::IPV6Address referenceAddress;
    uint16_t referencePort = IPV6EndPoint::AnyPort;
    const string expected = "::";
    IPV6EndPoint reference(referenceAddress, referencePort);
    EXPECT_EQ(reference, *target);
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, CreateFromEndPointIPV6EndPoint)
{
    IPV6EndPoint other(OSAL::Network::IPV6Address({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }), 1234);
    OSAL::Network::EndPointPtr target = OSAL::Network::IPV6EndPoint::Create(other);
    ASSERT_NOT_NULL(target);
    const string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    EXPECT_EQ(other, *target);
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, CreateFromEndPointMACAddress)
{
    OSAL::Network::MACAddress other({0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
    OSAL::Network::EndPointPtr target = OSAL::Network::IPV6EndPoint::Create(other);
    EXPECT_NULL(target);
}

TEST_FIXTURE(IPV6EndPointTest, CreateFromString)
{
    const std::string text = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:12345";
    OSAL::Network::IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    uint16_t port = 8080;
    IPV6EndPoint expected(ipAddress, port);
    OSAL::Network::EndPointPtr actual = OSAL::Network::IPV6EndPoint::Create(text);
    ASSERT_NOT_NULL(actual);
    EXPECT_EQ(expected, *actual);
}

TEST_FIXTURE(IPV6EndPointTest, ParseIPAddressOnly)
{
    const string text = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    OSAL::Network::IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    IPV6EndPoint expected(ipAddress);
    const IPV6EndPoint & actual = IPV6EndPoint::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6EndPointTest, Parse)
{
    const string text = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:12345";
    OSAL::Network::IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
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

    EXPECT_THROW(IPV6EndPoint::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(IPV6EndPoint::Parse(text2), OSAL::ArgumentException);
    EXPECT_THROW(IPV6EndPoint::Parse(text3), OSAL::ArgumentException);
}

TEST_FIXTURE(IPV6EndPointTest, TryParseIPAddressOnly)
{
    const string text = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    OSAL::Network::IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
    IPV6EndPoint expected(ipAddress);
    IPV6EndPoint actual;
    EXPECT_TRUE(IPV6EndPoint::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6EndPointTest, TryParse)
{
    const string text = "[fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0]:8080";
    OSAL::Network::IPV6Address ipAddress({ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240 });
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

TEST_FIXTURE(IPV6EndPointTest, OperatorEqualEndPoint)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    OSAL::Network::IPV6EndPoint target(ipAddress, port);
    OSAL::Network::IPV6EndPoint ref1;
    OSAL::Network::IPV6EndPoint ref2(IPV6Address({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}), 0);
    OSAL::Network::IPV6EndPoint ref3(ipAddress, port);
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

TEST_FIXTURE(IPV6EndPointTest, OperatorNotEqualEndPoint)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    uint16_t port = 1234;
    OSAL::Network::IPV6EndPoint target(ipAddress, port);
    OSAL::Network::IPV6EndPoint ref1;
    OSAL::Network::IPV6EndPoint ref2(IPV6Address({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}), 0);
    OSAL::Network::IPV6EndPoint ref3(ipAddress, port);
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

TEST_FIXTURE(IPV6EndPointTest, OperatorEqual)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
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

TEST_FIXTURE(IPV6EndPointTest, OperatorNotEqual)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
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

TEST_FIXTURE(IPV6EndPointTest, PrintToNoPortNoScopeID)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    OSAL::Network::IPV6EndPoint endPoint(ipAddress, 0);
    string expected = "102:304:506:708:90a:b0c:d0e:f10";
    ostringstream stream;
    endPoint.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, PrintToNoPort)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    OSAL::Network::IPV6EndPoint endPoint(ipAddress, 0, 0, 2);
    string expected = "102:304:506:708:90a:b0c:d0e:f10%2";
    ostringstream stream;
    endPoint.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, PrintToNoScopeID)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    OSAL::Network::IPV6EndPoint endPoint(ipAddress, 1234);
    string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234";
    ostringstream stream;
    endPoint.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, PrintTo)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    OSAL::Network::IPV6EndPoint endPoint(ipAddress, 1234, 0, 2);
    string expected = "[102:304:506:708:90a:b0c:d0e:f10]:1234%2";
    ostringstream stream;
    endPoint.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, PrintToNonMemberNoScopeID)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    OSAL::Network::IPV6EndPoint endPoint(ipAddress, 1234);
    ostringstream stream;
    PrintTo(endPoint, stream);
    EXPECT_EQ("[102:304:506:708:90a:b0c:d0e:f10]:1234", stream.str());
}

TEST_FIXTURE(IPV6EndPointTest, PrintToNonMember)
{
    OSAL::Network::IPV6Address ipAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    OSAL::Network::IPV6EndPoint endPoint(ipAddress, 1234, 0, 2);
    ostringstream stream;
    PrintTo(endPoint, stream);
    EXPECT_EQ("[102:304:506:708:90a:b0c:d0e:f10]:1234%2", stream.str());
}

} // TEST_SUITE(network)

} // namespace Test
} // namespace Network
} // namespace OSAL
