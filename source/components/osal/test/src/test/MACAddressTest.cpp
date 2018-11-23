#include <unittest-cpp/UnitTestC++.h>

#include <osal/MACAddress.h>
#include <osal/IPV4EndPoint.h>
#include <osal/Exception.h>

using namespace std;

namespace OSAL {
namespace Network {
namespace Test {

class MACAddressTest
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

bool MACAddressTest::_hadAssertion = false;
bool MACAddressTest::_expression = false;
const char * MACAddressTest::_expressionText = nullptr;
const char * MACAddressTest::_file = nullptr;
int MACAddressTest::_line = 0;
const char * MACAddressTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(MACAddressTest, Constructor)
{
    OSAL::Network::MACAddress target;
    OSAL::bytearray reference(OSAL::Network::MACAddress::AddressSize);
    const string expected = "00-00-00-00-00-00";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, ConstructorByteArray)
{
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5, 6});
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, ConstructorByteArrayOffset)
{
    OSAL::bytearray reference({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(reference == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, ConstructorByteArrayOffsetTooSmall)
{
    OSAL::bytearray reference({1, 2, 3, 4, 5, 0});
    EXPECT_FALSE(_hadAssertion);
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5}, 0);
    EXPECT_TRUE(_hadAssertion);

    OSAL::bytearray expectedAddress;
    target.GetBytes().get(0, expectedAddress, reference.size());
    EXPECT_EQ(reference, expectedAddress);
}

TEST_FIXTURE(MACAddressTest, ConstructorCopy)
{
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress other({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(other);
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, Assign)
{
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress other({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target;

    target = other;
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target.GetBytes());
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, CreateDefault)
{
    OSAL::Network::EndPointPtr target = OSAL::Network::MACAddress::Create();
    ASSERT_NOT_NULL(target);
    OSAL::bytearray reference(OSAL::Network::MACAddress::AddressSize);
    const string expected = "00-00-00-00-00-00";
    EXPECT_EQ(reference, target->GetBytes());
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, CreateFromEndPointMACAddress)
{
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress other({1, 2, 3, 4, 5, 6});

    OSAL::Network::EndPointPtr target = OSAL::Network::MACAddress::Create(other);
    ASSERT_NOT_NULL(target);
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target->GetBytes());
    ostringstream stream;
    target->PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, CreateFromEndPointIPV4EndPoint)
{
    OSAL::Network::IPV4EndPoint other(12345);

    OSAL::Network::EndPointPtr target = OSAL::Network::MACAddress::Create(other);
    EXPECT_NULL(target);
}

TEST_FIXTURE(MACAddressTest, OperatorIndex)
{
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
    OSAL::Network::MACAddress target(macAddress);
    EXPECT_EQ(macAddress[size_t{0}], target[0]);
    EXPECT_EQ(macAddress[size_t{1}], target[1]);
    EXPECT_EQ(macAddress[size_t{2}], target[2]);
    EXPECT_EQ(macAddress[size_t{3}], target[3]);
    EXPECT_EQ(macAddress[size_t{4}], target[4]);
    EXPECT_EQ(macAddress[size_t{5}], target[5]);

    EXPECT_FALSE(_hadAssertion);
    EXPECT_EQ(macAddress[OSAL::Network::MACAddress::AddressSize - 1], target[OSAL::Network::MACAddress::AddressSize]);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(MACAddressTest, Parse)
{
    const string text = "FF-FE-FD-01-02-03";
    OSAL::bytearray macAddress({255, 254, 253, 1, 2, 3});
    OSAL::Network::MACAddress expected(macAddress);
    const OSAL::Network::MACAddress & actual = OSAL::Network::MACAddress::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(MACAddressTest, ParseInvalid)
{
    const string text1 = "00-11-22-AA-BB";
    const string text2 = "XX-YY-ZZ-AA-BB-CC";

    EXPECT_THROW(OSAL::Network::MACAddress::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(OSAL::Network::MACAddress::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(MACAddressTest, TryParse)
{
    const string text = "FF-FE-FD-01-02-03";
    OSAL::bytearray macAddress({255, 254, 253, 1, 2, 3});
    OSAL::Network::MACAddress actual;
    OSAL::Network::MACAddress expected(macAddress);
    EXPECT_TRUE(OSAL::Network::MACAddress::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(MACAddressTest, TryParseInvalid)
{
    const string text1 = "00-11-22-AA-BB";
    const string text2 = "XX-YY-ZZ-AA-BB-CC";
    OSAL::Network::MACAddress macAddress;
    EXPECT_FALSE(OSAL::Network::MACAddress::TryParse(text1, macAddress));
    EXPECT_FALSE(OSAL::Network::MACAddress::TryParse(text2, macAddress));
}

TEST_FIXTURE(MACAddressTest, OperatorEqualAddress)
{
    OSAL::bytearray ipAddress({1, 2, 3, 4, 5, 6});
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
    OSAL::bytearray ipAddress({1, 2, 3, 4, 5, 6});
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
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
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
    OSAL::bytearray macAddress({1, 2, 3, 4, 5, 6});
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
    const string expected = "01-02-03-04-05-06";
    ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(MACAddressTest, PrintToNonMember)
{
    OSAL::Network::MACAddress target({1, 2, 3, 4, 5, 6});
    const string expected = "01-02-03-04-05-06";
    ostringstream stream;
    PrintTo(stream, target);
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(osal)
} // namespace Test
} // namespace Network
} // namespace OSAL
