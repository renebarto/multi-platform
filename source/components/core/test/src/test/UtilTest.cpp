#include <unit-test-c++/UnitTestC++.h>

#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

class UtilTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

};

void UtilTest::SetUp()
{
}

void UtilTest::TearDown()
{
}

TEST_FIXTURE(UtilTest, NextPowerOfTwo)
{
    EXPECT_EQ(size_t {1}, Util::NextPowerOfTwo(0));
    EXPECT_EQ(size_t {1}, Util::NextPowerOfTwo(1));
    EXPECT_EQ(size_t {2}, Util::NextPowerOfTwo(2));
    EXPECT_EQ(size_t {4}, Util::NextPowerOfTwo(3));
    EXPECT_EQ(size_t {4}, Util::NextPowerOfTwo(4));
    EXPECT_EQ(size_t {8}, Util::NextPowerOfTwo(5));
    EXPECT_EQ(size_t {8}, Util::NextPowerOfTwo(6));
    EXPECT_EQ(size_t {8}, Util::NextPowerOfTwo(7));
    EXPECT_EQ(size_t {8}, Util::NextPowerOfTwo(8));
    EXPECT_EQ(size_t {16}, Util::NextPowerOfTwo(9));
    EXPECT_EQ(size_t {16}, Util::NextPowerOfTwo(10));
}

TEST_FIXTURE(UtilTest, MilliSecondsToTimespec)
{
    timespec expected = { 1000, 0 };
    timespec actual = Util::MilliSecondsToTimespec(1000000);
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 1, 0 };
    actual = Util::MilliSecondsToTimespec(1000);
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000000 };
    actual = Util::MilliSecondsToTimespec(1);
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);
}

TEST_FIXTURE(UtilTest, Base64Encode1)
{
    OSAL::ByteArray data({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::String expected = _("AAECAwQFBgc=");
    OSAL::String actual = Util::Base64Encode(data);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UtilTest, Base64Encode2)
{
    OSAL::ByteArray data({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::String expected = _("AAECAwQFBgc=");
    OSAL::String actual;
    Util::Base64Encode(data, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UtilTest, Base64DecodeString1)
{
    OSAL::ByteArray expected({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::String base64 = Util::Base64Encode(expected);
    OSAL::ByteArray actual = Util::Base64Decode(base64);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UtilTest, Base64DecodeString2)
{
    OSAL::ByteArray expected({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::String base64 = Util::Base64Encode(expected);
    OSAL::ByteArray actual;
    Util::Base64Decode(base64, actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UtilTest, Base64DecodeCharPtr1)
{
    OSAL::ByteArray expected({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::String base64 = Util::Base64Encode(expected);
    OSAL::ByteArray actual = Util::Base64Decode(base64.c_str());
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UtilTest, Base64DecodeCharPtr2)
{
    OSAL::ByteArray expected({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::String base64 = Util::Base64Encode(expected);
    OSAL::ByteArray actual;
    Util::Base64Decode(base64.c_str(), actual);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(UtilTest, CompareWithOffset)
{
    OSAL::ByteArray reference({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::ByteArray value1({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::ByteArray value2({0, 1, 2, 3, 4, 5, 6, 8});
    OSAL::ByteArray value3({0, 1, 2, 3, 4, 5, 6});
    OSAL::ByteArray value4({0, 1, 2, 3, 4, 5, 6, 7, 8});
    EXPECT_TRUE(Util::Compare(reference.Data(), 0, value1.Data(), 0, std::min(reference.Size(), value1.Size())));
    EXPECT_FALSE(Util::Compare(reference.Data(), 1, value1.Data(), 0, std::min(reference.Size() - 1, value1.Size())));
    EXPECT_FALSE(Util::Compare(reference.Data(), 0, value1.Data(), 1, std::min(reference.Size(), value1.Size() - 1)));
    EXPECT_FALSE(Util::Compare(reference.Data(), 0, value2.Data(), 0, std::min(reference.Size(), value2.Size())));
    EXPECT_TRUE(Util::Compare(reference.Data(), 0, value3.Data(), 0, std::min(reference.Size(), value3.Size())));
    EXPECT_TRUE(Util::Compare(reference.Data(), 0, value4.Data(), 0, std::min(reference.Size(), value4.Size())));
}

TEST_FIXTURE(UtilTest, Compare)
{
    OSAL::ByteArray reference({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::ByteArray value1({0, 1, 2, 3, 4, 5, 6, 7});
    OSAL::ByteArray value2({0, 1, 2, 3, 4, 5, 6, 8});
    OSAL::ByteArray value3({0, 1, 2, 3, 4, 5, 6});
    OSAL::ByteArray value4({0, 1, 2, 3, 4, 5, 6, 7, 8});
    EXPECT_TRUE(Util::Compare(reference.Data(), value1.Data(), std::min(reference.Size(), value1.Size())));
    EXPECT_FALSE(Util::Compare(reference.Data(), value2.Data(), std::min(reference.Size(), value2.Size())));
    EXPECT_TRUE(Util::Compare(reference.Data(), value3.Data(), std::min(reference.Size(), value3.Size())));
    EXPECT_TRUE(Util::Compare(reference.Data(), value4.Data(), std::min(reference.Size(), value4.Size())));
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
