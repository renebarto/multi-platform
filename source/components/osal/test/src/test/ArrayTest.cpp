#include <unittest-c++/UnitTestC++.h>

#include "osal/Array.h"

namespace OSAL {
namespace Test {

static constexpr size_t TestSize = 16;

class ArrayTest : public UnitTestCpp::TestFixture
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

bool ArrayTest::_hadAssertion = false;
bool ArrayTest::_expression = false;
const char * ArrayTest::_expressionText = nullptr;
const char * ArrayTest::_file = nullptr;
int ArrayTest::_line = 0;
const char * ArrayTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(ArrayTest, DefaultConstructor)
{
    Array<uint8_t> valueArray;
    EXPECT_EQ(size_t(0), valueArray.Size());
}

TEST_FIXTURE(ArrayTest, ConstructorSize)
{
    uint8_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    Array<uint8_t> valueArray(size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size) == 0);
}

TEST_FIXTURE(ArrayTest, ConstructorDataLength)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size) == 0);
}

TEST_FIXTURE(ArrayTest, CopyConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    Array<uint8_t> target(valueArray);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(ArrayTest, MoveConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    Array<uint8_t> target(std::move(valueArray));
    EXPECT_EQ(size_t(0), valueArray.Size());
    EXPECT_NULL(valueArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
}

TEST_FIXTURE(ArrayTest, ConstructorInitializer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size) == 0);
}

TEST_FIXTURE(ArrayTest, CastToConstElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    Array<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueArray), data, size) == 0);
    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueArray), data, size) == 0);
}

TEST_FIXTURE(ArrayTest, CastToElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    Array<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueArray), data, size) == 0);
    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueArray), data, size) == 0);
}

TEST_FIXTURE(ArrayTest, OperatorAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    Array<uint8_t> target;
    target = valueArray;
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(ArrayTest, OperatorMoveAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    Array<uint8_t> target;
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.Size());
    EXPECT_NULL(valueArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
}

TEST_FIXTURE(ArrayTest, Size)
{
    uint8_t ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    size_t sizeNew = sizeof(ref) / sizeof(int8_t);
    Array<uint8_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size * sizeof(int8_t)) == 0);

    target.Size(sizeNew);
    EXPECT_EQ(sizeNew, target.Size());
    EXPECT_TRUE(memcmp(ref, target.Data(), sizeNew * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, Clear)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size) == 0);
    target.Clear();
    EXPECT_EQ(size_t(0), target.Size());
}

TEST_FIXTURE(ArrayTest, SetInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[TestSize];
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values) / sizeof(uint8_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size) == 0);
}

