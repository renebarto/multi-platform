#include "unit-test-c++/UnitTestC++.h"

#include <osal/MACAddress.h>
#include <osal/IPV4EndPoint.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test
{

class MACAddressTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void MACAddressTest::SetUp()
{

}

void MACAddressTest::TearDown()
{

}

TEST_SUITE(osal)
{

TEST_FIXTURE(MACAddressTest, Constructor)
{
    OSAL::Network::MACAddress target;
    OSAL::ByteArray reference(OSAL::Network::MACAddress::AddressSize);
    const OSAL::String expected = _("00-00-00-00-00-00");
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}


TEST_FIXTURE(MACAddressTest, ConstructorByteArray)
{
    OSAL::ByteArray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5, 6});
    const OSAL::String expected = _("01-02-03-04-05-06");
    EXPECT_TRUE(macAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, ConstructorByteArrayOffset)
{
    OSAL::ByteArray reference({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
    const OSAL::String expected = _("01-02-03-04-05-06");
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, CreateByteArray)
{
    OSAL::ByteArray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5, 6});
    const OSAL::String expected = _("01-02-03-04-05-06");
    EXPECT_TRUE(macAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, CreateByteArrayOffset)
{
    OSAL::ByteArray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
    const OSAL::String expected = _("01-02-03-04-05-06");
    EXPECT_TRUE(macAddress == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, OperatorIndex)
{
    OSAL::ByteArray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(macAddress);
    EXPECT_EQ(macAddress[size_t{0}], target[0]);
    EXPECT_EQ(macAddress[size_t{1}], target[1]);
    EXPECT_EQ(macAddress[size_t{2}], target[2]);
    EXPECT_EQ(macAddress[size_t{3}], target[3]);
    EXPECT_EQ(macAddress[size_t{4}], target[4]);
    EXPECT_EQ(macAddress[size_t{5}], target[5]);
}

TEST_FIXTURE(MACAddressTest, Parse)
{
    const OSAL::String text = _("FF-FE-FD-01-02-03");
    OSAL::ByteArray macAddress({255, 254, 253, 1, 2, 3});
    OSAL::Network::MACAddress expected(macAddress);
    const OSAL::Network::MACAddress & actual = OSAL::Network::MACAddress::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(MACAddressTest, ParseInvalid)
{
    const OSAL::String text1 = _("00-11-22-AA-BB");
    const OSAL::String text2 = _("XX-YY-ZZ-AA-BB-CC");

    EXPECT_THROW(OSAL::Network::MACAddress::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::Network::MACAddress::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(MACAddressTest, TryParse)
{
    const OSAL::String text = _("FF-FE-FD-01-02-03");
    OSAL::ByteArray macAddress({255, 254, 253, 1, 2, 3});
    OSAL::Network::MACAddress actual;
    OSAL::Network::MACAddress expected(macAddress);
    EXPECT_TRUE(OSAL::Network::MACAddress::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(MACAddressTest, TryParseInvalid)
{
    const OSAL::String text1 = _("00-11-22-AA-BB");
    const OSAL::String text2 = _("XX-YY-ZZ-AA-BB-CC");
    OSAL::Network::MACAddress macAddress;
    EXPECT_FALSE(OSAL::Network::MACAddress::TryParse(text1, macAddress));
    EXPECT_FALSE(OSAL::Network::MACAddress::TryParse(text2, macAddress));
}

TEST_FIXTURE(MACAddressTest, OperatorEqualAddress)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(ipAddress);
    OSAL::Network::MACAddress ref1;
    OSAL::Network::MACAddress ref2({0, 0, 0, 0, 0, 0});
    OSAL::Network::MACAddress ref3(ipAddress);
    OSAL::Network::IPV4EndPoint ref4(IPV4Address({0x00, 0x01, 0x02, 0x03}), 0xFFFF);
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

TEST_FIXTURE(MACAddressTest, OperatorNotEqualAddress)
{
    OSAL::ByteArray ipAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(ipAddress);
    OSAL::Network::MACAddress ref1;
    OSAL::Network::MACAddress ref2({0, 0, 0, 0, 0, 0});
    OSAL::Network::MACAddress ref3(ipAddress);
    OSAL::Network::IPV4EndPoint ref4(IPV4Address({0x00, 0x01, 0x02, 0x03}), 0xFFFF);
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

TEST_FIXTURE(MACAddressTest, OperatorEqual)
{
    OSAL::ByteArray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(macAddress);
    OSAL::Network::MACAddress ref1;
    OSAL::Network::MACAddress ref2({0, 0, 0, 0, 0, 0});
    OSAL::Network::MACAddress ref3(macAddress);
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

TEST_FIXTURE(MACAddressTest, OperatorNotEqual)
{
    OSAL::ByteArray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(macAddress);
    OSAL::Network::MACAddress ref1;
    OSAL::Network::MACAddress ref2({0, 0, 0, 0, 0, 0});
    OSAL::Network::MACAddress ref3(macAddress);
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

TEST_FIXTURE(MACAddressTest, PrintTo)
{
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5, 6});
    const OSAL::String expected = _("01-02-03-04-05-06");
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, PrintToNonMember)
{
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5, 6});
    const OSAL::String expected = _("01-02-03-04-05-06");
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(target, stream);
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
