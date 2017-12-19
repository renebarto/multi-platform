#include <unittest-c++/UnitTestC++.h>

#include <crypto/SHA256.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA224Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA224Test::SetUp()
{
}

void SHA224Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA224Test, Construct)
{
    SHA224 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("00000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessNoFinalize_Case2)
{
    SHA224 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("00000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessCase1)
{
    SHA224 target;

    OSAL::ByteArray expected = {
        0xD1, 0x4A, 0x02, 0x8C, 0x2A, 0x3A, 0x2B, 0xC9,
        0x47, 0x61, 0x02, 0xBB, 0x28, 0x82, 0x34, 0xC4,
        0x15, 0xA2, 0xB0, 0x1F, 0x82, 0x8E, 0xA6, 0x2A,
        0xC5, 0xB3, 0xE4, 0x2F,
    };
    uint8_t input[1];
    target.Process(input, 0);
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("D14A028C2A3A2BC9476102BB288234C415A2B01F828EA62AC5B3E42F", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessCase2)
{
    SHA224 target;

    OSAL::ByteArray expected = {
        0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22,
        0x86, 0x42, 0xA4, 0x77, 0xBD, 0xA2, 0x55, 0xB3,
        0x2A, 0xAD, 0xBC, 0xE4, 0xBD, 0xA0, 0xB3, 0xF7,
        0xE3, 0x6C, 0x9D, 0xA7,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("23097D223405D8228642A477BDA255B32AADBCE4BDA0B3F7E36C9DA7", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessResetCase2)
{
    SHA224 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("00000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessCase3)
{
    SHA224 target;

    OSAL::ByteArray expected = {
        0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC,
        0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50,
        0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19,
        0x52, 0x52, 0x25, 0x25,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("75388B16512776CC5DBA5DA1FD890150B0C6455CB4F58B1952522525", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessCase4)
{
    SHA224 target;

    OSAL::ByteArray expected = {
        0xC9, 0x7C, 0xA9, 0xA5, 0x59, 0x85, 0x0C, 0xE9,
        0x7A, 0x04, 0xA9, 0x6D, 0xEF, 0x6D, 0x99, 0xA9,
        0xE0, 0xE0, 0xE2, 0xAB, 0x14, 0xE6, 0xB8, 0xDF,
        0x26, 0x5F, 0xC0, 0xB3,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("C97CA9A559850CE97A04A96DEF6D99A9E0E0E2AB14E6B8DF265FC0B3", stream.str());
}

TEST_FIXTURE(SHA224Test, ProcessCase5)
{
    SHA224 target;

    OSAL::ByteArray expected = {
        0x20, 0x79, 0x46, 0x55, 0x98, 0x0C, 0x91, 0xD8,
        0xBB, 0xB4, 0xC1, 0xEA, 0x97, 0x61, 0x8A, 0x4B,
        0xF0, 0x3F, 0x42, 0x58, 0x19, 0x48, 0xB2, 0xEE,
        0x4E, 0xE7, 0xAD, 0x67,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("20794655980C91D8BBB4C1EA97618A4BF03F42581948B2EE4EE7AD67", stream.str());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
