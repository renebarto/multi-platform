#include <unittest-c++/UnitTestC++.h>

#include <crypto/SHA512.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA384Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA384Test::SetUp()
{
}

void SHA384Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA384Test, Construct)
{
    SHA384 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessNoFinalize_Case2)
{
    SHA384 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessCase1)
{
    SHA384 target;

    OSAL::ByteArray expected = {
        0x38, 0xB0, 0x60, 0xA7, 0x51, 0xAC, 0x96, 0x38,
        0x4C, 0xD9, 0x32, 0x7E, 0xB1, 0xB1, 0xE3, 0x6A,
        0x21, 0xFD, 0xB7, 0x11, 0x14, 0xBE, 0x07, 0x43,
        0x4C, 0x0C, 0xC7, 0xBF, 0x63, 0xF6, 0xE1, 0xDA,
        0x27, 0x4E, 0xDE, 0xBF, 0xE7, 0x6F, 0x65, 0xFB,
        0xD5, 0x1A, 0xD2, 0xF1, 0x48, 0x98, 0xB9, 0x5B,
    };
    uint8_t input[1];
    target.Process(input, 0);
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("38B060A751AC96384CD9327EB1B1E36A21FDB71114BE07434C0CC7BF63F6E1DA274EDEBFE76F65FBD51AD2F14898B95B", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessCase2)
{
    SHA384 target;

    OSAL::ByteArray expected = {
        0xCB, 0x00, 0x75, 0x3F, 0x45, 0xA3, 0x5E, 0x8B,
        0xB5, 0xA0, 0x3D, 0x69, 0x9A, 0xC6, 0x50, 0x07,
        0x27, 0x2C, 0x32, 0xAB, 0x0E, 0xDE, 0xD1, 0x63,
        0x1A, 0x8B, 0x60, 0x5A, 0x43, 0xFF, 0x5B, 0xED,
        0x80, 0x86, 0x07, 0x2B, 0xA1, 0xE7, 0xCC, 0x23,
        0x58, 0xBA, 0xEC, 0xA1, 0x34, 0xC8, 0x25, 0xA7,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED1631A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessResetCase2)
{
    SHA384 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessCase3)
{
    SHA384 target;

    OSAL::ByteArray expected = {
        0x33, 0x91, 0xFD, 0xDD, 0xFC, 0x8D, 0xC7, 0x39,
        0x37, 0x07, 0xA6, 0x5B, 0x1B, 0x47, 0x09, 0x39,
        0x7C, 0xF8, 0xB1, 0xD1, 0x62, 0xAF, 0x05, 0xAB,
        0xFE, 0x8F, 0x45, 0x0D, 0xE5, 0xF3, 0x6B, 0xC6,
        0xB0, 0x45, 0x5A, 0x85, 0x20, 0xBC, 0x4E, 0x6F,
        0x5F, 0xE9, 0x5B, 0x1F, 0xE3, 0xC8, 0x45, 0x2B,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("3391FDDDFC8DC7393707A65B1B4709397CF8B1D162AF05ABFE8F450DE5F36BC6B0455A8520BC4E6F5FE95B1FE3C8452B", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessCase4)
{
    SHA384 target;

    OSAL::ByteArray expected = {
        0x09, 0x33, 0x0C, 0x33, 0xF7, 0x11, 0x47, 0xE8,
        0x3D, 0x19, 0x2F, 0xC7, 0x82, 0xCD, 0x1B, 0x47,
        0x53, 0x11, 0x1B, 0x17, 0x3B, 0x3B, 0x05, 0xD2,
        0x2F, 0xA0, 0x80, 0x86, 0xE3, 0xB0, 0xF7, 0x12,
        0xFC, 0xC7, 0xC7, 0x1A, 0x55, 0x7E, 0x2D, 0xB9,
        0x66, 0xC3, 0xE9, 0xFA, 0x91, 0x74, 0x60, 0x39,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("09330C33F71147E83D192FC782CD1B4753111B173B3B05D22FA08086E3B0F712FCC7C71A557E2DB966C3E9FA91746039", stream.str());
}

TEST_FIXTURE(SHA384Test, ProcessCase5)
{
    SHA384 target;

    OSAL::ByteArray expected = {
        0x9D, 0x0E, 0x18, 0x09, 0x71, 0x64, 0x74, 0xCB,
        0x08, 0x6E, 0x83, 0x4E, 0x31, 0x0A, 0x4A, 0x1C,
        0xED, 0x14, 0x9E, 0x9C, 0x00, 0xF2, 0x48, 0x52,
        0x79, 0x72, 0xCE, 0xC5, 0x70, 0x4C, 0x2A, 0x5B,
        0x07, 0xB8, 0xB3, 0xDC, 0x38, 0xEC, 0xC4, 0xEB,
        0xAE, 0x97, 0xDD, 0xD8, 0x7F, 0x3D, 0x89, 0x85,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("9D0E1809716474CB086E834E310A4A1CED149E9C00F248527972CEC5704C2A5B07B8B3DC38ECC4EBAE97DDD87F3D8985", stream.str());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
