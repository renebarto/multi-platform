#include <unittest-cpp/UnitTestC++.h>

#include "osal/IPV4Address.h"
#include "osal/Exception.h"

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

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

TEST_SUITE(osal)
{

TEST_FIXTURE(IPV4AddressTest, Constructor)
{
    OSAL::Network::IPV4Address target;
    OSAL::bytearray reference({0, 0, 0, 0});
    const string expected = "0.0.0.0";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorCopy)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target(ipAddress);
    const string expected = "1.2.3.4";
    EXPECT_TRUE(ipAddress == target);
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArray)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target({1, 2, 3, 4});
    const string expected = "1.2.3.4";
    EXPECT_TRUE(ipAddress == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArrayOffset)
{
    OSAL::bytearray reference({1, 2, 3, 4});
    OSAL::Network::IPV4Address target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
    const string expected = "1.2.3.4";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorUInt32)
{
    OSAL::bytearray expected({255, 254, 253, 252});
    OSAL::Network::IPV4Address target(0xFCFDFEFF);
    OSAL::bytearray actual = target.GetBytes();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ConstructorInAddr)
{
    in_addr ipAddress = {1, 2, 3, 4};
    OSAL::Network::IPV4Address target(ipAddress);
    const string expected = "1.2.3.4";
    EXPECT_EQ(OSAL::bytearray({1, 2, 3, 4}), target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, OperatorIndex)
{
    OSAL::bytearray ipAddress({255, 254, 253, 252});
    OSAL::Network::IPV4Address target(ipAddress);
    EXPECT_EQ(ipAddress[size_t{0}], target[0]);
    EXPECT_EQ(ipAddress[size_t{1}], target[1]);
    EXPECT_EQ(ipAddress[size_t{2}], target[2]);
    EXPECT_EQ(ipAddress[size_t{3}], target[3]);
}

TEST_FIXTURE(IPV4AddressTest, GetUInt32)
{
    const string text = "255.254.253.252";
    OSAL::Network::IPV4Address ipAddress({255, 254, 253, 252});
    uint32_t expected = 0xFCFDFEFF;
    uint32_t actual = ipAddress.GetUInt32();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, SetUInt32)
{
    OSAL::Network::IPV4Address ipAddress;
    uint32_t expected = 0xFFFEFDFC;
    ipAddress.SetUInt32(expected);
    uint32_t actual = ipAddress.GetUInt32();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseNumeric)
{
    const string text = "255.254.253.252";
    OSAL::bytearray ipAddress({255, 254, 253, 252});
    OSAL::Network::IPV4Address expected(ipAddress);
    const OSAL::Network::IPV4Address & actual = OSAL::Network::IPV4Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseHostName)
{
    const string text = "localhost";
    OSAL::bytearray ipAddress({127, 0, 0, 1});
    OSAL::Network::IPV4Address expected(ipAddress);
    const OSAL::Network::IPV4Address & actual = OSAL::Network::IPV4Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseBroadcast)
{
    const string text1 = "255.255.255.255";

    EXPECT_EQ(OSAL::Network::IPV4Address({255, 255, 255, 255}), OSAL::Network::IPV4Address::Parse(text1));
}

TEST_FIXTURE(IPV4AddressTest, ParseInvalid)
{
    const string text1 = "256.255.255.255";
    const string text2 = "255.255.255.255.255";

    EXPECT_THROW(OSAL::Network::IPV4Address::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::Network::IPV4Address::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(IPV4AddressTest, TryParse)
{
    const string text = "255.254.253.252";
    OSAL::bytearray ipAddress({255, 254, 253, 252});
    OSAL::Network::IPV4Address actual;
    OSAL::Network::IPV4Address expected(ipAddress);
    EXPECT_TRUE(OSAL::Network::IPV4Address::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, TryParseInvalid)
{
    const string text1 = "256.255.255.255";
    const string text2 = "255.255.255.255.255";
    OSAL::Network::IPV4Address ipAddress;
    EXPECT_FALSE(OSAL::Network::IPV4Address::TryParse(text1, ipAddress));
    EXPECT_FALSE(OSAL::Network::IPV4Address::TryParse(text2, ipAddress));
}

TEST_FIXTURE(IPV4AddressTest, OperatorEqual)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target(ipAddress);
    OSAL::Network::IPV4Address ref1;
    OSAL::Network::IPV4Address ref2({0, 0, 0, 0});
    OSAL::Network::IPV4Address ref3(ipAddress);
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

TEST_FIXTURE(IPV4AddressTest, OperatorNotEqual)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target(ipAddress);
    OSAL::Network::IPV4Address ref1;
    OSAL::Network::IPV4Address ref2({0, 0, 0, 0});
    OSAL::Network::IPV4Address ref3(ipAddress);
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

TEST_FIXTURE(IPV4AddressTest, PrintTo)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    string expected = "1.2.3.4";
    ostringstream stream;
    ipAddress.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, PrintToNonMember)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    ostringstream stream;
    PrintTo(stream, ipAddress);
    EXPECT_EQ("1.2.3.4", stream.str());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Network
} // namespace OSAL
