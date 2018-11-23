#include <unittest-cpp/UnitTestC++.h>

#include "osal/FixedArray.h"

namespace OSAL {
namespace Test {

static constexpr size_t TestSize = 16;

class FixedArrayTest : public UnitTestCpp::TestFixture
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

bool FixedArrayTest::_hadAssertion = false;
bool FixedArrayTest::_expression = false;
const char * FixedArrayTest::_expressionText = nullptr;
const char * FixedArrayTest::_file = nullptr;
int FixedArrayTest::_line = 0;
const char * FixedArrayTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(FixedArrayTest, ConstructorSize)
{
    uint8_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    FixedArray<uint8_t> valueFixedArray(size);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, ConstructorDataLength)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, CopyConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    FixedArray<uint8_t> target(valueFixedArray);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
    EXPECT_NE(target.Data(), valueFixedArray.Data());
}

TEST_FIXTURE(FixedArrayTest, MoveConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    FixedArray<uint8_t> target(std::move(valueFixedArray));
    EXPECT_EQ(size_t(0), valueFixedArray.Size());
    EXPECT_NULL(valueFixedArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, ConstructorInitializer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, CastToConstElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueFixedArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueFixedArray.Data()), data, size) == 0);
    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueFixedArray.Data()), data, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, CastToElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueFixedArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueFixedArray.Data()), data, size) == 0);
    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueFixedArray.Data()), data, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    FixedArray<uint8_t> target(1);
    target = valueFixedArray;
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
    EXPECT_NE(target.Data(), valueFixedArray.Data());
}

