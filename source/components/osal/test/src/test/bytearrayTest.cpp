#include <unittest-cpp/UnitTestC++.h>

#include <cstring>
#include "osal/windows/bytearray-new.h"

using namespace std;

namespace OSAL {
namespace Test {

static const size_t TestSize = 16;

class bytearrayTest : public UnitTestCpp::TestFixture
{
public:
    static void AssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
    {
        _hadAssertion = true;
        _expression = expression;
        _expressionText = expressionText;
        _file = file;
        _line = line;
        _func = func;
    }
    static bool _hadAssertion;
    static bool _expression;
    static const char * _expressionText;
    static const char * _file;
    static int _line;
    static const char * _func;

    void SetUp() override
    {
        _hadAssertion = false;
        _expression = false;
        _expressionText = nullptr;
        _file = nullptr;
        _line = 0;
        _func = nullptr;
        SetAssertionHandler(AssertionHandler);
    }
};

bool bytearrayTest::_hadAssertion = false;
bool bytearrayTest::_expression = false;
const char * bytearrayTest::_expressionText = nullptr;
const char * bytearrayTest::_file = nullptr;
int bytearrayTest::_line = 0;
const char * bytearrayTest::_func = nullptr;


TEST_SUITE(osal)
{

TEST_FIXTURE(bytearrayTest, Constructor)
{
    bytearray byteArray;
    EXPECT_EQ(size_t(0), byteArray.size());
}

TEST_FIXTURE(bytearrayTest, ConstructorSize)
{
    uint8_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    bytearray byteArray(size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), ref, size) == 0);
}

TEST_FIXTURE(bytearrayTest, ConstructorDataLength)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data, size) == 0);
}

TEST_FIXTURE(bytearrayTest, ConstructorCharPtrLength)
{
    char data[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p' };
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data, size) == 0);
}

TEST_FIXTURE(bytearrayTest, ConstructorCharPtr)
{
    const char * data = "abcdefghijklmnop";
    size_t size = strlen(data);
    bytearray byteArray(data);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data, size) == 0);
}

TEST_FIXTURE(bytearrayTest, ConstructorString)
{
    string data = "abcdefghijklmnop";
    size_t size = data.length();
    bytearray byteArray(data);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data.c_str(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, CopyConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    bytearray target(byteArray);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data, size) == 0);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
    EXPECT_NE(target.data(), byteArray.data());
}

TEST_FIXTURE(bytearrayTest, MoveConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    bytearray target(std::move(byteArray));
    EXPECT_EQ(size_t(0), byteArray.size());
    EXPECT_NULL(byteArray.data());
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
    EXPECT_NE(target.data(), byteArray.data());
}

TEST_FIXTURE(bytearrayTest, ConstructorInitializer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray byteArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data, size) == 0);
}

TEST_FIXTURE(bytearrayTest, OperatorAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    bytearray target;
    target = byteArray;
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(byteArray.data(), data, size) == 0);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
    EXPECT_NE(target.data(), byteArray.data());
}

TEST_FIXTURE(bytearrayTest, OperatorMoveAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    bytearray target;
    target = std::move(byteArray);
    EXPECT_EQ(size_t(0), byteArray.size());
    EXPECT_NULL(byteArray.data());
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
    EXPECT_NE(target.data(), byteArray.data());
}

TEST_FIXTURE(bytearrayTest, SetSize)
{
    uint8_t ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    size_t sizeNew = sizeof(ref);
    bytearray target(data, size);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(data, target.data(), size) == 0);

    target.resize(sizeNew);
    EXPECT_EQ(sizeNew, target.size());
    EXPECT_TRUE(memcmp(ref, target.data(), sizeNew) == 0);
}

TEST_FIXTURE(bytearrayTest, Clear)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data);
    bytearray target(data, size);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(data, target.data(), size) == 0);
    target.clear();
    EXPECT_EQ(size_t(0), target.size());
}

