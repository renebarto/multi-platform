#include <unittest-cpp/UnitTestC++.h>

#include <osal/IPV6Address.h>
#include <osal/Exception.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

class IPV6AddressTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void IPV6AddressTest::SetUp()
{

}

void IPV6AddressTest::TearDown()
{

}

TEST_SUITE(osal)
{

TEST_FIXTURE(IPV6AddressTest, Constructor)
{
    OSAL::Network::IPV6Address target;
    OSAL::bytearray reference({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    const string expected = "::";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorCopy)
{
    OSAL::Network::IPV6Address ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target(ipAddress);
    const string expected = "102:304:506:708:90a:b0c:d0e:f10";
    EXPECT_TRUE(ipAddress == target);
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorByteArray)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    const string expected = "102:304:506:708:90a:b0c:d0e:f10";
    EXPECT_TRUE(ipAddress == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorByteArrayOffset)
{
    OSAL::bytearray reference({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 1);
    const string expected = "102:304:506:708:90a:b0c:d0e:f10";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorUInt8Array)
{
    OSAL::bytearray expected({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    uint8_t address[16] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240};
    OSAL::Network::IPV6Address target(address);
    OSAL::bytearray actual = target.GetBytes();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ConstructorInAddr)
{
    in_addr6 ipAddress = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    OSAL::Network::IPV6Address target(ipAddress);
    const string expected = "102:304:506:708:90a:b0c:d0e:f10";
    EXPECT_TRUE(OSAL::bytearray({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}) == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, OperatorIndex)
{
    OSAL::bytearray ipAddress({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::Network::IPV6Address target(ipAddress);
    EXPECT_EQ(ipAddress[size_t{0}], target[0]);
    EXPECT_EQ(ipAddress[size_t{1}], target[1]);
    EXPECT_EQ(ipAddress[size_t{2}], target[2]);
    EXPECT_EQ(ipAddress[size_t{3}], target[3]);
    EXPECT_EQ(ipAddress[size_t{4}], target[4]);
    EXPECT_EQ(ipAddress[size_t{5}], target[5]);
    EXPECT_EQ(ipAddress[size_t{6}], target[6]);
    EXPECT_EQ(ipAddress[size_t{7}], target[7]);
    EXPECT_EQ(ipAddress[size_t{8}], target[8]);
    EXPECT_EQ(ipAddress[size_t{9}], target[9]);
    EXPECT_EQ(ipAddress[size_t{10}], target[10]);
    EXPECT_EQ(ipAddress[size_t{11}], target[11]);
    EXPECT_EQ(ipAddress[size_t{12}], target[12]);
    EXPECT_EQ(ipAddress[size_t{13}], target[13]);
    EXPECT_EQ(ipAddress[size_t{14}], target[14]);
    EXPECT_EQ(ipAddress[size_t{15}], target[15]);
}

TEST_FIXTURE(IPV6AddressTest, GetData)
{
    OSAL::Network::IPV6Address ipAddress(
        {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::bytearray expected({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::bytearray actual = ipAddress.GetData();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, SetData)
{
    OSAL::Network::IPV6Address ipAddress;
    OSAL::bytearray expected({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    ipAddress.SetData(expected);
    OSAL::bytearray actual = ipAddress.GetData();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ParseNumeric)
{
    const string text = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    OSAL::bytearray ipAddress({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::Network::IPV6Address expected(ipAddress);
    const OSAL::Network::IPV6Address & actual = OSAL::Network::IPV6Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ParseHostName)
{
    const string text = "localhost";
    OSAL::bytearray ipAddress({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    OSAL::Network::IPV6Address expected(ipAddress);
    const OSAL::Network::IPV6Address & actual = OSAL::Network::IPV6Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ParseLocalhost)
{
    const string text = "::1";

    EXPECT_EQ(OSAL::Network::IPV6Address({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}),
              OSAL::Network::IPV6Address::Parse(text));
}

TEST_FIXTURE(IPV6AddressTest, ParseBroadcast)
{
    const string text = "ff02::1";

    EXPECT_EQ(OSAL::Network::IPV6Address({255, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}),
              OSAL::Network::IPV6Address::Parse(text));
}

TEST_FIXTURE(IPV6AddressTest, ParseShortened)
{
    const string text = "FD3D:4056:8978:0:E133:19D7:2C09:290E";

    IPV6Address address = OSAL::Network::IPV6Address::Parse(text);
    EXPECT_EQ(OSAL::Network::IPV6Address({0xFD, 0x3D, 0x40, 0x56, 0x89, 0x78, 0x00, 0x00, 0xE1, 0x33, 0x19, 0xD7, 0x2C, 0x09, 0x29, 0x0E}),
              address);
}

TEST_FIXTURE(IPV6AddressTest, ParseInvalid)
{
    const string text1 = "ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    const string text2 = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0";

    EXPECT_THROW(OSAL::Network::IPV6Address::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::Network::IPV6Address::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(IPV6AddressTest, TryParse)
{
    const string text = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    OSAL::bytearray ipAddress({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::Network::IPV6Address actual;
    OSAL::Network::IPV6Address expected(ipAddress);
    EXPECT_TRUE(OSAL::Network::IPV6Address::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, TryParseInvalid)
{
    const string text1 = "ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0";
    const string text2 = "fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0";
    OSAL::Network::IPV6Address ipAddress;
    EXPECT_FALSE(OSAL::Network::IPV6Address::TryParse(text1, ipAddress));
    EXPECT_FALSE(OSAL::Network::IPV6Address::TryParse(text2, ipAddress));
}

TEST_FIXTURE(IPV6AddressTest, OperatorEqual)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target(ipAddress);
    OSAL::Network::IPV6Address ref1;
    OSAL::Network::IPV6Address ref2({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::Network::IPV6Address ref3(ipAddress);
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(ref1 == target);
    EXPECT_TRUE(ref1 == ref2);
    EXPECT_FALSE(ref1 == ref3);
    EXPECT_FALSE(ref2 == target);
    EXPECT_TRUE(ref2 == ref1);
    EXPECT_FALSE(ref2 == ref3);
    EXPECT_TRUE(ref3 == target);
    EXPECT_FALSE(ref3 == ref1);
    EXPECT_FALSE(ref3 == ref2);
}

TEST_FIXTURE(IPV6AddressTest, OperatorNotEqual)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target(ipAddress);
    OSAL::Network::IPV6Address ref1;
    OSAL::Network::IPV6Address ref2({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::Network::IPV6Address ref3(ipAddress);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(ref1 != target);
    EXPECT_FALSE(ref1 != ref2);
    EXPECT_TRUE(ref1 != ref3);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref2 != ref1);
    EXPECT_TRUE(ref2 != ref3);
    EXPECT_FALSE(ref3 != target);
    EXPECT_TRUE(ref3 != ref1);
    EXPECT_TRUE(ref3 != ref2);
}

TEST_FIXTURE(IPV6AddressTest, PrintTo)
{
    OSAL::Network::IPV6Address ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    string expected = "102:304:506:708:90a:b0c:d0e:f10";
    ostringstream stream;
    ipAddress.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToNonMember)
{
    OSAL::Network::IPV6Address ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("102:304:506:708:90a:b0c:d0e:f10", stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToAddressUnspecified)
{
    OSAL::Network::IPV6Address ipAddress({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("::", stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToLocalhostAddress)
{
    OSAL::Network::IPV6Address ipAddress({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("::1", stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToAddressTrailingZeroSequence)
{
    OSAL::Network::IPV6Address ipAddress({1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("100::", stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToMultipleZeroSequences1)
{
    OSAL::Network::IPV6Address ipAddress({1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("100:0:100::1", stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToMultipleZeroSequences2)
{
    OSAL::Network::IPV6Address ipAddress({1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("100::100:0:0:1", stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToShortened)
{
    OSAL::Network::IPV6Address ipAddress({0xFE, 0x3D, 0x40, 0x56, 0x89, 0x78, 0x00, 0x00, 0xE1, 0x33, 0x19, 0xD7, 0x2C, 0x09, 0x29, 0x0E});
    ostringstream stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ("fe3d:4056:8978:0:e133:19d7:2c09:290e", stream.str());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