TEST_FIXTURE(FixedArrayTest, OperatorMoveAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    FixedArray<uint8_t> target(1);
    target = std::move(valueFixedArray);
    EXPECT_EQ(size_t(0), valueFixedArray.Size());
    EXPECT_NULL(valueFixedArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, Clear)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    uint8_t empty[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size) == 0);
    target.Clear();
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(empty, target.Data(), size) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetInitializedFixedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[TestSize];
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values, sizeof(values) / sizeof(uint8_t));
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, Get)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    EXPECT_EQ(uint8_t {0x01}, valueFixedArray.Get(0));
    EXPECT_EQ(uint8_t {0x02}, valueFixedArray.Get(1));
    EXPECT_EQ(uint8_t {0x03}, valueFixedArray.Get(2));
    EXPECT_EQ(uint8_t {0x04}, valueFixedArray.Get(3));
    EXPECT_EQ(uint8_t {0x05}, valueFixedArray.Get(4));
    EXPECT_EQ(uint8_t {0x06}, valueFixedArray.Get(5));
    EXPECT_EQ(uint8_t {0x07}, valueFixedArray.Get(6));
    EXPECT_EQ(uint8_t {0x08}, valueFixedArray.Get(7));
    EXPECT_EQ(uint8_t {0x09}, valueFixedArray.Get(8));
    EXPECT_EQ(uint8_t {0x0A}, valueFixedArray.Get(9));
    EXPECT_EQ(uint8_t {0x0B}, valueFixedArray.Get(10));
    EXPECT_EQ(uint8_t {0x0C}, valueFixedArray.Get(11));
    FixedArray<uint8_t> values = valueFixedArray.Get(12, 4);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueFixedArray.Get(valueFixedArray.Size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(FixedArrayTest, GetPtr)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref) / sizeof(uint8_t);

    uint8_t data[size];
    EXPECT_EQ(size, valueFixedArray.Get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueFixedArray.Get(valueFixedArray.Size(), data, 1);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArrayByRef)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);

    FixedArray<uint8_t> data(size);
    EXPECT_EQ(size, valueFixedArray.Get(12, data, size));
    EXPECT_EQ(size, data.Size());
    EXPECT_TRUE(memcmp(ref, data.Data(), size * sizeof(int8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueFixedArray.Get(valueFixedArray.Size(), data, 1);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArray)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> values = valueFixedArray.Get(12, size);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    values = valueFixedArray.Get(valueFixedArray.Size(), 1);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(FixedArrayTest, SetUninitializedFixedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueFixedArray(size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values, sizeof(values) / sizeof(uint8_t));
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayInitializedFixedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayUninitializedFixedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueFixedArray(size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetInitializerInitializedFixedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(int8_t);
    FixedArray<uint8_t> valueFixedArray(data, size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetInitializerUninitializedFixedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueFixedArray(size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorEquals)
{
    FixedArray<uint8_t> valueFixedArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<uint8_t> valueFixedArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<uint8_t> valueFixedArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<uint8_t> valueFixedArrayDummy(0);
    FixedArray<uint8_t> valueFixedArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueFixedArrayRef1 == valueFixedArrayData);
    EXPECT_TRUE(valueFixedArrayRef2 == valueFixedArrayData);
    EXPECT_FALSE(valueFixedArrayRef3 == valueFixedArrayData);
    EXPECT_FALSE(valueFixedArrayDummy == valueFixedArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueFixedArrayData == valueFixedArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorNotEquals)
{
    FixedArray<uint8_t> valueFixedArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<uint8_t> valueFixedArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<uint8_t> valueFixedArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<uint8_t> valueFixedArrayDummy(0);
    FixedArray<uint8_t> valueFixedArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueFixedArrayRef1 != valueFixedArrayData);
    EXPECT_FALSE(valueFixedArrayRef2 != valueFixedArrayData);
    EXPECT_TRUE(valueFixedArrayRef3 != valueFixedArrayData);
    EXPECT_TRUE(valueFixedArrayDummy != valueFixedArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueFixedArrayData != valueFixedArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndex)
{
    FixedArray<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
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

    ASSERT_FALSE(_hadAssertion);
    target[size_t{16}];
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndexConst)
{
    FixedArray<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const FixedArray<uint8_t> &targetConst = target;
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

    ASSERT_FALSE(_hadAssertion);
    targetConst[size_t{16}];
    ASSERT_TRUE(_hadAssertion);
}

TEST_FIXTURE(FixedArrayTest, PrintTo)
{
    FixedArray<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::FixedArray<unsigned char> Item size: 1 Size: 16\n";
    expected += "01 02 03 04 05 06 07 08 \n";
    expected += "09 0a 0b 0c 0d 0e 0f 10 \n\n";
    ostringstream stream;
    PrintTo(stream, target);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(FixedArrayTest, ConstructorInt32)
{
    FixedArray<int32_t> valueFixedArray(0);
    EXPECT_EQ(size_t(0), valueFixedArray.Size());
}

TEST_FIXTURE(FixedArrayTest, ConstructorSizeInt32)
{
    int32_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    FixedArray<int32_t> valueFixedArray(size);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size) == 0);
}

TEST_FIXTURE(FixedArrayTest, ConstructorDataLengthInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CopyConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    FixedArray<int32_t> target(valueFixedArray);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueFixedArray.Data());
}

TEST_FIXTURE(FixedArrayTest, MoveConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    FixedArray<int32_t> target(std::move(valueFixedArray));
    EXPECT_EQ(size_t(0), valueFixedArray.Size());
    EXPECT_NULL(valueFixedArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, ConstructorInitializerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CastToConstElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueFixedArray.Data()), data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueFixedArray.Data()), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CastToElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueFixedArray.Data()), data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueFixedArray.Data()), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    FixedArray<int32_t> target(1);
    target = valueFixedArray;
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueFixedArray.Data());
}

TEST_FIXTURE(FixedArrayTest, OperatorMoveAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    FixedArray<int32_t> target(1);
    target = std::move(valueFixedArray);
    EXPECT_EQ(size_t(0), valueFixedArray.Size());
    EXPECT_NULL(valueFixedArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueFixedArray.Data());
}

