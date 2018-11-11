#include "osal/exports.h"
#if defined(LINUX)
#include <unittest-cpp/UnitTestC++.h>

#include <osal/Exception.h>
#include <osal/linux/DomainSocketAddress.h>
#include <osal/IPV4EndPoint.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

class DomainSocketAddressTest
    : public UnitTestCpp::TestFixture
{
public:
    static void AssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
    {
        _hadAssertion = true;
        _expression = expression;
        _expressionText = expressionText;
        _file = file;
        _line = line;
        _func = func;
    }
    static bool _hadAssertion;
    static bool _expression;
    static const char * _expressionText;
    static const char * _file;
    static int _line;
    static const char * _func;

    void SetUp() override
    {
        _hadAssertion = false;
        _expression = false;
        _expressionText = nullptr;
        _file = nullptr;
        _line = 0;
        _func = nullptr;
        SetAssertionHandler(AssertionHandler);
    }
};

bool DomainSocketAddressTest::_hadAssertion = false;
bool DomainSocketAddressTest::_expression = false;
const char * DomainSocketAddressTest::_expressionText = nullptr;
const char * DomainSocketAddressTest::_file = nullptr;
int DomainSocketAddressTest::_line = 0;
const char * DomainSocketAddressTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(DomainSocketAddressTest, Constructor)
{
    OSAL::Network::DomainSocketAddress target;
    OSAL::ByteArray reference(OSAL::Network::DomainSocketAddress::AddressSize);
    const std::string expected = "";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, ConstructorByteArray)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    const std::string expected = "123456";
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, ConstructorByteArrayOffset)
{
    OSAL::ByteArray reference({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target({0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36}, 1);
    const std::string expected = "123456";
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, reference.Size());
    EXPECT_EQ(reference, expectedAddress);
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, ConstructorByteArrayOffsetTooLarge)
{
    OSAL::ByteArray reference(OSAL::Network::DomainSocketAddress::AddressSize);
    OSAL::ByteArray data(OSAL::Network::DomainSocketAddress::AddressSize + 1);

    EXPECT_FALSE(_hadAssertion);
    OSAL::Network::DomainSocketAddress target(data, 0);
    EXPECT_TRUE(_hadAssertion);

    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, reference.Size());
    EXPECT_EQ(reference, expectedAddress);
}

TEST_FIXTURE(DomainSocketAddressTest, ConstructorCopy)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress other(domainAddress);
    OSAL::Network::DomainSocketAddress target(other);
    const std::string expected = "123456";
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, Assign)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress other(domainAddress);
    OSAL::Network::DomainSocketAddress target;
    target = other;
    const std::string expected = "123456";
    OSAL::ByteArray expectedAddress;
    target.GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, CreateDefault)
{
    OSAL::Network::EndPointPtr target = OSAL::Network::DomainSocketAddress::Create();
    ASSERT_NOT_NULL(target);
    OSAL::ByteArray reference(OSAL::Network::DomainSocketAddress::AddressSize);
    const std::string expected = "";
    EXPECT_EQ(reference, target->GetBytes());
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, CreateFromEndPointDomainSocketAddress)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress other(domainAddress);
    OSAL::Network::EndPointPtr target = OSAL::Network::DomainSocketAddress::Create(other);
    ASSERT_NOT_NULL(target);
    const std::string expected = "123456";
    OSAL::ByteArray expectedAddress;
    target->GetBytes().Get(0, expectedAddress, domainAddress.Size());
    EXPECT_EQ(domainAddress, expectedAddress);
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, CreateFromEndPointIPV4EndPoint)
{
    OSAL::Network::IPV4EndPoint other(12345);
    OSAL::Network::EndPointPtr target = OSAL::Network::DomainSocketAddress::Create(other);
    EXPECT_NULL(target);
}

TEST_FIXTURE(DomainSocketAddressTest, CreateFromString)
{
    const std::string text = "/abc/def";
    OSAL::ByteArray domainAddress(text);
    OSAL::Network::DomainSocketAddress expected(domainAddress);
    OSAL::Network::EndPointPtr actual = OSAL::Network::DomainSocketAddress::Create(text);
    EXPECT_EQ(expected, *actual);
}

