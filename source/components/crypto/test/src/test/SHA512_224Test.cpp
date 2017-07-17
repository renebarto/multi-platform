#include <unit-test-c++/UnitTestC++.h>

#include <crypto/SHA512.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA512_224Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA512_224Test::SetUp()
{
}

void SHA512_224Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA512_224Test, Construct)
{
    SHA512_224 target;

    Core::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("00000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessNoFinalize_Case2)
{
    SHA512_224 target;

    Core::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("00000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessCase1)
{
    SHA512_224 target;

    Core::ByteArray expected = {
        0x6E, 0xD0, 0xDD, 0x02, 0x80, 0x6F, 0xA8, 0x9E,
        0x25, 0xDE, 0x06, 0x0C, 0x19, 0xD3, 0xAC, 0x86,
        0xCA, 0xBB, 0x87, 0xD6, 0xA0, 0xDD, 0xD0, 0x5C,
        0x33, 0x3B, 0x84, 0xF4,
    };
    uint8_t input[0];
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("6ED0DD02806FA89E25DE060C19D3AC86CABB87D6A0DDD05C333B84F4", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessCase2)
{
    SHA512_224 target;

    Core::ByteArray expected = {
        0x46, 0x34, 0x27, 0x0F, 0x70, 0x7B, 0x6A, 0x54,
        0xDA, 0xAE, 0x75, 0x30, 0x46, 0x08, 0x42, 0xE2,
        0x0E, 0x37, 0xED, 0x26, 0x5C, 0xEE, 0xE9, 0xA4,
        0x3E, 0x89, 0x24, 0xAA,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("4634270F707B6A54DAAE7530460842E20E37ED265CEEE9A43E8924AA", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessResetCase2)
{
    SHA512_224 target;

    Core::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("00000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessCase3)
{
    SHA512_224 target;

    Core::ByteArray expected = {
        0xE5, 0x30, 0x2D, 0x6D, 0x54, 0xBB, 0x24, 0x22,
        0x75, 0xD1, 0xE7, 0x62, 0x2D, 0x68, 0xDF, 0x6E,
        0xB0, 0x2D, 0xED, 0xD1, 0x3F, 0x56, 0x4C, 0x13,
        0xDB, 0xDA, 0x21, 0x74,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("E5302D6D54BB242275D1E7622D68DF6EB02DEDD13F564C13DBDA2174", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessCase4)
{
    SHA512_224 target;

    Core::ByteArray expected = {
        0x23, 0xFE, 0xC5, 0xBB, 0x94, 0xD6, 0x0B, 0x23,
        0x30, 0x81, 0x92, 0x64, 0x0B, 0x0C, 0x45, 0x33,
        0x35, 0xD6, 0x64, 0x73, 0x4F, 0xE4, 0x0E, 0x72,
        0x68, 0x67, 0x4A, 0xF9,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("23FEC5BB94D60B23308192640B0C453335D664734FE40E7268674AF9", target.ToString());
}

TEST_FIXTURE(SHA512_224Test, ProcessCase5)
{
    SHA512_224 target;

    Core::ByteArray expected = {
        0x37, 0xAB, 0x33, 0x1D, 0x76, 0xF0, 0xD3, 0x6D,
        0xE4, 0x22, 0xBD, 0x0E, 0xDE, 0xB2, 0x2A, 0x28,
        0xAC, 0xCD, 0x48, 0x7B, 0x7A, 0x84, 0x53, 0xAE,
        0x96, 0x5D, 0xD2, 0x87,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("37AB331D76F0D36DE422BD0EDEB22A28ACCD487B7A8453AE965DD287", target.ToString());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