TEST_FIXTURE(FixedArrayTest, ClearInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int32_t empty[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size * sizeof(int32_t)) == 0);
    target.Clear();
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(empty, target.Data(), size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetInitializedFixedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values, sizeof(values)/ sizeof(int32_t));
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    EXPECT_EQ((int32_t)0x01, valueFixedArray.Get(0));
    EXPECT_EQ((int32_t)0x02, valueFixedArray.Get(1));
    EXPECT_EQ((int32_t)0x03, valueFixedArray.Get(2));
    EXPECT_EQ((int32_t)0x04, valueFixedArray.Get(3));
    EXPECT_EQ((int32_t)0x05, valueFixedArray.Get(4));
    EXPECT_EQ((int32_t)0x06, valueFixedArray.Get(5));
    EXPECT_EQ((int32_t)0x07, valueFixedArray.Get(6));
    EXPECT_EQ((int32_t)0x08, valueFixedArray.Get(7));
    EXPECT_EQ((int32_t)0x09, valueFixedArray.Get(8));
    EXPECT_EQ((int32_t)0x0A, valueFixedArray.Get(9));
    EXPECT_EQ((int32_t)0x0B, valueFixedArray.Get(10));
    EXPECT_EQ((int32_t)0x0C, valueFixedArray.Get(11));
    FixedArray<int32_t> values = valueFixedArray.Get(12, 4);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetPtrInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
	const size_t size = sizeof(ref) / sizeof(int32_t);

    int32_t data[size];
    EXPECT_EQ(size, valueFixedArray.Get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArrayByRefInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);

    FixedArray<int32_t> data(size);
    EXPECT_EQ(size, valueFixedArray.Get(12, data, size));
    EXPECT_EQ(size, data.Size());
    EXPECT_TRUE(memcmp(ref, data.Data(), size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArrayInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueFixedArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> values = valueFixedArray.Get(12, size);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetUninitializedFixedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values, sizeof(values)/ sizeof(int32_t));
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayInitializedFixedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(data, size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayUninitializedFixedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> valueFixedArray(size);
    valueFixedArray.Set(0, 0x01);
    valueFixedArray.Set(1, 0x02);
    valueFixedArray.Set(2, 0x03);
    valueFixedArray.Set(3, 0x04);
    valueFixedArray.Set(4, 0x05);
    valueFixedArray.Set(5, 0x06);
    valueFixedArray.Set(6, 0x07);
    valueFixedArray.Set(7, 0x08);
    valueFixedArray.Set(8, 0x09);
    valueFixedArray.Set(9, 0x0A);
    valueFixedArray.Set(10, 0x0B);
    valueFixedArray.Set(11, 0x0C);
    valueFixedArray.Set(12, values);
    EXPECT_EQ(size, valueFixedArray.Size());
    EXPECT_TRUE(memcmp(valueFixedArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorEqualsInt32)
{
    FixedArray<int32_t> valueFixedArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<int32_t> valueFixedArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<int32_t> valueFixedArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<int32_t> valueFixedArrayDummy(0);
    FixedArray<int32_t> valueFixedArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueFixedArrayRef1 == valueFixedArrayData);
    EXPECT_TRUE(valueFixedArrayRef2 == valueFixedArrayData);
    EXPECT_FALSE(valueFixedArrayRef3 == valueFixedArrayData);
    EXPECT_FALSE(valueFixedArrayDummy == valueFixedArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueFixedArrayData == valueFixedArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorNotEqualsInt32)
{
    FixedArray<int32_t> valueFixedArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<int32_t> valueFixedArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<int32_t> valueFixedArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<int32_t> valueFixedArrayDummy(0);
    FixedArray<int32_t> valueFixedArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueFixedArrayRef1 != valueFixedArrayData);
    EXPECT_FALSE(valueFixedArrayRef2 != valueFixedArrayData);
    EXPECT_TRUE(valueFixedArrayRef3 != valueFixedArrayData);
    EXPECT_TRUE(valueFixedArrayDummy != valueFixedArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueFixedArrayData != valueFixedArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndexInt32)
{
    FixedArray<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
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

TEST_FIXTURE(FixedArrayTest, OperatorIndexConstInt32)
{
    FixedArray<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const FixedArray<int32_t> &targetConst = target;
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

TEST_FIXTURE(FixedArrayTest, ToStringInt32)
{
    FixedArray<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::FixedArray<int> Item size: 4 Size: 16\n";
    expected += "00000001 00000002 00000003 00000004 00000005 00000006 00000007 00000008 \n";
    expected += "00000009 0000000a 0000000b 0000000c 0000000d 0000000e 0000000f 00000010 \n\n";
    ostringstream stream;
    PrintTo(stream, target);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
