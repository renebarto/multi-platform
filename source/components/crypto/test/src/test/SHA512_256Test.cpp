#include <unit-test-c++/UnitTestC++.h>

#include <crypto/SHA512.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA512_256Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA512_256Test::SetUp()
{
}

void SHA512_256Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA512_256Test, Construct)
{
    SHA512_256 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessNoFinalize_Case2)
{
    SHA512_256 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessCase1)
{
    SHA512_256 target;

    OSAL::ByteArray expected = {
        0xC6, 0x72, 0xB8, 0xD1, 0xEF, 0x56, 0xED, 0x28,
        0xAB, 0x87, 0xC3, 0x62, 0x2C, 0x51, 0x14, 0x06,
        0x9B, 0xDD, 0x3A, 0xD7, 0xB8, 0xF9, 0x73, 0x74,
        0x98, 0xD0, 0xC0, 0x1E, 0xCE, 0xF0, 0x96, 0x7A,
    };
    uint8_t input[0];
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("C672B8D1EF56ED28AB87C3622C5114069BDD3AD7B8F9737498D0C01ECEF0967A", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessCase2)
{
    SHA512_256 target;

    OSAL::ByteArray expected = {
        0x53, 0x04, 0x8E, 0x26, 0x81, 0x94, 0x1E, 0xF9,
        0x9B, 0x2E, 0x29, 0xB7, 0x6B, 0x4C, 0x7D, 0xAB,
        0xE4, 0xC2, 0xD0, 0xC6, 0x34, 0xFC, 0x6D, 0x46,
        0xE0, 0xE2, 0xF1, 0x31, 0x07, 0xE7, 0xAF, 0x23,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("53048E2681941EF99B2E29B76B4C7DABE4C2D0C634FC6D46E0E2F13107E7AF23", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessResetCase2)
{
    SHA512_256 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessCase3)
{
    SHA512_256 target;

    OSAL::ByteArray expected = {
        0xBD, 0xE8, 0xE1, 0xF9, 0xF1, 0x9B, 0xB9, 0xFD,
        0x34, 0x06, 0xC9, 0x0E, 0xC6, 0xBC, 0x47, 0xBD,
        0x36, 0xD8, 0xAD, 0xA9, 0xF1, 0x18, 0x80, 0xDB,
        0xC8, 0xA2, 0x2A, 0x70, 0x78, 0xB6, 0xA4, 0x61,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("BDE8E1F9F19BB9FD3406C90EC6BC47BD36D8ADA9F11880DBC8A22A7078B6A461", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessCase4)
{
    SHA512_256 target;

    OSAL::ByteArray expected = {
        0x39, 0x28, 0xE1, 0x84, 0xFB, 0x86, 0x90, 0xF8,
        0x40, 0xDA, 0x39, 0x88, 0x12, 0x1D, 0x31, 0xBE,
        0x65, 0xCB, 0x9D, 0x3E, 0xF8, 0x3E, 0xE6, 0x14,
        0x6F, 0xEA, 0xC8, 0x61, 0xE1, 0x9B, 0x56, 0x3A,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("3928E184FB8690F840DA3988121D31BE65CB9D3EF83EE6146FEAC861E19B563A", target.ToString());
}

TEST_FIXTURE(SHA512_256Test, ProcessCase5)
{
    SHA512_256 target;

    OSAL::ByteArray expected = {
        0x9A, 0x59, 0xA0, 0x52, 0x93, 0x01, 0x87, 0xA9,
        0x70, 0x38, 0xCA, 0xE6, 0x92, 0xF3, 0x07, 0x08,
        0xAA, 0x64, 0x91, 0x92, 0x3E, 0xF5, 0x19, 0x43,
        0x94, 0xDC, 0x68, 0xD5, 0x6C, 0x74, 0xFB, 0x21,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("9A59A052930187A97038CAE692F30708AA6491923EF5194394DC68D56C74FB21", target.ToString());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