TEST_FIXTURE(bytearrayTest, SetUInt8InitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt8(0, 0x01);
    byteArray.SetUInt8(1, 0x02);
    byteArray.SetUInt8(2, 0x03);
    byteArray.SetUInt8(3, 0x04);
    byteArray.SetUInt8(4, 0x05);
    byteArray.SetUInt8(5, 0x06);
    byteArray.SetUInt8(6, 0x07);
    byteArray.SetUInt8(7, 0x08);
    byteArray.SetUInt8(8, 0x09);
    byteArray.SetUInt8(9, 0x0A);
    byteArray.SetUInt8(10, 0x0B);
    byteArray.SetUInt8(11, 0x0C);
    byteArray.SetUInt8(12, 0x0D);
    byteArray.SetUInt8(13, 0x0E);
    byteArray.SetUInt8(14, 0x0F);
    byteArray.SetUInt8(15, 0x10);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetUInt8UninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt8(0, 0x01);
    byteArray.SetUInt8(1, 0x02);
    byteArray.SetUInt8(2, 0x03);
    byteArray.SetUInt8(3, 0x04);
    byteArray.SetUInt8(4, 0x05);
    byteArray.SetUInt8(5, 0x06);
    byteArray.SetUInt8(6, 0x07);
    byteArray.SetUInt8(7, 0x08);
    byteArray.SetUInt8(8, 0x09);
    byteArray.SetUInt8(9, 0x0A);
    byteArray.SetUInt8(10, 0x0B);
    byteArray.SetUInt8(11, 0x0C);
    byteArray.SetUInt8(12, 0x0D);
    byteArray.SetUInt8(13, 0x0E);
    byteArray.SetUInt8(14, 0x0F);
    byteArray.SetUInt8(15, 0x10);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt8)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_EQ(ref[0], byteArray.GetUInt8(0));
    EXPECT_EQ(ref[1], byteArray.GetUInt8(1));
    EXPECT_EQ(ref[2], byteArray.GetUInt8(2));
    EXPECT_EQ(ref[3], byteArray.GetUInt8(3));
    EXPECT_EQ(ref[4], byteArray.GetUInt8(4));
    EXPECT_EQ(ref[5], byteArray.GetUInt8(5));
    EXPECT_EQ(ref[6], byteArray.GetUInt8(6));
    EXPECT_EQ(ref[7], byteArray.GetUInt8(7));
    EXPECT_EQ(ref[8], byteArray.GetUInt8(8));
    EXPECT_EQ(ref[9], byteArray.GetUInt8(9));
    EXPECT_EQ(ref[10], byteArray.GetUInt8(10));
    EXPECT_EQ(ref[11], byteArray.GetUInt8(11));
    EXPECT_EQ(ref[12], byteArray.GetUInt8(12));
    EXPECT_EQ(ref[13], byteArray.GetUInt8(13));
    EXPECT_EQ(ref[14], byteArray.GetUInt8(14));
    EXPECT_EQ(ref[15], byteArray.GetUInt8(15));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt8(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetUInt16InitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt16(0, 0x0201);
    byteArray.SetUInt16(2, 0x0403);
    byteArray.SetUInt16(4, 0x0605);
    byteArray.SetUInt16(6, 0x0807);
    byteArray.SetUInt16(8, 0x0A09);
    byteArray.SetUInt16(10, 0x0C0B);
    byteArray.SetUInt16(12, 0x0E0D);
    byteArray.SetUInt16(14, 0x100F);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetUInt16UninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt16(0, 0x0201);
    byteArray.SetUInt16(2, 0x0403);
    byteArray.SetUInt16(4, 0x0605);
    byteArray.SetUInt16(6, 0x0807);
    byteArray.SetUInt16(8, 0x0A09);
    byteArray.SetUInt16(10, 0x0C0B);
    byteArray.SetUInt16(12, 0x0E0D);
    byteArray.SetUInt16(14, 0x100F);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt16)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
    EXPECT_EQ(ref[0] | ref[1] << 8, byteArray.GetUInt16(0));
    EXPECT_EQ(ref[2] | ref[3] << 8, byteArray.GetUInt16(2));
    EXPECT_EQ(ref[4] | ref[5] << 8, byteArray.GetUInt16(4));
    EXPECT_EQ(ref[6] | ref[7] << 8, byteArray.GetUInt16(6));
    EXPECT_EQ(ref[8] | ref[9] << 8, byteArray.GetUInt16(8));
    EXPECT_EQ(ref[10] | ref[11] << 8, byteArray.GetUInt16(10));
    EXPECT_EQ(ref[12] | ref[13] << 8, byteArray.GetUInt16(12));
    EXPECT_EQ(ref[14] | ref[15] << 8, byteArray.GetUInt16(14));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt16(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetUInt32InitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.SetUInt32(12, 0x100F0E0D);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetUInt32UninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.SetUInt32(12, 0x100F0E0D);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt32)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
    EXPECT_EQ((uint32_t)(ref[0] | ref[1] << 8 | ref[2] << 16 | ref[3] << 24), byteArray.GetUInt32(0));
    EXPECT_EQ((uint32_t)(ref[4] | ref[5] << 8 | ref[6] << 16 | ref[7] << 24), byteArray.GetUInt32(4));
    EXPECT_EQ((uint32_t)(ref[8] | ref[9] << 8 | ref[10] << 16 | ref[11] << 24), byteArray.GetUInt32(8));
    EXPECT_EQ((uint32_t)(ref[12] | ref[13] << 8 | ref[14] << 16 | ref[15] << 24), byteArray.GetUInt32(12));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt32(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetUInt64InitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt64(0, 0x0807060504030201);
    byteArray.SetUInt64(8, 0x100F0E0D0C0B0A09);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetUInt64UninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt64(0, 0x0807060504030201);
    byteArray.SetUInt64(8, 0x100F0E0D0C0B0A09);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt64)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
    EXPECT_EQ((uint64_t)((uint64_t)ref[0] | (uint64_t)ref[1] << 8 |
                         (uint64_t)ref[2] << 16 | (uint64_t)ref[3] << 24 |
                         (uint64_t)ref[4] << 32 | (uint64_t)ref[5] << 40 |
                         (uint64_t)ref[6] << 48 | (uint64_t)ref[7] << 56), byteArray.GetUInt64(0));
    EXPECT_EQ((uint64_t)((uint64_t)ref[8] | (uint64_t)ref[9] << 8 |
                         (uint64_t)ref[10] << 16 | (uint64_t)ref[11] << 24 |
                         (uint64_t)ref[12] << 32 | (uint64_t)ref[13] << 40 |
                         (uint64_t)ref[14] << 48 | (uint64_t)ref[15] << 56), byteArray.GetUInt64(8));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt64(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetUInt16BE)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt16BE(0, 0x0102);
    byteArray.SetUInt16BE(2, 0x0304);
    byteArray.SetUInt16BE(4, 0x0506);
    byteArray.SetUInt16BE(6, 0x0708);
    byteArray.SetUInt16BE(8, 0x090A);
    byteArray.SetUInt16BE(10, 0x0B0C);
    byteArray.SetUInt16BE(12, 0x0D0E);
    byteArray.SetUInt16BE(14, 0x0F10);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt16BE)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
    EXPECT_EQ(ref[0] << 8 | ref[1], byteArray.GetUInt16BE(0));
    EXPECT_EQ(ref[2] << 8 | ref[3], byteArray.GetUInt16BE(2));
    EXPECT_EQ(ref[4] << 8 | ref[5], byteArray.GetUInt16BE(4));
    EXPECT_EQ(ref[6] << 8 | ref[7], byteArray.GetUInt16BE(6));
    EXPECT_EQ(ref[8] << 8 | ref[9], byteArray.GetUInt16BE(8));
    EXPECT_EQ(ref[10] << 8 | ref[11], byteArray.GetUInt16BE(10));
    EXPECT_EQ(ref[12] << 8 | ref[13], byteArray.GetUInt16BE(12));
    EXPECT_EQ(ref[14] << 8 | ref[15], byteArray.GetUInt16BE(14));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt16BE(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetUInt32BE)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt32BE(0, 0x01020304);
    byteArray.SetUInt32BE(4, 0x05060708);
    byteArray.SetUInt32BE(8, 0x090A0B0C);
    byteArray.SetUInt32BE(12, 0x0D0E0F10);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt32BE)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
    EXPECT_EQ((uint32_t)(ref[0] << 24 | ref[1] << 16 | ref[2] << 8 | ref[3] << 0), byteArray.GetUInt32BE(0));
    EXPECT_EQ((uint32_t)(ref[4] << 24 | ref[5] << 16 | ref[6] << 8 | ref[7] << 0), byteArray.GetUInt32BE(4));
    EXPECT_EQ((uint32_t)(ref[8] << 24 | ref[9] << 16 | ref[10] << 8 | ref[11] << 0), byteArray.GetUInt32BE(8));
    EXPECT_EQ((uint32_t)(ref[12] << 24 | ref[13] << 16 | ref[14] << 8 | ref[15] << 0), byteArray.GetUInt32BE(12));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt32BE(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetUInt64BE)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt64BE(0, 0x0102030405060708);
    byteArray.SetUInt64BE(8, 0x090A0B0C0D0E0F10);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetUInt64BE)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray(ref, size);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
    EXPECT_EQ((uint64_t)((uint64_t)ref[0] << 56 | (uint64_t)ref[1] << 48 |
                         (uint64_t)ref[2] << 40 | (uint64_t)ref[3] << 32 |
                         (uint64_t)ref[4] << 24 | (uint64_t)ref[5] << 16 |
                         (uint64_t)ref[6] << 8 | (uint64_t)ref[7] << 0), byteArray.GetUInt64BE(0));
    EXPECT_EQ((uint64_t)((uint64_t)ref[8] << 56 | (uint64_t)ref[9] << 48 |
                         (uint64_t)ref[10] << 40 | (uint64_t)ref[11] << 32 |
                         (uint64_t)ref[12] << 24 | (uint64_t)ref[13] << 16 |
                         (uint64_t)ref[14] << 8 | (uint64_t)ref[15] << 0), byteArray.GetUInt64BE(8));

    ASSERT_FALSE(_hadAssertion);
    byteArray.GetUInt64BE(size_t{16});
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, SetPtrInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t bytes[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.set(12, bytes, sizeof(bytes));
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetPtrUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t bytes[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.set(12, bytes, sizeof(bytes));
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetByteArrayInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    bytearray bytes({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.set(12, bytes);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetByteArrayUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    bytearray bytes({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.set(12, bytes);
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetInitializerInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data);
    bytearray byteArray(data, size);
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, SetInitializerUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray byteArray;
    byteArray.SetUInt32(0, 0x04030201);
    byteArray.SetUInt32(4, 0x08070605);
    byteArray.SetUInt32(8, 0x0C0B0A09);
    byteArray.set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, byteArray.size());
    EXPECT_TRUE(memcmp(ref, byteArray.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, AppendSingle)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    bytearray valueArray(data, sizeof(data));
    valueArray.append(0x0D);
    valueArray.append(0x0E);
    valueArray.append(0x0F);
    valueArray.append(0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size) == 0);
}

TEST_FIXTURE(bytearrayTest, Append)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref);
    bytearray values({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    bytearray valueArray(data, sizeof(data));
    valueArray.append(values);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetPtr)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    bytearray byteArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref);
    EXPECT_EQ((uint32_t)0x04030201, byteArray.GetUInt32(0));
    EXPECT_EQ((uint32_t)0x08070605, byteArray.GetUInt32(4));
    EXPECT_EQ((uint32_t)0x0C0B0A09, byteArray.GetUInt32(8));

    uint8_t data[size];
    EXPECT_EQ(size, byteArray.get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetByteArrayByRef)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    bytearray byteArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref);
    EXPECT_EQ((uint32_t)0x04030201, byteArray.GetUInt32(0));
    EXPECT_EQ((uint32_t)0x08070605, byteArray.GetUInt32(4));
    EXPECT_EQ((uint32_t)0x0C0B0A09, byteArray.GetUInt32(8));

    bytearray data;
    EXPECT_EQ(size, byteArray.Get(12, data, size));
    EXPECT_EQ(size, data.size());
    EXPECT_TRUE(memcmp(ref, data.data(), size) == 0);
}

