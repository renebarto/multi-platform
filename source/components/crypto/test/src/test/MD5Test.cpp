#include <unit-test-c++/UnitTestC++.h>

#include <crypto/MD5.h>

using namespace std;

namespace Crypto {
namespace Test {

class MD5Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void MD5Test::SetUp()
{
}

void MD5Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(MD5Test, Construct)
{
    MD5 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("00000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessNoFinalize_Case2)
{
    MD5 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("00000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessCase1)
{
    MD5 target;

    OSAL::ByteArray expected = {
        0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04,
        0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E,
    };
    uint8_t input[0];
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("D41D8CD98F00B204E9800998ECF8427E", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessCase2)
{
    MD5 target;

    OSAL::ByteArray expected = {
        0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0,
        0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("900150983CD24FB0D6963F7D28E17F72", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessResetCase2)
{
    MD5 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("00000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessCase3)
{
    MD5 target;

    OSAL::ByteArray expected = {
        0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00,
        0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B,
    };
    uint8_t input[26];
    memcpy(input, "abcdefghijklmnopqrstuvwxyz", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("C3FCD3D76192E4007DFB496CCA67E13B", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessCase4)
{
    MD5 target;

    OSAL::ByteArray expected = {
        0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5,
        0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F,
    };
    uint8_t input[62];
    memcpy(input, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("D174AB98D277D9F5A5611C2C9F419D9F", target.ToString());
}

TEST_FIXTURE(MD5Test, ProcessCase5)
{
    MD5 target;

    OSAL::ByteArray expected = {
        0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55,
        0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A,
    };
    uint8_t input[80];
    memcpy(input, "12345678901234567890123456789012345678901234567890123456789012345678901234567890", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("57EDF4A22BE3C955AC49DA2E2107B67A", target.ToString());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
