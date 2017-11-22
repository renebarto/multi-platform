#include "unit-test-c++/UnitTestC++.h"

#include "osal/IPV4Address.h"

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
    OSAL::ByteArray reference({0, 0, 0, 0});
    const OSAL::String expected = _("0.0.0.0");
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorCopy)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target(ipAddress);
    const OSAL::String expected = _("1.2.3.4");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArray)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target({1, 2, 3, 4});
    const OSAL::String expected = _("1.2.3.4");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, ConstructorByteArrayOffset)
{
    OSAL::ByteArray reference({1, 2, 3, 4});
    OSAL::Network::IPV4Address target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
    const OSAL::String expected = _("1.2.3.4");
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, CreateByteArray)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target({1, 2, 3, 4});
    const OSAL::String expected = _("1.2.3.4");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, CreateByteArrayOffset)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4});
    OSAL::Network::IPV4Address target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
    const OSAL::String expected = _("1.2.3.4");
    EXPECT_TRUE(ipAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, CreateUInt32)
{
    OSAL::ByteArray expected({255, 254, 253, 252});
    OSAL::Network::IPV4Address target(0xFCFDFEFF);
    OSAL::ByteArray actual = target.GetBytes();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, OperatorIndex)
{
    OSAL::ByteArray ipAddress({255, 254, 253, 252});
    OSAL::Network::IPV4Address target(ipAddress);
    EXPECT_EQ(ipAddress[size_t{0}], target[0]);
    EXPECT_EQ(ipAddress[size_t{1}], target[1]);
    EXPECT_EQ(ipAddress[size_t{2}], target[2]);
    EXPECT_EQ(ipAddress[size_t{3}], target[3]);
}

TEST_FIXTURE(IPV4AddressTest, GetUInt32)
{
    const OSAL::String text = _("255.254.253.252");
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
    const OSAL::String text = _("255.254.253.252");
    OSAL::ByteArray ipAddress({255, 254, 253, 252});
    OSAL::Network::IPV4Address expected(ipAddress);
    const OSAL::Network::IPV4Address & actual = OSAL::Network::IPV4Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseHostName)
{
    const OSAL::String text = _("localhost");
    OSAL::ByteArray ipAddress({127, 0, 0, 1});
    OSAL::Network::IPV4Address expected(ipAddress);
    const OSAL::Network::IPV4Address & actual = OSAL::Network::IPV4Address::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, ParseBroadcast)
{
    const OSAL::String text1 = _("255.255.255.255");

    EXPECT_EQ(OSAL::Network::IPV4Address({255, 255, 255, 255}), OSAL::Network::IPV4Address::Parse(text1));
}

TEST_FIXTURE(IPV4AddressTest, ParseInvalid)
{
    const OSAL::String text1 = _("256.255.255.255");
    const OSAL::String text2 = _("255.255.255.255.255");

    EXPECT_THROW(OSAL::Network::IPV4Address::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::Network::IPV4Address::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(IPV4AddressTest, TryParse)
{
    const OSAL::String text = _("255.254.253.252");
    OSAL::ByteArray ipAddress({255, 254, 253, 252});
    OSAL::Network::IPV4Address actual;
    OSAL::Network::IPV4Address expected(ipAddress);
    EXPECT_TRUE(OSAL::Network::IPV4Address::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(IPV4AddressTest, TryParseInvalid)
{
    const OSAL::String text1 = _("256.255.255.255");
    const OSAL::String text2 = _("255.255.255.255.255");
    OSAL::Network::IPV4Address ipAddress;
    EXPECT_FALSE(OSAL::Network::IPV4Address::TryParse(text1, ipAddress));
    EXPECT_FALSE(OSAL::Network::IPV4Address::TryParse(text2, ipAddress));
}

TEST_FIXTURE(IPV4AddressTest, OperatorEqual)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4});
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
    OSAL::ByteArray ipAddress({1, 2, 3, 4});
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
    string expected = _("1.2.3.4");
    basic_ostringstream<OSAL::Char> stream;
    ipAddress.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(IPV4AddressTest, PrintToNonMember)
{
    OSAL::Network::IPV4Address ipAddress({1, 2, 3, 4});
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(ipAddress, stream);
    EXPECT_EQ(_("1.2.3.4"), stream.str());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Network
} // namespace OSAL