TEST_FIXTURE(bytearrayTest, GetByteArray)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    bytearray byteArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref);
    EXPECT_EQ((uint32_t)0x04030201, byteArray.GetUInt32(0));
    EXPECT_EQ((uint32_t)0x08070605, byteArray.GetUInt32(4));
    EXPECT_EQ((uint32_t)0x0C0B0A09, byteArray.GetUInt32(8));
    bytearray bytes = byteArray.Get(12, size);
    EXPECT_EQ(size, bytes.size());
    EXPECT_TRUE(memcmp(bytes.data(), ref, size) == 0);

    ASSERT_FALSE(_hadAssertion);
    byteArray.Get(byteArray.size(), 1);
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(bytearrayTest, OperatorEquals)
{
    bytearray byteArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    bytearray byteArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    bytearray byteArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    bytearray byteArrayDummy;
    bytearray byteArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(byteArrayRef1 == byteArrayData);
    EXPECT_TRUE(byteArrayRef2 == byteArrayData);
    EXPECT_FALSE(byteArrayRef3 == byteArrayData);
    EXPECT_FALSE(byteArrayDummy == byteArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(byteArrayData == byteArrayData);
}

TEST_FIXTURE(bytearrayTest, OperatorNotEquals)
{
    bytearray byteArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    bytearray byteArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    bytearray byteArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    bytearray byteArrayDummy;
    bytearray byteArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(byteArrayRef1 != byteArrayData);
    EXPECT_FALSE(byteArrayRef2 != byteArrayData);
    EXPECT_TRUE(byteArrayRef3 != byteArrayData);
    EXPECT_TRUE(byteArrayDummy != byteArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(byteArrayData != byteArrayData);
}

TEST_FIXTURE(bytearrayTest, OperatorIndex)
{
    bytearray target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(0x01, target[size_t{0}]);
    EXPECT_EQ(0x02, target[size_t{1}]);
    EXPECT_EQ(0x03, target[size_t{2}]);
    EXPECT_EQ(0x04, target[size_t{3}]);
    EXPECT_EQ(0x05, target[size_t{4}]);
    EXPECT_EQ(0x06, target[size_t{5}]);
    EXPECT_EQ(0x07, target[size_t{6}]);
    EXPECT_EQ(0x08, target[size_t{7}]);
    EXPECT_EQ(0x09, target[size_t{8}]);
    EXPECT_EQ(0x0A, target[size_t{9}]);
    EXPECT_EQ(0x0B, target[size_t{10}]);
    EXPECT_EQ(0x0C, target[size_t{11}]);
    EXPECT_EQ(0x0D, target[size_t{12}]);
    EXPECT_EQ(0x0E, target[size_t{13}]);
    EXPECT_EQ(0x0F, target[size_t{14}]);
    EXPECT_EQ(0x10, target[size_t{15}]);
    target[size_t{15}] = 0xFF;
    EXPECT_EQ(0xFF, target[size_t{15}]);
}

TEST_FIXTURE(bytearrayTest, OperatorIndexConst)
{
    bytearray target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const bytearray &targetConst = target;
    EXPECT_EQ(0x01, targetConst[size_t{0}]);
    EXPECT_EQ(0x02, targetConst[size_t{1}]);
    EXPECT_EQ(0x03, targetConst[size_t{2}]);
    EXPECT_EQ(0x04, targetConst[size_t{3}]);
    EXPECT_EQ(0x05, targetConst[size_t{4}]);
    EXPECT_EQ(0x06, targetConst[size_t{5}]);
    EXPECT_EQ(0x07, targetConst[size_t{6}]);
    EXPECT_EQ(0x08, targetConst[size_t{7}]);
    EXPECT_EQ(0x09, targetConst[size_t{8}]);
    EXPECT_EQ(0x0A, targetConst[size_t{9}]);
    EXPECT_EQ(0x0B, targetConst[size_t{10}]);
    EXPECT_EQ(0x0C, targetConst[size_t{11}]);
    EXPECT_EQ(0x0D, targetConst[size_t{12}]);
    EXPECT_EQ(0x0E, targetConst[size_t{13}]);
    EXPECT_EQ(0x0F, targetConst[size_t{14}]);
    EXPECT_EQ(0x10, targetConst[size_t{15}]);
}

TEST_FIXTURE(bytearrayTest, PrintTo)
{
    bytearray target({ 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x00 });
    string expected;
    expected =  "OSAL::bytearray Size: 16 Allocated: 16\n";
    expected += "41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 00 A B C D E F G H I J K L M N O ? \n\n";
    ostringstream stream;
    PrintTo(stream, target);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
