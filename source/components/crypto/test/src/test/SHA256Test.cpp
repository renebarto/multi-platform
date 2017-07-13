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

TEST_FIXTURE(SHA256Test, Construct)
{
    SHA256 target;

    Core::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessNoFinalize_Case1)
{
    SHA256 target;

    Core::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase1)
{
    SHA256 target;

    Core::ByteArray expected = {
        0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A,
        0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C,
        0x9C, 0xD0, 0xD8, 0x9D, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessResetCase1)
{
    SHA256 target;

    Core::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase2)
{
    SHA256 target;

    Core::ByteArray expected = {
        0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E,
        0xBA, 0xAE, 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5,
        0xE5, 0x46, 0x70, 0xF1, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

TEST_FIXTURE(SHA256Test, ProcessCase3)
{
    SHA256 target;

    Core::ByteArray expected = {
        0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4,
        0xF6, 0x1E, 0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31,
        0x65, 0x34, 0x01, 0x6F, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000", target.ToString());
}

} // namespace Test
} // namespace Crypto
