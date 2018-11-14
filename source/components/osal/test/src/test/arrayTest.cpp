#include <unittest-cpp/UnitTestC++.h>

#include "osal/array.h"

namespace OSAL {
namespace Test {

static constexpr size_t TestSize = 16;

class arrayTest : public UnitTestCpp::TestFixture
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

bool arrayTest::_hadAssertion = false;
bool arrayTest::_expression = false;
const char * arrayTest::_expressionText = nullptr;
const char * arrayTest::_file = nullptr;
int arrayTest::_line = 0;
const char * arrayTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(arrayTest, DefaultConstructor)
{
    OSAL::array<uint8_t> valueArray;
    EXPECT_EQ(size_t(0), valueArray.size());
}

TEST_FIXTURE(arrayTest, ConstructorSize)
{
    uint8_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    OSAL::array<uint8_t> valueArray(size);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size) == 0);
}

TEST_FIXTURE(arrayTest, ConstructorInitializer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size) == 0);
}

TEST_FIXTURE(arrayTest, ConstructorDataLength)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> valueArray(data, size);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size) == 0);
}

TEST_FIXTURE(arrayTest, CopyConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> valueArray(data, size);
    array<uint8_t> target(valueArray);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size) == 0);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
    EXPECT_NE(target.data(), valueArray.data());
}

TEST_FIXTURE(arrayTest, MoveConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> valueArray(data, size);
    array<uint8_t> target(std::move(valueArray));
    EXPECT_EQ(size_t(0), valueArray.size());
    EXPECT_NULL(valueArray.data());
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
}

TEST_FIXTURE(arrayTest, OperatorAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> valueArray(data, size);
    array<uint8_t> target;
    target = valueArray;
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size) == 0);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
    EXPECT_NE(target.data(), valueArray.data());
}

TEST_FIXTURE(arrayTest, OperatorMoveAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> valueArray(data, size);
    array<uint8_t> target;
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.size());
    EXPECT_NULL(valueArray.data());
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size) == 0);
}

TEST_FIXTURE(arrayTest, CastToConstElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    const array<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueArray.data()), data, size) == 0);
}

TEST_FIXTURE(arrayTest, CastToElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    array<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueArray.data()), data, size) == 0);
}

TEST_FIXTURE(arrayTest, Resize)
{
    uint8_t ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    size_t sizeNew = sizeof(ref) / sizeof(int8_t);
    array<uint8_t> target(data, size);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(data, target.data(), size * sizeof(int8_t)) == 0);

    target.resize(sizeNew);
    EXPECT_EQ(sizeNew, target.size());
    EXPECT_TRUE(memcmp(ref, target.data(), sizeNew * sizeof(int8_t)) == 0);
}

TEST_FIXTURE(arrayTest, Clear)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int8_t);
    array<uint8_t> target(data, size);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(data, target.data(), size) == 0);
    target.clear();
    EXPECT_EQ(size_t(0), target.size());
}

