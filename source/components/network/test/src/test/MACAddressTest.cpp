#include "unit-test-c++/UnitTestC++.h"
#include "network/MACAddress.h"
#include "core/Core.h"

using namespace std;

namespace Network
{

namespace Test
{

class MACAddressTest : public UnitTestCpp::TestFixture
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

TEST_FIXTURE(MACAddressTest, Constructor)
{
    MACAddress target;
    Core::ByteArray reference(MACAddress::AddressSize);
    const string expected = "00-00-00-00-00-00";
    EXPECT_TRUE(reference == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}


TEST_FIXTURE(MACAddressTest, ConstructorByteArray)
{
    Core::ByteArray macAddress({ 1, 2, 3, 4, 5, 6 });
    MACAddress target({ 1, 2, 3, 4, 5, 6 });
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(MACAddressTest, ConstructorByteArrayOffset)
{
    Core::ByteArray reference({ 1, 2, 3, 4, 5, 6 });
    MACAddress target({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 1);
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(reference == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(MACAddressTest, CreateByteArray)
{
    Core::ByteArray macAddress({ 1, 2, 3, 4, 5, 6 });
    MACAddress target({ 1, 2, 3, 4, 5, 6 });
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(MACAddressTest, CreateByteArrayOffset)
{
    Core::ByteArray macAddress({ 1, 2, 3, 4, 5, 6 });
    MACAddress target({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 1);
    const string expected = "01-02-03-04-05-06";
    EXPECT_TRUE(macAddress == target.GetBytes());
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(MACAddressTest, OperatorIndex)
{
    Core::ByteArray macAddress({ 1, 2, 3, 4, 5, 6 });
    MACAddress target(macAddress);
    EXPECT_EQ(macAddress[0], target[0]);
    EXPECT_EQ(macAddress[1], target[1]);
    EXPECT_EQ(macAddress[2], target[2]);
    EXPECT_EQ(macAddress[3], target[3]);
    EXPECT_EQ(macAddress[4], target[4]);
    EXPECT_EQ(macAddress[5], target[5]);
}

TEST_FIXTURE(MACAddressTest, Parse)
{
    const string text = "FF-FE-FD-01-02-03";
    Core::ByteArray macAddress({ 255, 254, 253, 1, 2, 3 });
    MACAddress expected(macAddress);
    const MACAddress & actual = MACAddress::Parse(text);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(MACAddressTest, ParseInvalid)
{
    const string text1 = "00-11-22-AA-BB";
    const string text2 = "XX-YY-ZZ-AA-BB-CC";

    EXPECT_THROW(MACAddress::Parse(text1), OSAL::ArgumentException);
    EXPECT_THROW(MACAddress::Parse(text2), OSAL::ArgumentException);
}

TEST_FIXTURE(MACAddressTest, TryParse)
{
    const string text = "FF-FE-FD-01-02-03";
    Core::ByteArray macAddress({ 255, 254, 253, 1, 2, 3 });
    MACAddress actual;
    MACAddress expected(macAddress);
    EXPECT_TRUE(MACAddress::TryParse(text, actual));
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(MACAddressTest, TryParseInvalid)
{
    const string text1 = "00-11-22-AA-BB";
    const string text2 = "XX-YY-ZZ-AA-BB-CC";
    MACAddress macAddress;
    EXPECT_FALSE(MACAddress::TryParse(text1, macAddress));
    EXPECT_FALSE(MACAddress::TryParse(text2, macAddress));
}

TEST_FIXTURE(MACAddressTest, OperatorNotEqual)
{
    Core::ByteArray macAddress({ 1, 2, 3, 4, 5, 6 });
    MACAddress target(macAddress);
    MACAddress ref1;
    MACAddress ref2({ 0, 0, 0, 0, 0, 0 });
    MACAddress ref3(macAddress);
    EXPECT_TRUE(target != ref1);
    EXPECT_TRUE(target != ref2);
    EXPECT_FALSE(target != ref3);
    EXPECT_TRUE(ref2 != target);
    EXPECT_FALSE(ref3 != target);
}

TEST_FIXTURE(MACAddressTest, OperatorEqual)
{
    Core::ByteArray macAddress({ 1, 2, 3, 4, 5, 6 });
    MACAddress target(macAddress);
    MACAddress ref1;
    MACAddress ref2({ 0, 0, 0, 0, 0, 0 });
    MACAddress ref3(macAddress);
    EXPECT_FALSE(target == ref1);
    EXPECT_FALSE(target == ref2);
    EXPECT_TRUE(target == ref3);
    EXPECT_FALSE(ref2 == target);
    EXPECT_TRUE(ref3 == target);
}

TEST_FIXTURE(MACAddressTest, ToString)
{
    MACAddress target({ 1, 2, 3, 4, 5, 6 });
    const string expected = "01-02-03-04-05-06";
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(MACAddressTest, PrintTo)
{
    MACAddress target({ 1, 2, 3, 4, 5, 6 });
    const string expected = "01-02-03-04-05-06";
    ostringstream stream;
    PrintTo(target, stream);
    EXPECT_EQ(expected, stream.str());
}

} // namespace Test

} // namespace Network
