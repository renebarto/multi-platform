#include <unit-test-c++/UnitTestC++.h>

#include <crypto/SHA256.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA256Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA256Test::SetUp()
{
}

void SHA256Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA256Test, Construct)
{
    SHA256 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessNoFinalize_Case2)
{
    SHA256 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase1)
{
    SHA256 target;

    OSAL::ByteArray expected = {
        0XE3, 0XB0, 0XC4, 0X42, 0X98, 0XFC, 0X1C, 0X14,
        0X9A, 0XFB, 0XF4, 0XC8, 0X99, 0X6F, 0XB9, 0X24,
        0X27, 0XAE, 0X41, 0XE4, 0X64, 0X9B, 0X93, 0X4C,
        0XA4, 0X95, 0X99, 0X1B, 0X78, 0X52, 0XB8, 0X55,
    };
    uint8_t input[0];
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase2)
{
    SHA256 target;

    OSAL::ByteArray expected = {
        0XBA, 0X78, 0X16, 0XBF, 0X8F, 0X01, 0XCF, 0XEA,
        0X41, 0X41, 0X40, 0XDE, 0X5D, 0XAE, 0X22, 0X23,
        0XB0, 0X03, 0X61, 0XA3, 0X96, 0X17, 0X7A, 0X9C,
        0XB4, 0X10, 0XFF, 0X61, 0XF2, 0X00, 0X15, 0XAD,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessResetCase2)
{
    SHA256 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase3)
{
    SHA256 target;

    OSAL::ByteArray expected = {
        0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
        0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
        0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
        0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase4)
{
    SHA256 target;

    OSAL::ByteArray expected = {
        0xCF, 0x5B, 0x16, 0xA7, 0x78, 0xAF, 0x83, 0x80,
        0x03, 0x6C, 0xE5, 0x9E, 0x7B, 0x04, 0x92, 0x37,
        0x0B, 0x24, 0x9B, 0x11, 0xE8, 0xF0, 0x7A, 0x51,
        0xAF, 0xAC, 0x45, 0x03, 0x7A, 0xFE, 0xE9, 0xD1,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("CF5B16A778AF8380036CE59E7B0492370B249B11E8F07A51AFAC45037AFEE9D1", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase5)
{
    SHA256 target;

    OSAL::ByteArray expected = {
        0xCD, 0xC7, 0x6E, 0x5C, 0x99, 0x14, 0xFB, 0x92,
        0x81, 0xA1, 0xC7, 0xE2, 0x84, 0xD7, 0x3E, 0x67,
        0xF1, 0x80, 0x9A, 0x48, 0xA4, 0x97, 0x20, 0x0E,
        0x04, 0x6D, 0x39, 0xCC, 0xC7, 0x11, 0x2C, 0xD0,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0", target.ToString());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
