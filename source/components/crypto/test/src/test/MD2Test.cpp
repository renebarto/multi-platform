#include <unit-test-c++/UnitTestC++.h>

#include <crypto/MD2.h>

using namespace std;

namespace Crypto {
namespace Test {

class MD2Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void MD2Test::SetUp()
{
}

void MD2Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(MD2Test, Construct)
{
    MD2 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("00000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessNoFinalize_Case2)
{
    MD2 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("00000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessCase1)
{
    MD2 target;

    OSAL::ByteArray expected = {
        0x83, 0x50, 0xE5, 0xA3, 0xE2, 0x4C, 0x15, 0x3D,
        0xF2, 0x27, 0x5C, 0x9F, 0x80, 0x69, 0x27, 0x73,
    };
    uint8_t input[1];
    target.Process(input, 0);
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("8350E5A3E24C153DF2275C9F80692773", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessCase2)
{
    MD2 target;

    OSAL::ByteArray expected = {
        0xDA, 0x85, 0x3B, 0x0D, 0x3F, 0x88, 0xD9, 0x9B,
        0x30, 0x28, 0x3A, 0x69, 0xE6, 0xDE, 0xD6, 0xBB,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("DA853B0D3F88D99B30283A69E6DED6BB", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessResetCase2)
{
    MD2 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("00000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessCase3)
{
    MD2 target;

    OSAL::ByteArray expected = {
        0x4E, 0x8D, 0xDF, 0xF3, 0x65, 0x02, 0x92, 0xAB,
        0x5A, 0x41, 0x08, 0xC3, 0xAA, 0x47, 0x94, 0x0B,
    };
    uint8_t input[26];
    memcpy(input, "abcdefghijklmnopqrstuvwxyz", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("4E8DDFF3650292AB5A4108C3AA47940B", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessCase4)
{
    MD2 target;

    OSAL::ByteArray expected = {
        0xDA, 0x33, 0xDE, 0xF2, 0xA4, 0x2D, 0xF1, 0x39,
        0x75, 0x35, 0x28, 0x46, 0xC3, 0x03, 0x38, 0xCD,
    };
    uint8_t input[62];
    memcpy(input, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("DA33DEF2A42DF13975352846C30338CD", stream.str());
}

TEST_FIXTURE(MD2Test, ProcessCase5)
{
    MD2 target;

    OSAL::ByteArray expected = {
        0xD5, 0x97, 0x6F, 0x79, 0xD8, 0x3D, 0x3A, 0x0D,
        0xC9, 0x80, 0x6C, 0x3C, 0x66, 0xF3, 0xEF, 0xD8,
    };
    uint8_t input[80];
    memcpy(input, "12345678901234567890123456789012345678901234567890123456789012345678901234567890", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("D5976F79D83D3A0DC9806C3C66F3EFD8", stream.str());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
