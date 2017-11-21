#include "unit-test-c++/UnitTestC++.h"
#include <osal/IPV6Address.h>
#include <osal/MACAddress.h>
#include "core/Core.h"

using namespace std;

namespace OSAL {
namespace Network {
namespace Test
{

class IPV6AddressTest
    : public UnitTestCpp::TestFixture
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
    OSAL::ByteArray reference({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    const OSAL::String expected = _("::");
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorCopy)
{
    OSAL::Network::IPV6Address ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target(ipAddress);
    const OSAL::String expected = _("102:304:506:708:90a:b0c:d0e:f10");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorByteArray)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    const OSAL::String expected = _("102:304:506:708:90a:b0c:d0e:f10");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, ConstructorByteArrayOffset)
{
    OSAL::ByteArray reference({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 1);
    const OSAL::String expected = _("102:304:506:708:90a:b0c:d0e:f10");
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, CreateByteArray)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    const OSAL::String expected = _("102:304:506:708:90a:b0c:d0e:f10");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, CreateByteArrayOffset)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 1);
    const OSAL::String expected = _("102:304:506:708:90a:b0c:d0e:f10");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, CreateUInt8Array)
{
    OSAL::ByteArray expected({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    uint8_t address[16] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240};
    OSAL::Network::IPV6Address target(address);
    OSAL::ByteArray actual = target.GetBytes();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, OperatorIndex)
{
    OSAL::ByteArray ipAddress({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::Network::IPV6Address target(ipAddress);
    EXPECT_EQ(ipAddress[0], target[0]);
    EXPECT_EQ(ipAddress[1], target[1]);
    EXPECT_EQ(ipAddress[2], target[2]);
    EXPECT_EQ(ipAddress[3], target[3]);
    EXPECT_EQ(ipAddress[4], target[4]);
    EXPECT_EQ(ipAddress[5], target[5]);
    EXPECT_EQ(ipAddress[6], target[6]);
    EXPECT_EQ(ipAddress[7], target[7]);
    EXPECT_EQ(ipAddress[8], target[8]);
    EXPECT_EQ(ipAddress[9], target[9]);
    EXPECT_EQ(ipAddress[10], target[10]);
    EXPECT_EQ(ipAddress[11], target[11]);
    EXPECT_EQ(ipAddress[12], target[12]);
    EXPECT_EQ(ipAddress[13], target[13]);
    EXPECT_EQ(ipAddress[14], target[14]);
    EXPECT_EQ(ipAddress[15], target[15]);
}

TEST_FIXTURE(IPV6AddressTest, GetData)
{
    OSAL::Network::IPV6Address ipAddress(
        {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::ByteArray expected({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::ByteArray actual = ipAddress.GetData();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, SetData)
{
    OSAL::Network::IPV6Address ipAddress;
    OSAL::ByteArray expected({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    ipAddress.SetData(expected);
    OSAL::ByteArray actual = ipAddress.GetData();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ParseNumeric)
{
    const OSAL::String text = _(_("fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0"));
    OSAL::ByteArray ipAddress({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::Network::IPV6Address expected(ipAddress);
    const OSAL::Network::IPV6Address & actual = OSAL::Network::IPV6Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ParseHostName)
{
    const OSAL::String text = _("localhost");
    OSAL::ByteArray ipAddress({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    OSAL::Network::IPV6Address expected(ipAddress);
    const OSAL::Network::IPV6Address & actual = OSAL::Network::IPV6Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, ParseLocalhost)
{
    const OSAL::String text = _("::1");

    EXPECT_EQ(OSAL::Network::IPV6Address({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}),
              OSAL::Network::IPV6Address::Parse(text));
}

TEST_FIXTURE(IPV6AddressTest, ParseBroadcast)
{
    const OSAL::String text = _("ff02::1");

    EXPECT_EQ(OSAL::Network::IPV6Address({255, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}),
              OSAL::Network::IPV6Address::Parse(text));
}

TEST_FIXTURE(IPV6AddressTest, ParseInvalid)
{
    const OSAL::String text1 = _("ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0");
    const OSAL::String text2 = _("fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0");

    EXPECT_THROW(OSAL::Network::IPV6Address::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::Network::IPV6Address::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(IPV6AddressTest, TryParse)
{
    const OSAL::String text = _("fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0");
    OSAL::ByteArray ipAddress({255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240});
    OSAL::Network::IPV6Address actual;
    OSAL::Network::IPV6Address expected(ipAddress);
    EXPECT_TRUE(OSAL::Network::IPV6Address::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV6AddressTest, TryParseInvalid)
{
    const OSAL::String text1 = _("ffffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0");
    const OSAL::String text2 = _("fffe:fdfc:fbfa:f9f8:f7f6:f5f4:f3f2:f1f0:0");
    OSAL::Network::IPV6Address ipAddress;
    EXPECT_FALSE(OSAL::Network::IPV6Address::TryParse(text1, ipAddress));
    EXPECT_FALSE(OSAL::Network::IPV6Address::TryParse(text2, ipAddress));
}

TEST_FIXTURE(IPV6AddressTest, OperatorEqualAddress)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target(ipAddress);
    OSAL::Network::IPV6Address ref1;
    OSAL::Network::IPV6Address ref2({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::Network::IPV6Address ref3(ipAddress);
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

TEST_FIXTURE(IPV6AddressTest, OperatorNotEqualAddress)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    OSAL::Network::IPV6Address target(ipAddress);
    OSAL::Network::IPV6Address ref1;
    OSAL::Network::IPV6Address ref2({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    OSAL::Network::IPV6Address ref3(ipAddress);
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

TEST_FIXTURE(IPV6AddressTest, OperatorEqual)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
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
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
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
    string expected = _("102:304:506:708:90a:b0c:d0e:f10");
    basic_ostringstream<OSAL::Char> stream;
    ipAddress.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToNonMember)
{
    OSAL::Network::IPV6Address ipAddress({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("102:304:506:708:90a:b0c:d0e:f10"), stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToAddressUnspecified)
{
    OSAL::Network::IPV6Address ipAddress({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("::"), stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToLocalhostAddress)
{
    OSAL::Network::IPV6Address ipAddress({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("::1"), stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToAddressTrailingZeroSequence)
{
    OSAL::Network::IPV6Address ipAddress({1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("100::"), stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToMultipleZeroSequences1)
{
    OSAL::Network::IPV6Address ipAddress({1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("100:0:100::1"), stream.str());
}

TEST_FIXTURE(IPV6AddressTest, PrintToMultipleZeroSequences2)
{
    OSAL::Network::IPV6Address ipAddress({1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("100::100:0:0:1"), stream.str());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