TEST_FIXTURE(arrayTest, OperatorIndex)
{
    array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
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

TEST_FIXTURE(arrayTest, OperatorIndexConst)
{
    array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const array<uint8_t> &targetConst = target;
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

TEST_FIXTURE(arrayTest, GetSingle)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    EXPECT_EQ(uint8_t {0x01}, valueArray.get(0));
    EXPECT_EQ(uint8_t {0x02}, valueArray.get(1));
    EXPECT_EQ(uint8_t {0x03}, valueArray.get(2));
    EXPECT_EQ(uint8_t {0x04}, valueArray.get(3));
    EXPECT_EQ(uint8_t {0x05}, valueArray.get(4));
    EXPECT_EQ(uint8_t {0x06}, valueArray.get(5));
    EXPECT_EQ(uint8_t {0x07}, valueArray.get(6));
    EXPECT_EQ(uint8_t {0x08}, valueArray.get(7));
    EXPECT_EQ(uint8_t {0x09}, valueArray.get(8));
    EXPECT_EQ(uint8_t {0x0A}, valueArray.get(9));
    EXPECT_EQ(uint8_t {0x0B}, valueArray.get(10));
    EXPECT_EQ(uint8_t {0x0C}, valueArray.get(11));
    array<uint8_t> values = valueArray.get(12, 4);
    EXPECT_EQ(size, values.size());
    EXPECT_TRUE(memcmp(values.data(), ref, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.get(valueArray.size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(arrayTest, GetPtr)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref) / sizeof(uint8_t);

    uint8_t data[size];
    EXPECT_EQ(size, valueArray.get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.get(valueArray.size(), data, 1);
    EXPECT_TRUE(_hadAssertion);

    _hadAssertion = false;
    uint8_t data2[16];
    valueArray.get(1, data2, valueArray.size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(arrayTest, GetArrayByRef)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);

    array<uint8_t> data;
    EXPECT_EQ(size, valueArray.get(12, data, size));
    EXPECT_EQ(size, data.size());
    EXPECT_TRUE(memcmp(ref, data.data(), size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.get(valueArray.size(), data, 1);
    EXPECT_TRUE(_hadAssertion);

    _hadAssertion = false;
    valueArray.get(1, data, valueArray.size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(arrayTest, GetArray)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    array<uint8_t> values = valueArray.get(12, size);
    EXPECT_EQ(size, values.size());
    EXPECT_TRUE(memcmp(values.data(), ref, size * sizeof(uint8_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    values = valueArray.get(valueArray.size(), 1);
    EXPECT_TRUE(_hadAssertion);
    EXPECT_EQ(size_t{ 0 }, values.size());

    _hadAssertion = false;
    values = valueArray.get(1, valueArray.size());
    EXPECT_TRUE(_hadAssertion);
    EXPECT_EQ(size_t{ 0 }, values.size());
}

TEST_FIXTURE(arrayTest, SetSingleInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[TestSize];
    size_t size = sizeof(data) / sizeof(uint8_t);
    array<uint8_t> valueArray(data, size);
    valueArray.set(0, 0x01);
    valueArray.set(1, 0x02);
    valueArray.set(2, 0x03);
    valueArray.set(3, 0x04);
    valueArray.set(4, 0x05);
    valueArray.set(5, 0x06);
    valueArray.set(6, 0x07);
    valueArray.set(7, 0x08);
    valueArray.set(8, 0x09);
    valueArray.set(9, 0x0A);
    valueArray.set(10, 0x0B);
    valueArray.set(11, 0x0C);
    valueArray.set(12, 0x0D);
    valueArray.set(13, 0x0E);
    valueArray.set(14, 0x0F);
    valueArray.set(15, 0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size) == 0);
}

TEST_FIXTURE(arrayTest, SetSingleUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    array<uint8_t> valueArray;
    valueArray.set(0, 0x01);
    valueArray.set(1, 0x02);
    valueArray.set(2, 0x03);
    valueArray.set(3, 0x04);
    valueArray.set(4, 0x05);
    valueArray.set(5, 0x06);
    valueArray.set(6, 0x07);
    valueArray.set(7, 0x08);
    valueArray.set(8, 0x09);
    valueArray.set(9, 0x0A);
    valueArray.set(10, 0x0B);
    valueArray.set(11, 0x0C);
    valueArray.set(12, 0x0D);
    valueArray.set(13, 0x0E);
    valueArray.set(14, 0x0F);
    valueArray.set(15, 0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetArrayInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    array<uint8_t> values1({ 0x01, 0x02, 0x03, 0x04 });
    array<uint8_t> values2({ 0x05, 0x06, 0x07, 0x08 });
    array<uint8_t> values3({ 0x09, 0x0A, 0x0B, 0x0C });
    array<uint8_t> values4({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(uint8_t);
    array<uint8_t> valueArray(data, size);
    valueArray.set(0, values1);
    valueArray.set(4, values2);
    valueArray.set(8, values3);
    valueArray.set(12, values4);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetArrayUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    array<uint8_t> values1({ 0x01, 0x02, 0x03, 0x04 });
    array<uint8_t> values2({ 0x05, 0x06, 0x07, 0x08 });
    array<uint8_t> values3({ 0x09, 0x0A, 0x0B, 0x0C });
    array<uint8_t> values4({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    array<uint8_t> valueArray;
    valueArray.set(0, values1);
    valueArray.set(4, values2);
    valueArray.set(8, values3);
    valueArray.set(12, values4);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetInitializerInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(uint8_t);
    array<uint8_t> valueArray(data, size);
    valueArray.set(0, { 0x01, 0x02, 0x03, 0x04 });
    valueArray.set(4, { 0x05, 0x06, 0x07, 0x08 });
    valueArray.set(8, { 0x09, 0x0A, 0x0B, 0x0C });
    valueArray.set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetInitializerUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    array<uint8_t> valueArray;
    valueArray.set(0, { 0x01, 0x02, 0x03, 0x04 });
    valueArray.set(4, { 0x05, 0x06, 0x07, 0x08 });
    valueArray.set(8, { 0x09, 0x0A, 0x0B, 0x0C });
    valueArray.set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, AppendSingle)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    array<uint8_t> valueArray(data, sizeof(data) / sizeof(uint8_t));
    valueArray.append(0x0D);
    valueArray.append(0x0E);
    valueArray.append(0x0F);
    valueArray.append(0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, Append)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    array<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    array<uint8_t> valueArray(data, sizeof(data) / sizeof(uint8_t));
    valueArray.append(values);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(arrayTest, PrintTo)
{
    array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::array<unsigned char> Item size: 1 Size: 16\n";
    expected += "01 02 03 04 05 06 07 08 \n";
    expected += "09 0a 0b 0c 0d 0e 0f 10 \n\n";
    ostringstream stream;
    PrintTo(stream, target);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(arrayTest, OperatorOutput)
{
    array<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected = "OSAL::array<unsigned char> Item size: 1 Size: 16\n";
    expected += "01 02 03 04 05 06 07 08 \n";
    expected += "09 0a 0b 0c 0d 0e 0f 10 \n\n";
    ostringstream stream;
    stream << target;
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(arrayTest, OperatorEquals)
{
    array<uint8_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    array<uint8_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    array<uint8_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    array<uint8_t> valueArrayDummy;
    array<uint8_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(arrayTest, OperatorNotEquals)
{
    array<uint8_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    array<uint8_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    array<uint8_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    array<uint8_t> valueArrayDummy;
    array<uint8_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(arrayTest, ConstructorInt32)
{
    array<int32_t> valueArray;
    EXPECT_EQ(size_t(0), valueArray.size());
}

TEST_FIXTURE(arrayTest, ConstructorSizeInt32)
{
    int32_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = TestSize;
    array<int32_t> valueArray(size);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size) == 0);
}

TEST_FIXTURE(arrayTest, ConstructorInitializerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size) == 0);
}

TEST_FIXTURE(arrayTest, ConstructorDataLengthInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, CopyConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    array<int32_t> target(valueArray);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.data(), valueArray.data());
}

TEST_FIXTURE(arrayTest, MoveConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    array<int32_t> target(std::move(valueArray));
    EXPECT_EQ(size_t(0), valueArray.size());
    EXPECT_NULL(valueArray.data());
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, OperatorAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    array<int32_t> target;
    target = valueArray;
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.data(), valueArray.data());
}

TEST_FIXTURE(arrayTest, OperatorMoveAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    array<int32_t> target;
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.size());
    EXPECT_NULL(valueArray.data());
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(target.data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.data(), valueArray.data());
}

TEST_FIXTURE(arrayTest, CastToConstElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    const array<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueArray.data()), data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueArray.data()), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, CastToElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueArray.data()), data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueArray.data()), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, ResizeInt32)
{
    int32_t ref[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    size_t sizeNew = sizeof(ref) / sizeof(int32_t);
    array<int32_t> target(data, size);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(data, target.data(), size * sizeof(int32_t)) == 0);

    target.resize(sizeNew);
    EXPECT_EQ(sizeNew, target.size());
    EXPECT_TRUE(memcmp(ref, target.data(), sizeNew * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, ClearInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> target(data, size);
    EXPECT_EQ(size, target.size());
    EXPECT_TRUE(memcmp(data, target.data(), size * sizeof(int32_t)) == 0);
    target.clear();
    EXPECT_EQ(size_t(0), target.size());
}

TEST_FIXTURE(arrayTest, OperatorIndexInt32)
{
    array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
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

TEST_FIXTURE(arrayTest, OperatorIndexConstInt32)
{
    array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const array<int32_t> &targetConst = target;
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

TEST_FIXTURE(arrayTest, GetSingleInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    EXPECT_EQ(int32_t{ 0x01 }, valueArray.get(0));
    EXPECT_EQ(int32_t{ 0x02 }, valueArray.get(1));
    EXPECT_EQ(int32_t{ 0x03 }, valueArray.get(2));
    EXPECT_EQ(int32_t{ 0x04 }, valueArray.get(3));
    EXPECT_EQ(int32_t{ 0x05 }, valueArray.get(4));
    EXPECT_EQ(int32_t{ 0x06 }, valueArray.get(5));
    EXPECT_EQ(int32_t{ 0x07 }, valueArray.get(6));
    EXPECT_EQ(int32_t{ 0x08 }, valueArray.get(7));
    EXPECT_EQ(int32_t{ 0x09 }, valueArray.get(8));
    EXPECT_EQ(int32_t{ 0x0A }, valueArray.get(9));
    EXPECT_EQ(int32_t{ 0x0B }, valueArray.get(10));
    EXPECT_EQ(int32_t{ 0x0C }, valueArray.get(11));
    array<int32_t> values = valueArray.get(12, 4);
    EXPECT_EQ(size, values.size());
    EXPECT_TRUE(memcmp(values.data(), ref, size * sizeof(int32_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.get(valueArray.size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(arrayTest, GetPtrInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref) / sizeof(int32_t);

    int32_t data[size];
    EXPECT_EQ(size, valueArray.get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int32_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.get(valueArray.size(), data, 1);
    EXPECT_TRUE(_hadAssertion);

    _hadAssertion = false;
    int32_t data2[16];
    valueArray.get(1, data2, valueArray.size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(arrayTest, GetArrayByRefInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);

    array<int32_t> data;
    EXPECT_EQ(size, valueArray.get(12, data, size));
    EXPECT_EQ(size, data.size());
    EXPECT_TRUE(memcmp(ref, data.data(), size * sizeof(int32_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    valueArray.get(valueArray.size(), data, 1);
    EXPECT_TRUE(_hadAssertion);

    _hadAssertion = false;
    valueArray.get(1, data, valueArray.size());
    EXPECT_TRUE(_hadAssertion);
}

TEST_FIXTURE(arrayTest, GetArrayInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    array<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    array<int32_t> values = valueArray.get(12, size);
    EXPECT_EQ(size, values.size());
    EXPECT_TRUE(memcmp(values.data(), ref, size * sizeof(int32_t)) == 0);

    EXPECT_FALSE(_hadAssertion);
    values = valueArray.get(valueArray.size(), 1);
    EXPECT_TRUE(_hadAssertion);
    EXPECT_EQ(size_t{ 0 }, values.size());

    _hadAssertion = false;
    values = valueArray.get(1, valueArray.size());
    EXPECT_TRUE(_hadAssertion);
    EXPECT_EQ(size_t{ 0 }, values.size());
}

TEST_FIXTURE(arrayTest, SetSingleInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t data[TestSize];
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    valueArray.set(0, 0x01);
    valueArray.set(1, 0x02);
    valueArray.set(2, 0x03);
    valueArray.set(3, 0x04);
    valueArray.set(4, 0x05);
    valueArray.set(5, 0x06);
    valueArray.set(6, 0x07);
    valueArray.set(7, 0x08);
    valueArray.set(8, 0x09);
    valueArray.set(9, 0x0A);
    valueArray.set(10, 0x0B);
    valueArray.set(11, 0x0C);
    valueArray.set(12, 0x0D);
    valueArray.set(13, 0x0E);
    valueArray.set(14, 0x0F);
    valueArray.set(15, 0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size) == 0);
}

TEST_FIXTURE(arrayTest, SetSingleUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    array<int32_t> valueArray;
    valueArray.set(0, 0x01);
    valueArray.set(1, 0x02);
    valueArray.set(2, 0x03);
    valueArray.set(3, 0x04);
    valueArray.set(4, 0x05);
    valueArray.set(5, 0x06);
    valueArray.set(6, 0x07);
    valueArray.set(7, 0x08);
    valueArray.set(8, 0x09);
    valueArray.set(9, 0x0A);
    valueArray.set(10, 0x0B);
    valueArray.set(11, 0x0C);
    valueArray.set(12, 0x0D);
    valueArray.set(13, 0x0E);
    valueArray.set(14, 0x0F);
    valueArray.set(15, 0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetArrayInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    array<int32_t> values1({ 0x01, 0x02, 0x03, 0x04 });
    array<int32_t> values2({ 0x05, 0x06, 0x07, 0x08 });
    array<int32_t> values3({ 0x09, 0x0A, 0x0B, 0x0C });
    array<int32_t> values4({ 0x0D, 0x0E, 0x0F, 0x10 });
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    valueArray.set(0, values1);
    valueArray.set(4, values2);
    valueArray.set(8, values3);
    valueArray.set(12, values4);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetArrayUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    array<int32_t> values1({ 0x01, 0x02, 0x03, 0x04 });
    array<int32_t> values2({ 0x05, 0x06, 0x07, 0x08 });
    array<int32_t> values3({ 0x09, 0x0A, 0x0B, 0x0C });
    array<int32_t> values4({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    array<int32_t> valueArray;
    valueArray.set(0, values1);
    valueArray.set(4, values2);
    valueArray.set(8, values3);
    valueArray.set(12, values4);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetInitializerInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    array<int32_t> valueArray(data, size);
    valueArray.set(0, { 0x01, 0x02, 0x03, 0x04 });
    valueArray.set(4, { 0x05, 0x06, 0x07, 0x08 });
    valueArray.set(8, { 0x09, 0x0A, 0x0B, 0x0C });
    valueArray.set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, SetInitializerUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    array<int32_t> valueArray;
    valueArray.set(0, { 0x01, 0x02, 0x03, 0x04 });
    valueArray.set(4, { 0x05, 0x06, 0x07, 0x08 });
    valueArray.set(8, { 0x09, 0x0A, 0x0B, 0x0C });
    valueArray.set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, AppendSingleInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    int32_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    array<int32_t> valueArray(data, sizeof(data) / sizeof(int32_t));
    valueArray.append(0x0D);
    valueArray.append(0x0E);
    valueArray.append(0x0F);
    valueArray.append(0x10);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, AppendInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    array<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    int32_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };
    array<int32_t> valueArray(data, sizeof(data) / sizeof(int32_t));
    valueArray.append(values);
    EXPECT_EQ(size, valueArray.size());
    EXPECT_TRUE(memcmp(valueArray.data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(arrayTest, OperatorEqualsInt32)
{
    array<int32_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    array<int32_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    array<int32_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    array<int32_t> valueArrayDummy;
    array<int32_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(arrayTest, OperatorNotEqualsInt32)
{
    array<int32_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    array<int32_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    array<int32_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    array<int32_t> valueArrayDummy;
    array<int32_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(arrayTest, PrintToInt32)
{
    array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::array<int> Item size: 4 Size: 16\n";
    expected += "00000001 00000002 00000003 00000004 00000005 00000006 00000007 00000008 \n";
    expected += "00000009 0000000a 0000000b 0000000c 0000000d 0000000e 0000000f 00000010 \n\n";
    ostringstream stream;
    PrintTo(stream, target);
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(arrayTest, OperatorOutputInt32)
{
    array<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    string expected;
    expected =  "OSAL::array<int> Item size: 4 Size: 16\n";
    expected += "00000001 00000002 00000003 00000004 00000005 00000006 00000007 00000008 \n";
    expected += "00000009 0000000a 0000000b 0000000c 0000000d 0000000e 0000000f 00000010 \n\n";
    ostringstream stream;
    stream << target;
    string actual = stream.str();
    EXPECT_EQ(expected, actual);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
