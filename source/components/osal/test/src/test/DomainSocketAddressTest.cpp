#include "unit-test-c++/UnitTestC++.h"

#include "osal/exports.h"
#if defined(LINUX)
#include <osal/linux/DomainSocketAddress.h>
#include <osal/IPV4Address.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test
{

class DomainSocketAddressTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void DomainSocketAddressTest::SetUp()
{

}

void DomainSocketAddressTest::TearDown()
{

}

TEST_SUITE(osal)
{

TEST_FIXTURE(DomainSocketAddressTest, Constructor)
{
    OSAL::Network::DomainSocketAddress target;
    OSAL::ByteArray reference(OSAL::Network::DomainSocketAddress::AddressSize);
    const OSAL::String expected = "";
    EXPECT_TRUE(reference == target.GetBytes());
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}


TEST_FIXTURE(DomainSocketAddressTest, ConstructorByteArray)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    const OSAL::String expected = _("123456");
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, ConstructorByteArrayOffset)
{
    OSAL::ByteArray reference({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target({0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36}, 1);
    const OSAL::String expected = _("123456");
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, reference.Size());
    EXPECT_EQ(reference, expectedAddress);
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, CreateByteArray)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    const OSAL::String expected = _("123456");
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, CreateByteArrayOffset)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target({0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36}, 1);
    const OSAL::String expected = _("123456");
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, OperatorIndex)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    EXPECT_EQ(domainAddress[0], target[0]);
    EXPECT_EQ(domainAddress[1], target[1]);
    EXPECT_EQ(domainAddress[2], target[2]);
    EXPECT_EQ(domainAddress[3], target[3]);
    EXPECT_EQ(domainAddress[4], target[4]);
    EXPECT_EQ(domainAddress[5], target[5]);
}

TEST_FIXTURE(DomainSocketAddressTest, Parse)
{
    const OSAL::String text = _("/abc/def");
    OSAL::ByteArray domainAddress(text);
    OSAL::Network::DomainSocketAddress expected(domainAddress);
    const OSAL::Network::DomainSocketAddress & actual = OSAL::Network::DomainSocketAddress::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DomainSocketAddressTest, ParseInvalid)
{
    const OSAL::String textTooLong = _("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678");

    EXPECT_THROW(OSAL::Network::DomainSocketAddress::Parse(textTooLong), OSAL::ArgumentException);
}

TEST_FIXTURE(DomainSocketAddressTest, TryParse)
{
    const OSAL::String text = _("/abc/def");
    OSAL::ByteArray domainAddress(text);
    OSAL::Network::DomainSocketAddress actual;
    OSAL::Network::DomainSocketAddress expected(domainAddress);
    EXPECT_TRUE(OSAL::Network::DomainSocketAddress::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DomainSocketAddressTest, TryParseInvalid)
{
    const OSAL::String textTooLong = _("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678");

    OSAL::Network::DomainSocketAddress domainAddress;
    EXPECT_FALSE(OSAL::Network::DomainSocketAddress::TryParse(textTooLong, domainAddress));
}

TEST_FIXTURE(DomainSocketAddressTest, OperatorEqualAddress)
{
    OSAL::ByteArray ipAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(ipAddress);
    OSAL::Network::DomainSocketAddress ref1;
    OSAL::Network::DomainSocketAddress ref2(ByteArray({0, 0, 0, 0, 0, 0}));
    OSAL::Network::DomainSocketAddress ref3(ipAddress);
    OSAL::Network::IPV4Address ref4({0x00, 0x01, 0x02, 0x03});
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

TEST_FIXTURE(DomainSocketAddressTest, OperatorNotEqualAddress)
{
    OSAL::ByteArray ipAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(ipAddress);
    OSAL::Network::DomainSocketAddress ref1;
    OSAL::Network::DomainSocketAddress ref2(ByteArray({0, 0, 0, 0, 0, 0}));
    OSAL::Network::DomainSocketAddress ref3(ipAddress);
    OSAL::Network::IPV4Address ref4({0x00, 0x01, 0x02, 0x03});
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

TEST_FIXTURE(DomainSocketAddressTest, OperatorEqual)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    OSAL::Network::DomainSocketAddress ref1;
    OSAL::Network::DomainSocketAddress ref2(ByteArray({0, 0, 0, 0, 0, 0}));
    OSAL::Network::DomainSocketAddress ref3(domainAddress);
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

TEST_FIXTURE(DomainSocketAddressTest, OperatorNotEqual)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    OSAL::Network::DomainSocketAddress ref1;
    OSAL::Network::DomainSocketAddress ref2(ByteArray({0, 0, 0, 0, 0, 0}));
    OSAL::Network::DomainSocketAddress ref3(domainAddress);
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

TEST_FIXTURE(DomainSocketAddressTest, PrintTo)
{
    OSAL::Network::DomainSocketAddress target(_("/tmp/socket"));
    const OSAL::String expected = _("/tmp/socket");
    basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, PrintToNonMember)
{
    OSAL::Network::DomainSocketAddress target(_("/tmp/socket"));
    const OSAL::String expected = _("/tmp/socket");
    basic_ostringstream<OSAL::Char> stream;
    PrintTo(target, stream);
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
#endif