TEST_FIXTURE(DomainSocketAddressTest, OperatorIndex)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    EXPECT_EQ(domainAddress[size_t{0}], target[0]);
    EXPECT_EQ(domainAddress[size_t{1}], target[1]);
    EXPECT_EQ(domainAddress[size_t{2}], target[2]);
    EXPECT_EQ(domainAddress[size_t{3}], target[3]);
    EXPECT_EQ(domainAddress[size_t{4}], target[4]);
    EXPECT_EQ(domainAddress[size_t{5}], target[5]);
    EXPECT_EQ(uint8_t {0}, target[6]);

    EXPECT_FALSE(_hadAssertion);
    EXPECT_EQ(target[OSAL::Network::DomainSocketAddress::AddressSize - 1],
              target[OSAL::Network::DomainSocketAddress::AddressSize]);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(DomainSocketAddressTest, Parse)
{
    const std::string text = "/abc/def";
    OSAL::ByteArray domainAddress(text);
    OSAL::Network::DomainSocketAddress expected(domainAddress);
    const OSAL::Network::DomainSocketAddress & actual = OSAL::Network::DomainSocketAddress::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DomainSocketAddressTest, ParseInvalid)
{
    const std::string textTooLong = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";

    EXPECT_THROW(OSAL::Network::DomainSocketAddress::Parse(textTooLong), OSAL::ArgumentException);
}

TEST_FIXTURE(DomainSocketAddressTest, TryParse)
{
    const std::string text = "/abc/def";
    OSAL::ByteArray domainAddress(text);
    OSAL::Network::DomainSocketAddress actual;
    OSAL::Network::DomainSocketAddress expected(domainAddress);
    EXPECT_TRUE(OSAL::Network::DomainSocketAddress::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DomainSocketAddressTest, TryParseInvalid)
{
    const std::string textTooLong = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";

    OSAL::Network::DomainSocketAddress domainAddress;
    EXPECT_FALSE(OSAL::Network::DomainSocketAddress::TryParse(textTooLong, domainAddress));
}

TEST_FIXTURE(DomainSocketAddressTest, OperatorEqualEndPoint)
{
    OSAL::ByteArray ipAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(ipAddress);
    const OSAL::Network::EndPoint &ref1 = IPV4EndPoint::Parse("127.0.0.1");
    OSAL::Network::DomainSocketAddress ref2(OSAL::ByteArray({0, 0, 0, 0, 0, 0}));
    OSAL::Network::DomainSocketAddress ref3(ipAddress);
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(ref2 == target);
    EXPECT_FALSE(ref2 == ref1);
    EXPECT_FALSE(ref2 == ref3);
    EXPECT_TRUE(ref3 == target);
    EXPECT_FALSE(ref3 == ref1);
    EXPECT_FALSE(ref3 == ref2);
}

TEST_FIXTURE(DomainSocketAddressTest, OperatorNotEqualEndPoint)
{
    OSAL::ByteArray ipAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(ipAddress);
    const OSAL::Network::EndPoint &ref1 = IPV4EndPoint::Parse("127.0.0.1");
    OSAL::Network::DomainSocketAddress ref2(OSAL::ByteArray({0, 0, 0, 0, 0, 0}));
    OSAL::Network::DomainSocketAddress ref3(ipAddress);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(ref2 != target);
    EXPECT_TRUE(ref2 != ref1);
    EXPECT_TRUE(ref2 != ref3);
    EXPECT_FALSE(ref3 != target);
    EXPECT_TRUE(ref3 != ref1);
    EXPECT_TRUE(ref3 != ref2);
}

TEST_FIXTURE(DomainSocketAddressTest, OperatorEqual)
{
    OSAL::ByteArray domainAddress({0x31, 0x32, 0x33, 0x34, 0x35, 0x36});
    OSAL::Network::DomainSocketAddress target(domainAddress);
    OSAL::Network::DomainSocketAddress ref1;
    OSAL::Network::DomainSocketAddress ref2(OSAL::ByteArray({0, 0, 0, 0, 0, 0}));
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
    OSAL::Network::DomainSocketAddress ref2(OSAL::ByteArray({0, 0, 0, 0, 0, 0}));
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
    const std::string path = "/tmp/socket";
    OSAL::Network::DomainSocketAddress target(path);
    const std::string expected = path;
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(DomainSocketAddressTest, PrintToNonMember)
{
    const std::string path = "/tmp/socket";
    OSAL::Network::DomainSocketAddress target(path);
    const std::string expected = path;
    ostringstream stream;
    PrintTo(target, stream);
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
#endif