TEST_FIXTURE(ArrayTest, Get)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    EXPECT_EQ(uint8_t {0x01}, valueArray.Get(0));
    EXPECT_EQ(uint8_t {0x02}, valueArray.Get(1));
    EXPECT_EQ(uint8_t {0x03}, valueArray.Get(2));
    EXPECT_EQ(uint8_t {0x04}, valueArray.Get(3));
    EXPECT_EQ(uint8_t {0x05}, valueArray.Get(4));
    EXPECT_EQ(uint8_t {0x06}, valueArray.Get(5));
    EXPECT_EQ(uint8_t {0x07}, valueArray.Get(6));
    EXPECT_EQ(uint8_t {0x08}, valueArray.Get(7));
    EXPECT_EQ(uint8_t {0x09}, valueArray.Get(8));
    EXPECT_EQ(uint8_t {0x0A}, valueArray.Get(9));
    EXPECT_EQ(uint8_t {0x0B}, valueArray.Get(10));
    EXPECT_EQ(uint8_t {0x0C}, valueArray.Get(11));
    Array<uint8_t> values = valueArray.Get(12, 4);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.Get(valueArray.Size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(ArrayTest, GetPtr)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref) / sizeof(uint8_t);

    uint8_t data[size];
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.Get(valueArray.Size(), data, 1);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(ArrayTest, GetArrayByRef)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);

    Array<uint8_t> data;
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_EQ(size, data.Size());
    EXPECT_TRUE(memcmp(ref, data.Data(), size * sizeof(int8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.Get(valueArray.Size(), data, 1);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(ArrayTest, GetArray)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    Array<uint8_t> values = valueArray.Get(12, size);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    values = valueArray.Get(valueArray.Size(), 1);
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(ArrayTest, SetUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    Array<uint8_t> valueArray;
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values) / sizeof(uint8_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetArrayInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    Array<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetArrayUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    Array<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    Array<uint8_t> valueArray;
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetInitializerInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(int8_t);
    Array<uint8_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetInitializerUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    Array<uint8_t> valueArray;
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, Append)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    Array<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    Array<uint8_t> valueArray(data, sizeof(data));
    valueArray.Append(values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(ArrayTest, OperatorEquals)
{
    Array<uint8_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    Array<uint8_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    Array<uint8_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    Array<uint8_t> valueArrayDummy;
    Array<uint8_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(ArrayTest, OperatorNotEquals)
{
    Array<uint8_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    Array<uint8_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    Array<uint8_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    Array<uint8_t> valueArrayDummy;
    Array<uint8_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(ArrayTest, OperatorIndex)
{
    Array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
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

TEST_FIXTURE(ArrayTest, OperatorIndexConst)
{
    Array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const Array<uint8_t> &targetConst = target;
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

TEST_FIXTURE(ArrayTest, PrintTo)
{
    Array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::Array<unsigned char> Item size: 1 Size: 16 Allocated: 16\n";
    expected += "01 02 03 04 05 06 07 08 \n";
    expected += "09 0a 0b 0c 0d 0e 0f 10 \n\n";
    ostringstream stream;
    PrintTo(target, stream);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(ArrayTest, ConstructorInt32)
{
    Array<int32_t> valueArray;
    EXPECT_EQ(size_t(0), valueArray.Size());
}

TEST_FIXTURE(ArrayTest, ConstructorSizeInt32)
{
    int32_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    Array<int32_t> valueArray(size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size) == 0);
}

TEST_FIXTURE(ArrayTest, ConstructorDataLengthInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, CopyConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    Array<int32_t> target(valueArray);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(ArrayTest, MoveConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    Array<int32_t> target(std::move(valueArray));
    EXPECT_EQ(size_t(0), valueArray.Size());
    EXPECT_NULL(valueArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, ConstructorInitializerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, CastToConstElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp((const int32_t *)valueArray, data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueArray), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, CastToElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp((int32_t *)valueArray, data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueArray), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, OperatorAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    Array<int32_t> target;
    target = valueArray;
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(ArrayTest, OperatorMoveAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    Array<int32_t> target;
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.Size());
    EXPECT_NULL(valueArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(ArrayTest, SetSizeInt32)
{
    int32_t ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    size_t sizeNew = sizeof(ref) / sizeof(int32_t);
    Array<int32_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size * sizeof(int32_t)) == 0);

    target.Size(sizeNew);
    EXPECT_EQ(sizeNew, target.Size());
    EXPECT_TRUE(memcmp(ref, target.Data(), sizeNew * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, ClearInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size * sizeof(int32_t)) == 0);
    target.Clear();
    EXPECT_EQ(size_t(0), target.Size());
}

TEST_FIXTURE(ArrayTest, SetInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values)/ sizeof(int32_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, GetInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    EXPECT_EQ((int32_t)0x01, valueArray.Get(0));
    EXPECT_EQ((int32_t)0x02, valueArray.Get(1));
    EXPECT_EQ((int32_t)0x03, valueArray.Get(2));
    EXPECT_EQ((int32_t)0x04, valueArray.Get(3));
    EXPECT_EQ((int32_t)0x05, valueArray.Get(4));
    EXPECT_EQ((int32_t)0x06, valueArray.Get(5));
    EXPECT_EQ((int32_t)0x07, valueArray.Get(6));
    EXPECT_EQ((int32_t)0x08, valueArray.Get(7));
    EXPECT_EQ((int32_t)0x09, valueArray.Get(8));
    EXPECT_EQ((int32_t)0x0A, valueArray.Get(9));
    EXPECT_EQ((int32_t)0x0B, valueArray.Get(10));
    EXPECT_EQ((int32_t)0x0C, valueArray.Get(11));
    Array<int32_t> values = valueArray.Get(12, 4);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, GetPtrInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
	const size_t size = sizeof(ref) / sizeof(int32_t);

    int32_t data[size];
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, GetArrayByRefInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);

    Array<int32_t> data;
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_EQ(size, data.Size());
    EXPECT_TRUE(memcmp(ref, data.Data(), size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, GetArrayInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    Array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    Array<int32_t> values = valueArray.Get(12, size);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    Array<int32_t> valueArray;
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values)/ sizeof(int32_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetArrayInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    Array<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    Array<int32_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, SetArrayUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    Array<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    Array<int32_t> valueArray;
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(ArrayTest, OperatorEqualsInt32)
{
    Array<int32_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    Array<int32_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    Array<int32_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    Array<int32_t> valueArrayDummy;
    Array<int32_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(ArrayTest, OperatorNotEqualsInt32)
{
    Array<int32_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    Array<int32_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    Array<int32_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    Array<int32_t> valueArrayDummy;
    Array<int32_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(ArrayTest, OperatorIndexInt32)
{
    Array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
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

TEST_FIXTURE(ArrayTest, OperatorIndexConstInt32)
{
    Array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const Array<int32_t> &targetConst = target;
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

TEST_FIXTURE(ArrayTest, ToStringInt32)
{
    Array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::Array<int> Item size: 4 Size: 16 Allocated: 16\n";
    expected += "00000001 00000002 00000003 00000004 00000005 00000006 00000007 00000008 \n";
    expected += "00000009 0000000a 0000000b 0000000c 0000000d 0000000e 0000000f 00000010 \n\n";
    ostringstream stream;
    PrintTo(target, stream);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL