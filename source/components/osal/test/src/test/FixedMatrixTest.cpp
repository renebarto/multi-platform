#include <unittest-cpp/UnitTestC++.h>

#include "osal/FixedMatrix.h"

namespace OSAL {
namespace Test {

static constexpr size_t TestSize = 16;

class FixedMatrixTest : public UnitTestCpp::TestFixture
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

bool FixedMatrixTest::_hadAssertion = false;
bool FixedMatrixTest::_expression = false;
const char * FixedMatrixTest::_expressionText = nullptr;
const char * FixedMatrixTest::_file = nullptr;
int FixedMatrixTest::_line = 0;
const char * FixedMatrixTest::_func = nullptr;

TEST_SUITE(osal)
{

//TEST_FIXTURE(FixedMatrixTest, ConstructorSize)
//{
//    uint8_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    size_t size = TestSize;
//    FixedMatrix<uint8_t> valueFixedMatrix(size);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, ConstructorDataLength)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, CopyConstructor)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    FixedMatrix<uint8_t> target(valueFixedMatrix);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size) == 0);
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
//    EXPECT_NE(target.Data(), valueFixedMatrix.Data());
//}
//
//TEST_FIXTURE(FixedMatrixTest, MoveConstructor)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    FixedMatrix<uint8_t> target(std::move(valueFixedMatrix));
//    EXPECT_EQ(size_t(0), valueFixedMatrix.Size());
//    EXPECT_NULL(valueFixedMatrix.Data());
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, ConstructorInitializer)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, CastToConstElementPointer)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(uint8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueFixedMatrix.Data()), data, size) == 0);
//    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueFixedMatrix.Data()), data, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, CastToElementPointer)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(uint8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueFixedMatrix.Data()), data, size) == 0);
//    EXPECT_TRUE(memcmp(static_cast<uint8_t *>(valueFixedMatrix.Data()), data, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorAssign)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    FixedMatrix<uint8_t> target(1);
//    target = valueFixedMatrix;
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size) == 0);
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
//    EXPECT_NE(target.Data(), valueFixedMatrix.Data());
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorMoveAssign)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    FixedMatrix<uint8_t> target(1);
//    target = std::move(valueFixedMatrix);
//    EXPECT_EQ(size_t(0), valueFixedMatrix.Size());
//    EXPECT_NULL(valueFixedMatrix.Data());
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, Clear)
//{
//    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    uint8_t empty[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> target(data, size);
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(data, target.Data(), size) == 0);
//    target.Clear();
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(empty, target.Data(), size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetInitializedFixedMatrix)
//{
//    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    uint8_t data[TestSize];
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values, sizeof(values) / sizeof(uint8_t));
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, Get)
//{
//    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(uint8_t);
//    EXPECT_EQ(uint8_t {0x01}, valueFixedMatrix.Get(0));
//    EXPECT_EQ(uint8_t {0x02}, valueFixedMatrix.Get(1));
//    EXPECT_EQ(uint8_t {0x03}, valueFixedMatrix.Get(2));
//    EXPECT_EQ(uint8_t {0x04}, valueFixedMatrix.Get(3));
//    EXPECT_EQ(uint8_t {0x05}, valueFixedMatrix.Get(4));
//    EXPECT_EQ(uint8_t {0x06}, valueFixedMatrix.Get(5));
//    EXPECT_EQ(uint8_t {0x07}, valueFixedMatrix.Get(6));
//    EXPECT_EQ(uint8_t {0x08}, valueFixedMatrix.Get(7));
//    EXPECT_EQ(uint8_t {0x09}, valueFixedMatrix.Get(8));
//    EXPECT_EQ(uint8_t {0x0A}, valueFixedMatrix.Get(9));
//    EXPECT_EQ(uint8_t {0x0B}, valueFixedMatrix.Get(10));
//    EXPECT_EQ(uint8_t {0x0C}, valueFixedMatrix.Get(11));
//    FixedMatrix<uint8_t> values = valueFixedMatrix.Get(12, 4);
//    EXPECT_EQ(size, values.Size());
//    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);
//
//    EXPECT_FALSE(_hadAssertion);
//    valueFixedMatrix.Get(valueFixedMatrix.Size());
//    EXPECT_TRUE(_hadAssertion);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetPtr)
//{
//    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    const size_t size = sizeof(ref) / sizeof(uint8_t);
//
//    uint8_t data[size];
//    EXPECT_EQ(size, valueFixedMatrix.Get(12, data, size));
//    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int8_t)) == 0);
//
//    EXPECT_FALSE(_hadAssertion);
//    valueFixedMatrix.Get(valueFixedMatrix.Size(), data, 1);
//    EXPECT_TRUE(_hadAssertion);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetFixedMatrixByRef)
//{
//    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(uint8_t);
//
//    FixedMatrix<uint8_t> data(size);
//    EXPECT_EQ(size, valueFixedMatrix.Get(12, data, size));
//    EXPECT_EQ(size, data.Size());
//    EXPECT_TRUE(memcmp(ref, data.Data(), size * sizeof(int8_t)) == 0);
//
//    EXPECT_FALSE(_hadAssertion);
//    valueFixedMatrix.Get(valueFixedMatrix.Size(), data, 1);
//    EXPECT_TRUE(_hadAssertion);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetFixedMatrix)
//{
//    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<uint8_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(uint8_t);
//    FixedMatrix<uint8_t> values = valueFixedMatrix.Get(12, size);
//    EXPECT_EQ(size, values.Size());
//    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);
//
//    EXPECT_FALSE(_hadAssertion);
//    values = valueFixedMatrix.Get(valueFixedMatrix.Size(), 1);
//    EXPECT_TRUE(_hadAssertion);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetUninitializedFixedMatrix)
//{
//    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    size_t size = sizeof(ref) / sizeof(uint8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values, sizeof(values) / sizeof(uint8_t));
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int8_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetFixedMatrixInitializedFixedMatrix)
//{
//    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
//    uint8_t data[16];
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int8_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetFixedMatrixUninitializedFixedMatrix)
//{
//    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(uint8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int8_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetInitializerInitializedFixedMatrix)
//{
//    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    uint8_t data[16];
//    size_t size = sizeof(data) / sizeof(int8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(data, size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int8_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetInitializerUninitializedFixedMatrix)
//{
//    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    size_t size = sizeof(ref) / sizeof(uint8_t);
//    FixedMatrix<uint8_t> valueFixedMatrix(size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int8_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorEquals)
//{
//    FixedMatrix<uint8_t> valueFixedMatrixRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    FixedMatrix<uint8_t> valueFixedMatrixRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    FixedMatrix<uint8_t> valueFixedMatrixRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
//    FixedMatrix<uint8_t> valueFixedMatrixDummy(0);
//    FixedMatrix<uint8_t> valueFixedMatrixData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    EXPECT_FALSE(valueFixedMatrixRef1 == valueFixedMatrixData);
//    EXPECT_TRUE(valueFixedMatrixRef2 == valueFixedMatrixData);
//    EXPECT_FALSE(valueFixedMatrixRef3 == valueFixedMatrixData);
//    EXPECT_FALSE(valueFixedMatrixDummy == valueFixedMatrixData);
//    // cppcheck-suppress duplicateExpression
//    EXPECT_TRUE(valueFixedMatrixData == valueFixedMatrixData);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorNotEquals)
//{
//    FixedMatrix<uint8_t> valueFixedMatrixRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    FixedMatrix<uint8_t> valueFixedMatrixRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    FixedMatrix<uint8_t> valueFixedMatrixRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
//    FixedMatrix<uint8_t> valueFixedMatrixDummy(0);
//    FixedMatrix<uint8_t> valueFixedMatrixData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    EXPECT_TRUE(valueFixedMatrixRef1 != valueFixedMatrixData);
//    EXPECT_FALSE(valueFixedMatrixRef2 != valueFixedMatrixData);
//    EXPECT_TRUE(valueFixedMatrixRef3 != valueFixedMatrixData);
//    EXPECT_TRUE(valueFixedMatrixDummy != valueFixedMatrixData);
//    // cppcheck-suppress duplicateExpression
//    EXPECT_FALSE(valueFixedMatrixData != valueFixedMatrixData);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorIndex)
//{
//    FixedMatrix<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    EXPECT_EQ(0x01, target[size_t{0}]);
//    EXPECT_EQ(0x02, target[size_t{1}]);
//    EXPECT_EQ(0x03, target[size_t{2}]);
//    EXPECT_EQ(0x04, target[size_t{3}]);
//    EXPECT_EQ(0x05, target[size_t{4}]);
//    EXPECT_EQ(0x06, target[size_t{5}]);
//    EXPECT_EQ(0x07, target[size_t{6}]);
//    EXPECT_EQ(0x08, target[size_t{7}]);
//    EXPECT_EQ(0x09, target[size_t{8}]);
//    EXPECT_EQ(0x0A, target[size_t{9}]);
//    EXPECT_EQ(0x0B, target[size_t{10}]);
//    EXPECT_EQ(0x0C, target[size_t{11}]);
//    EXPECT_EQ(0x0D, target[size_t{12}]);
//    EXPECT_EQ(0x0E, target[size_t{13}]);
//    EXPECT_EQ(0x0F, target[size_t{14}]);
//    EXPECT_EQ(0x10, target[size_t{15}]);
//    target[size_t{15}] = 0xFF;
//    EXPECT_EQ(0xFF, target[size_t{15}]);
//
//    ASSERT_FALSE(_hadAssertion);
//    target[size_t{16}];
//    ASSERT_TRUE(_hadAssertion);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorIndexConst)
//{
//    FixedMatrix<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    const FixedMatrix<uint8_t> &targetConst = target;
//    EXPECT_EQ(0x01, targetConst[size_t{0}]);
//    EXPECT_EQ(0x02, targetConst[size_t{1}]);
//    EXPECT_EQ(0x03, targetConst[size_t{2}]);
//    EXPECT_EQ(0x04, targetConst[size_t{3}]);
//    EXPECT_EQ(0x05, targetConst[size_t{4}]);
//    EXPECT_EQ(0x06, targetConst[size_t{5}]);
//    EXPECT_EQ(0x07, targetConst[size_t{6}]);
//    EXPECT_EQ(0x08, targetConst[size_t{7}]);
//    EXPECT_EQ(0x09, targetConst[size_t{8}]);
//    EXPECT_EQ(0x0A, targetConst[size_t{9}]);
//    EXPECT_EQ(0x0B, targetConst[size_t{10}]);
//    EXPECT_EQ(0x0C, targetConst[size_t{11}]);
//    EXPECT_EQ(0x0D, targetConst[size_t{12}]);
//    EXPECT_EQ(0x0E, targetConst[size_t{13}]);
//    EXPECT_EQ(0x0F, targetConst[size_t{14}]);
//    EXPECT_EQ(0x10, targetConst[size_t{15}]);
//
//    ASSERT_FALSE(_hadAssertion);
//    targetConst[size_t{16}];
//    ASSERT_TRUE(_hadAssertion);
//}
//
//TEST_FIXTURE(FixedMatrixTest, PrintTo)
//{
//    FixedMatrix<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    string expected;
//    expected =  "OSAL::FixedMatrix<unsigned char> Item size: 1 Size: 16\n";
//    expected += "01 02 03 04 05 06 07 08 \n";
//    expected += "09 0a 0b 0c 0d 0e 0f 10 \n\n";
//    ostringstream stream;
//    PrintTo(target, stream);
//    string actual = stream.str();
//    EXPECT_EQ(expected, actual);
//}
//
//TEST_FIXTURE(FixedMatrixTest, ConstructorInt32)
//{
//    FixedMatrix<int32_t> valueFixedMatrix(0);
//    EXPECT_EQ(size_t(0), valueFixedMatrix.Size());
//}
//
//TEST_FIXTURE(FixedMatrixTest, ConstructorSizeInt32)
//{
//    int32_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    size_t size = TestSize;
//    FixedMatrix<int32_t> valueFixedMatrix(size);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, ConstructorDataLengthInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, CopyConstructorInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    FixedMatrix<int32_t> target(valueFixedMatrix);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size * sizeof(int32_t)) == 0);
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
//    EXPECT_NE(target.Data(), valueFixedMatrix.Data());
//}
//
//TEST_FIXTURE(FixedMatrixTest, MoveConstructorInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    FixedMatrix<int32_t> target(std::move(valueFixedMatrix));
//    EXPECT_EQ(size_t(0), valueFixedMatrix.Size());
//    EXPECT_NULL(valueFixedMatrix.Data());
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, ConstructorInitializerInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, CastToConstElementPointerInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueFixedMatrix.Data()), data, size * sizeof(int32_t)) == 0);
//    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueFixedMatrix.Data()), data, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, CastToElementPointerInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueFixedMatrix.Data()), data, size * sizeof(int32_t)) == 0);
//    EXPECT_TRUE(memcmp(static_cast<int32_t *>(valueFixedMatrix.Data()), data, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorAssignInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    FixedMatrix<int32_t> target(1);
//    target = valueFixedMatrix;
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), data, size * sizeof(int32_t)) == 0);
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
//    EXPECT_NE(target.Data(), valueFixedMatrix.Data());
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorMoveAssignInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    FixedMatrix<int32_t> target(1);
//    target = std::move(valueFixedMatrix);
//    EXPECT_EQ(size_t(0), valueFixedMatrix.Size());
//    EXPECT_NULL(valueFixedMatrix.Data());
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
//    EXPECT_NE(target.Data(), valueFixedMatrix.Data());
//}
//
//TEST_FIXTURE(FixedMatrixTest, ClearInt32)
//{
//    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    int32_t empty[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> target(data, size);
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(data, target.Data(), size * sizeof(int32_t)) == 0);
//    target.Clear();
//    EXPECT_EQ(size, target.Size());
//    EXPECT_TRUE(memcmp(empty, target.Data(), size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetInitializedFixedMatrixInt32)
//{
//    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    int32_t data[16];
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values, sizeof(values)/ sizeof(int32_t));
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetInt32)
//{
//    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<int32_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(int32_t);
//    EXPECT_EQ((int32_t)0x01, valueFixedMatrix.Get(0));
//    EXPECT_EQ((int32_t)0x02, valueFixedMatrix.Get(1));
//    EXPECT_EQ((int32_t)0x03, valueFixedMatrix.Get(2));
//    EXPECT_EQ((int32_t)0x04, valueFixedMatrix.Get(3));
//    EXPECT_EQ((int32_t)0x05, valueFixedMatrix.Get(4));
//    EXPECT_EQ((int32_t)0x06, valueFixedMatrix.Get(5));
//    EXPECT_EQ((int32_t)0x07, valueFixedMatrix.Get(6));
//    EXPECT_EQ((int32_t)0x08, valueFixedMatrix.Get(7));
//    EXPECT_EQ((int32_t)0x09, valueFixedMatrix.Get(8));
//    EXPECT_EQ((int32_t)0x0A, valueFixedMatrix.Get(9));
//    EXPECT_EQ((int32_t)0x0B, valueFixedMatrix.Get(10));
//    EXPECT_EQ((int32_t)0x0C, valueFixedMatrix.Get(11));
//    FixedMatrix<int32_t> values = valueFixedMatrix.Get(12, 4);
//    EXPECT_EQ(size, values.Size());
//    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetPtrInt32)
//{
//    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<int32_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//	const size_t size = sizeof(ref) / sizeof(int32_t);
//
//    int32_t data[size];
//    EXPECT_EQ(size, valueFixedMatrix.Get(12, data, size));
//    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetFixedMatrixByRefInt32)
//{
//    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<int32_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(int32_t);
//
//    FixedMatrix<int32_t> data(size);
//    EXPECT_EQ(size, valueFixedMatrix.Get(12, data, size));
//    EXPECT_EQ(size, data.Size());
//    EXPECT_TRUE(memcmp(ref, data.Data(), size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, GetFixedMatrixInt32)
//{
//    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<int32_t> valueFixedMatrix({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(int32_t);
//    FixedMatrix<int32_t> values = valueFixedMatrix.Get(12, size);
//    EXPECT_EQ(size, values.Size());
//    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetUninitializedFixedMatrixInt32)
//{
//    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
//    size_t size = sizeof(ref) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values, sizeof(values)/ sizeof(int32_t));
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetFixedMatrixInitializedFixedMatrixInt32)
//{
//    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
//    int32_t data[16];
//    size_t size = sizeof(data) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(data, size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, SetFixedMatrixUninitializedFixedMatrixInt32)
//{
//    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
//    FixedMatrix<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
//    size_t size = sizeof(ref) / sizeof(int32_t);
//    FixedMatrix<int32_t> valueFixedMatrix(size);
//    valueFixedMatrix.Set(0, 0x01);
//    valueFixedMatrix.Set(1, 0x02);
//    valueFixedMatrix.Set(2, 0x03);
//    valueFixedMatrix.Set(3, 0x04);
//    valueFixedMatrix.Set(4, 0x05);
//    valueFixedMatrix.Set(5, 0x06);
//    valueFixedMatrix.Set(6, 0x07);
//    valueFixedMatrix.Set(7, 0x08);
//    valueFixedMatrix.Set(8, 0x09);
//    valueFixedMatrix.Set(9, 0x0A);
//    valueFixedMatrix.Set(10, 0x0B);
//    valueFixedMatrix.Set(11, 0x0C);
//    valueFixedMatrix.Set(12, values);
//    EXPECT_EQ(size, valueFixedMatrix.Size());
//    EXPECT_TRUE(memcmp(valueFixedMatrix.Data(), ref, size * sizeof(int32_t)) == 0);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorEqualsInt32)
//{
//    FixedMatrix<int32_t> valueFixedMatrixRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    FixedMatrix<int32_t> valueFixedMatrixRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    FixedMatrix<int32_t> valueFixedMatrixRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
//    FixedMatrix<int32_t> valueFixedMatrixDummy(0);
//    FixedMatrix<int32_t> valueFixedMatrixData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    EXPECT_FALSE(valueFixedMatrixRef1 == valueFixedMatrixData);
//    EXPECT_TRUE(valueFixedMatrixRef2 == valueFixedMatrixData);
//    EXPECT_FALSE(valueFixedMatrixRef3 == valueFixedMatrixData);
//    EXPECT_FALSE(valueFixedMatrixDummy == valueFixedMatrixData);
//    // cppcheck-suppress duplicateExpression
//    EXPECT_TRUE(valueFixedMatrixData == valueFixedMatrixData);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorNotEqualsInt32)
//{
//    FixedMatrix<int32_t> valueFixedMatrixRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    FixedMatrix<int32_t> valueFixedMatrixRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    FixedMatrix<int32_t> valueFixedMatrixRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
//    FixedMatrix<int32_t> valueFixedMatrixDummy(0);
//    FixedMatrix<int32_t> valueFixedMatrixData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
//    EXPECT_TRUE(valueFixedMatrixRef1 != valueFixedMatrixData);
//    EXPECT_FALSE(valueFixedMatrixRef2 != valueFixedMatrixData);
//    EXPECT_TRUE(valueFixedMatrixRef3 != valueFixedMatrixData);
//    EXPECT_TRUE(valueFixedMatrixDummy != valueFixedMatrixData);
//    // cppcheck-suppress duplicateExpression
//    EXPECT_FALSE(valueFixedMatrixData != valueFixedMatrixData);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorIndexInt32)
//{
//    FixedMatrix<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    EXPECT_EQ(0x01, target[size_t{0}]);
//    EXPECT_EQ(0x02, target[size_t{1}]);
//    EXPECT_EQ(0x03, target[size_t{2}]);
//    EXPECT_EQ(0x04, target[size_t{3}]);
//    EXPECT_EQ(0x05, target[size_t{4}]);
//    EXPECT_EQ(0x06, target[size_t{5}]);
//    EXPECT_EQ(0x07, target[size_t{6}]);
//    EXPECT_EQ(0x08, target[size_t{7}]);
//    EXPECT_EQ(0x09, target[size_t{8}]);
//    EXPECT_EQ(0x0A, target[size_t{9}]);
//    EXPECT_EQ(0x0B, target[size_t{10}]);
//    EXPECT_EQ(0x0C, target[size_t{11}]);
//    EXPECT_EQ(0x0D, target[size_t{12}]);
//    EXPECT_EQ(0x0E, target[size_t{13}]);
//    EXPECT_EQ(0x0F, target[size_t{14}]);
//    EXPECT_EQ(0x10, target[size_t{15}]);
//    target[size_t{15}] = 0xFF;
//    EXPECT_EQ(0xFF, target[size_t{15}]);
//}
//
//TEST_FIXTURE(FixedMatrixTest, OperatorIndexConstInt32)
//{
//    FixedMatrix<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    const FixedMatrix<int32_t> &targetConst = target;
//    EXPECT_EQ(0x01, targetConst[size_t{0}]);
//    EXPECT_EQ(0x02, targetConst[size_t{1}]);
//    EXPECT_EQ(0x03, targetConst[size_t{2}]);
//    EXPECT_EQ(0x04, targetConst[size_t{3}]);
//    EXPECT_EQ(0x05, targetConst[size_t{4}]);
//    EXPECT_EQ(0x06, targetConst[size_t{5}]);
//    EXPECT_EQ(0x07, targetConst[size_t{6}]);
//    EXPECT_EQ(0x08, targetConst[size_t{7}]);
//    EXPECT_EQ(0x09, targetConst[size_t{8}]);
//    EXPECT_EQ(0x0A, targetConst[size_t{9}]);
//    EXPECT_EQ(0x0B, targetConst[size_t{10}]);
//    EXPECT_EQ(0x0C, targetConst[size_t{11}]);
//    EXPECT_EQ(0x0D, targetConst[size_t{12}]);
//    EXPECT_EQ(0x0E, targetConst[size_t{13}]);
//    EXPECT_EQ(0x0F, targetConst[size_t{14}]);
//    EXPECT_EQ(0x10, targetConst[size_t{15}]);
//}
//
//TEST_FIXTURE(FixedMatrixTest, ToStringInt32)
//{
//    FixedMatrix<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
//    string expected;
//    expected =  "OSAL::FixedMatrix<int> Item size: 4 Size: 16\n";
//    expected += "00000001 00000002 00000003 00000004 00000005 00000006 00000007 00000008 \n";
//    expected += "00000009 0000000a 0000000b 0000000c 0000000d 0000000e 0000000f 00000010 \n\n";
//    ostringstream stream;
//    PrintTo(target, stream);
//    string actual = stream.str();
//    EXPECT_EQ(expected, actual);
//}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
