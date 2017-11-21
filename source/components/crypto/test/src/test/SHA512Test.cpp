#include <unit-test-c++/UnitTestC++.h>

#include <crypto/SHA512.h>

using namespace std;

namespace Crypto {
namespace Test {

class SHA512Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void SHA512Test::SetUp()
{
}

void SHA512Test::TearDown()
{
}

TEST_SUITE(crypto)
{

TEST_FIXTURE(SHA512Test, Construct)
{
    SHA512 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessNoFinalize_Case2)
{
    SHA512 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    target.Process(reinterpret_cast<const uint8_t *>("abc"), 3);
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessCase1)
{
    SHA512 target;

    OSAL::ByteArray expected = {
        0xCF, 0x83, 0xE1, 0x35, 0x7E, 0xEF, 0xB8, 0xBD,
        0xF1, 0x54, 0x28, 0x50, 0xD6, 0x6D, 0x80, 0x07,
        0xD6, 0x20, 0xE4, 0x05, 0x0B, 0x57, 0x15, 0xDC,
        0x83, 0xF4, 0xA9, 0x21, 0xD3, 0x6C, 0xE9, 0xCE,
        0x47, 0xD0, 0xD1, 0x3C, 0x5D, 0x85, 0xF2, 0xB0,
        0xFF, 0x83, 0x18, 0xD2, 0x87, 0x7E, 0xEC, 0x2F,
        0x63, 0xB9, 0x31, 0xBD, 0x47, 0x41, 0x7A, 0x81,
        0xA5, 0x38, 0x32, 0x7A, 0xF9, 0x27, 0xDA, 0x3E,
    };
    uint8_t input[1];
    target.Process(input, 0);
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("CF83E1357EEFB8BDF1542850D66D8007D620E4050B5715DC83F4A921D36CE9CE"
              "47D0D13C5D85F2B0FF8318D2877EEC2F63B931BD47417A81A538327AF927DA3E", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessCase2)
{
    SHA512 target;

    OSAL::ByteArray expected = {
        0xDD, 0xAF, 0x35, 0xA1, 0x93, 0x61, 0x7A, 0xBA,
        0xCC, 0x41, 0x73, 0x49, 0xAE, 0x20, 0x41, 0x31,
        0x12, 0xE6, 0xFA, 0x4E, 0x89, 0xA9, 0x7E, 0xA2,
        0x0A, 0x9E, 0xEE, 0xE6, 0x4B, 0x55, 0xD3, 0x9A,
        0x21, 0x92, 0x99, 0x2A, 0x27, 0x4F, 0xC1, 0xA8,
        0x36, 0xBA, 0x3C, 0x23, 0xA3, 0xFE, 0xEB, 0xBD,
        0x45, 0x4D, 0x44, 0x23, 0x64, 0x3C, 0xE8, 0x0E,
        0x2A, 0x9A, 0xC9, 0x4F, 0xA5, 0x4C, 0xA4, 0x9F,
    };
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A"
              "2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessResetCase2)
{
    SHA512 target;

    OSAL::ByteArray expected(target.GetDigestSize());
    uint8_t input[3];
    memcpy(input, "abc", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    target.Initialize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000000000000000000000000000000000000000000000", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessCase3)
{
    SHA512 target;

    OSAL::ByteArray expected = {
        0x20, 0x4A, 0x8F, 0xC6, 0xDD, 0xA8, 0x2F, 0x0A,
        0x0C, 0xED, 0x7B, 0xEB, 0x8E, 0x08, 0xA4, 0x16,
        0x57, 0xC1, 0x6E, 0xF4, 0x68, 0xB2, 0x28, 0xA8,
        0x27, 0x9B, 0xE3, 0x31, 0xA7, 0x03, 0xC3, 0x35,
        0x96, 0xFD, 0x15, 0xC1, 0x3B, 0x1B, 0x07, 0xF9,
        0xAA, 0x1D, 0x3B, 0xEA, 0x57, 0x78, 0x9C, 0xA0,
        0x31, 0xAD, 0x85, 0xC7, 0xA7, 0x1D, 0xD7, 0x03,
        0x54, 0xEC, 0x63, 0x12, 0x38, 0xCA, 0x34, 0x45,
    };
    uint8_t input[56];
    memcpy(input, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("204A8FC6DDA82F0A0CED7BEB8E08A41657C16EF468B228A8279BE331A703C335"
              "96FD15C13B1B07F9AA1D3BEA57789CA031AD85C7A71DD70354EC631238CA3445", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessCase4)
{
    SHA512 target;

    OSAL::ByteArray expected = {
        0x8E, 0x95, 0x9B, 0x75, 0xDA, 0xE3, 0x13, 0xDA,
        0x8C, 0xF4, 0xF7, 0x28, 0x14, 0xFC, 0x14, 0x3F,
        0x8F, 0x77, 0x79, 0xC6, 0xEB, 0x9F, 0x7F, 0xA1,
        0x72, 0x99, 0xAE, 0xAD, 0xB6, 0x88, 0x90, 0x18,
        0x50, 0x1D, 0x28, 0x9E, 0x49, 0x00, 0xF7, 0xE4,
        0x33, 0x1B, 0x99, 0xDE, 0xC4, 0xB5, 0x43, 0x3A,
        0xC7, 0xD3, 0x29, 0xEE, 0xB6, 0xDD, 0x26, 0x54,
        0x5E, 0x96, 0xE5, 0x5B, 0x87, 0x4B, 0xE9, 0x09,
    };
    uint8_t input[112];
    memcpy(input, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", sizeof(input));
    target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("8E959B75DAE313DA8CF4F72814FC143F8F7779C6EB9F7FA17299AEADB6889018"
              "501D289E4900F7E4331B99DEC4B5433AC7D329EEB6DD26545E96E55B874BE909", stream.str());
}

TEST_FIXTURE(SHA512Test, ProcessCase5)
{
    SHA512 target;

    OSAL::ByteArray expected = {
        0xE7, 0x18, 0x48, 0x3D, 0x0C, 0xE7, 0x69, 0x64,
        0x4E, 0x2E, 0x42, 0xC7, 0xBC, 0x15, 0xB4, 0x63,
        0x8E, 0x1F, 0x98, 0xB1, 0x3B, 0x20, 0x44, 0x28,
        0x56, 0x32, 0xA8, 0x03, 0xAF, 0xA9, 0x73, 0xEB,
        0xDE, 0x0F, 0xF2, 0x44, 0x87, 0x7E, 0xA6, 0x0A,
        0x4C, 0xB0, 0x43, 0x2C, 0xE5, 0x77, 0xC3, 0x1B,
        0xEB, 0x00, 0x9C, 0x5C, 0x2C, 0x49, 0xAA, 0x2E,
        0x4E, 0xAD, 0xB2, 0x17, 0xAD, 0x8C, 0xC0, 0x9B,
    };
    uint8_t input[10];
    memset(input, 'a', sizeof(input));
    for (int i = 0; i < 100000; ++i)
        target.Process(input, sizeof(input));
    target.Finalize();
    EXPECT_EQ(expected, target.GetDigest());
    std::basic_ostringstream<OSAL::Char> stream;
    stream << target;
    EXPECT_EQ("E718483D0CE769644E2E42C7BC15B4638E1F98B13B2044285632A803AFA973EB"
              "DE0FF244877EA60A4CB0432CE577C31BEB009C5C2C49AA2E4EADB217AD8CC09B", stream.str());
}

} // TEST_SUITE(crypto)

} // namespace Test
} // namespace Crypto
