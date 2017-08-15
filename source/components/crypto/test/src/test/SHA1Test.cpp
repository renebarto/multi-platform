#include <unit-test-c++/UnitTestC++.h>

#include <crypto/SHA1.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA1Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA1Test::SetUp()
{
}

void SHA1Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA1Test, Construct)
{
    SHA1 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessNoFinalize_Case2)
{
    SHA1 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessCase1)
{
    SHA1 target;

    OSAL::ByteArray expected = {
        0xDA, 0x39, 0xA3, 0xEE, 0x5E, 0x6B, 0x4B, 0x0D,
        0x32, 0x55, 0xBF, 0xEF, 0x95, 0x60, 0x18, 0x90,
        0xAF, 0xD8, 0x07, 0x09,
    };
    uint8_t input[1];
    target.Process(input, 0);
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("DA39A3EE5E6B4B0D3255BFEF95601890AFD80709", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessCase2)
{
    SHA1 target;

    OSAL::ByteArray expected = {
        0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A,
        0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C,
        0x9C, 0xD0, 0xD8, 0x9D,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("A9993E364706816ABA3E25717850C26C9CD0D89D", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessResetCase2)
{
    SHA1 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessCase3)
{
    SHA1 target;

    OSAL::ByteArray expected = {
        0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E,
        0xBA, 0xAE, 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5,
        0xE5, 0x46, 0x70, 0xF1,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("84983E441C3BD26EBAAE4AA1F95129E5E54670F1", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessCase4)
{
    SHA1 target;

    OSAL::ByteArray expected = {
        0xA4, 0x9B, 0x24, 0x46, 0xA0, 0x2C, 0x64, 0x5B,
        0xF4, 0x19, 0xF9, 0x95, 0xB6, 0x70, 0x91, 0x25,
        0x3A, 0x04, 0xA2, 0x59,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("A49B2446A02C645BF419F995B67091253A04A259", target.ToString());
}

TEST_FIXTURE(SHA1Test, ProcessCase5)
{
    SHA1 target;

    OSAL::ByteArray expected = {
        0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4,
        0xF6, 0x1E, 0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31,
        0x65, 0x34, 0x01, 0x6F,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("34AA973CD4C4DAA4F61EEB2BDBAD27316534016F", target.ToString());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
