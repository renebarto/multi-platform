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

TEST_FIXTURE(FixedMatrixTest, ConstructorSize)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        memset(ref[x], 0, cols * sizeof(uint8_t));
    }
    FixedMatrix<uint8_t> valueArray(rows, cols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeDataLength)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedMatrix<uint8_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeDataLengthIncompleteData)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if (index < size)
            {
                ref[x][y] = index++;
            }
            else
            {
                ref[x][y] = 0;
            }
        }
    }

    FixedMatrix<uint8_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeConstDataRowsCols)
{
    const size_t rows = 5;
    const size_t cols = 6;
    const size_t dataRows = 4;
    const size_t dataCols = 5;
    uint8_t ** data = new uint8_t *[dataRows];
    uint8_t index = 0;
    for (size_t x = 0; x < dataRows; x++)
    {
        data[x] = new uint8_t[dataCols];
        for (size_t y = 0; y < dataCols; y++)
        {
            data[x][y] = index++;
        }
    }
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if ((x < dataRows) && (y < dataCols))
            {
                ref[x][y] = data[x][y];

            }
            else
            {
                ref[x][y] = 0;
            }
        }
    }
    FixedMatrix<uint8_t> valueArray(rows, cols, const_cast<const uint8_t **>(data), dataRows, dataCols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
    for (size_t x = 0; x < dataRows; x++)
    {
        delete[] data[x];
    }
    delete[] data;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeNonConstDataRowsCols)
{
    const size_t rows = 5;
    const size_t cols = 6;
    const size_t dataRows = 4;
    const size_t dataCols = 5;
    uint8_t ** data = new uint8_t *[dataRows];
    uint8_t index = 0;
    for (size_t x = 0; x < dataRows; x++)
    {
        data[x] = new uint8_t[dataCols];
        for (size_t y = 0; y < dataCols; y++)
        {
            data[x][y] = index++;
        }
    }
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if ((x < dataRows) && (y < dataCols))
            {
                ref[x][y] = data[x][y];

            }
            else
            {
                ref[x][y] = 0;
            }
        }
    }
    FixedMatrix<uint8_t> valueArray(rows, cols, data, dataRows, dataCols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
    for (size_t x = 0; x < dataRows; x++)
    {
        delete[] data[x];
    }
    delete[] data;
}

TEST_FIXTURE(FixedMatrixTest, CopyConstructor)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedMatrix<uint8_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    FixedMatrix<uint8_t> target(valueArray);
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NE(target.Data(), valueArray.Data());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, MoveConstructor)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedMatrix<uint8_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    FixedMatrix<uint8_t> target(std::move(valueArray));
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NE(target.Data(), valueArray.Data());

    EXPECT_EQ(size_t(0), valueArray.Rows());
    EXPECT_EQ(size_t(0), valueArray.Cols());
    EXPECT_NULL(valueArray.Data());

    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorInitializer)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorInitializerPartial)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if ((x < 3) && (y == 4))
            {
                ref[x][y] = 0;
            }
            else
            {
                ref[x][y] = index;
            }
            index++;
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0, 1, 2, 3 },
    { 5, 6, 7, 8 },
    { 10, 11, 12, 13 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, CastToElementPointer)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    const FixedMatrix<uint8_t> & valueArrayConst = valueArray;
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    EXPECT_TRUE(Util::Compare(ref,
        valueArrayConst.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, OperatorAssign)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    FixedMatrix<uint8_t> target(0, 0);
    target = valueArray;
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NE(target.Data(), valueArray.Data());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, OperatorMove)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    uint8_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    FixedMatrix<uint8_t> target(0, 0);
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.Rows());
    EXPECT_EQ(size_t(0), valueArray.Cols());
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NULL(valueArray.Data());
    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, Clear)
{
    const size_t rows = 4;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = 0;
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    valueArray.Clear();
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetInitializedArray)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<uint8_t>(index++);
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 1, 1, 1, 1 },
    { 2, 2, 2, 2, 2 },
    { 3, 3, 3, 3 },
    { 4, 4, 4, 4 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    uint8_t values[] = { 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13 };
    size_t size = sizeof(values) / sizeof(uint8_t);
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, Get)
{
    const size_t rows = 2;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
    }
    ref[0][0] = 0x0B;
    ref[0][1] = 0x0C;
    ref[0][2] = 0x0D;
    ref[0][3] = 0x0E;
    ref[0][4] = 0x0F;
    ref[1][0] = 0x10;
    ref[1][1] = 0x00;
    ref[1][2] = 0x00;
    ref[1][3] = 0x00;
    ref[1][4] = 0x00;
    FixedMatrix<uint8_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    EXPECT_EQ((uint8_t)0x01, valueArray.Get(0, 0));
    EXPECT_EQ((uint8_t)0x02, valueArray.Get(0, 1));
    EXPECT_EQ((uint8_t)0x03, valueArray.Get(0, 2));
    EXPECT_EQ((uint8_t)0x04, valueArray.Get(0, 3));
    EXPECT_EQ((uint8_t)0x05, valueArray.Get(0, 4));
    EXPECT_EQ((uint8_t)0x06, valueArray.Get(1, 0));
    EXPECT_EQ((uint8_t)0x07, valueArray.Get(1, 1));
    EXPECT_EQ((uint8_t)0x08, valueArray.Get(1, 2));
    EXPECT_EQ((uint8_t)0x09, valueArray.Get(1, 3));
    EXPECT_EQ((uint8_t)0x0A, valueArray.Get(1, 4));
    FixedMatrix<uint8_t> values = valueArray.Get(2, 0, 2, 5);
    EXPECT_EQ(size_t(2), values.Rows());
    EXPECT_EQ(size_t(5), values.Cols());
    EXPECT_TRUE(Util::Compare(ref, values.Data(), values.Rows(), values.Cols()));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, GetPtr)
{
    uint8_t ref[] =
    {
        0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x00, 0x00
    };
    FixedMatrix<uint8_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    constexpr size_t size = sizeof(ref) / sizeof(uint8_t);
    uint8_t data[size];
    EXPECT_EQ(size, valueArray.Get(1, 1, data, size));
    EXPECT_TRUE(Util::Compare(ref, data, size));
}

TEST_FIXTURE(FixedMatrixTest, GetFixedArrayByRef)
{
    const size_t rows = 2;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
    }
    ref[0][0] = 0x0B;
    ref[0][1] = 0x0C;
    ref[0][2] = 0x0D;
    ref[0][3] = 0x0E;
    ref[0][4] = 0x0F;
    ref[1][0] = 0x10;
    ref[1][1] = 0x00;
    ref[1][2] = 0x00;
    ref[1][3] = 0x00;
    ref[1][4] = 0x00;
    FixedMatrix<uint8_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });

    FixedMatrix<uint8_t> data(0, 0);
    valueArray.Get(2, 0, data, valueArray.Rows(), valueArray.Cols());
    EXPECT_EQ(rows, data.Rows());
    EXPECT_EQ(cols, data.Cols());
    EXPECT_TRUE(Util::Compare(ref, data.Data(),
        data.Rows(), data.Cols()));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, GetFixedArray)
{
    const size_t rows = 2;
    const size_t cols = 5;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
    }
    ref[0][0] = 0x0B;
    ref[0][1] = 0x0C;
    ref[0][2] = 0x0D;
    ref[0][3] = 0x0E;
    ref[0][4] = 0x0F;
    ref[1][0] = 0x10;
    ref[1][1] = 0x00;
    ref[1][2] = 0x00;
    ref[1][3] = 0x00;
    ref[1][4] = 0x00;
    FixedMatrix<uint8_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    FixedMatrix<uint8_t> data = valueArray.Get(2, 0, valueArray.Rows(), valueArray.Cols());
    EXPECT_EQ(rows, data.Rows());
    EXPECT_EQ(cols, data.Cols());
    EXPECT_TRUE(Util::Compare(ref, data.Data(),
        data.Rows(), data.Cols()));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetUninitializedArray)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<uint8_t>(index++);
        }
    }
    FixedMatrix<uint8_t> valueArray(rows, cols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    uint8_t values[] = { 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13 };
    size_t size = sizeof(values) / sizeof(uint8_t);
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetFixedArrayInitializedArray)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<uint8_t>(index++);
        }
    }
    FixedMatrix<uint8_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    FixedMatrix<uint8_t> values({ { 0x05, 0x06, 0x07, 0x08, 0x09 },
    { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E },
    { 0x0F, 0x10, 0x11, 0x12, 0x13 } });
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetFixedArrayUninitializedArray)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    uint8_t ** ref = new uint8_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new uint8_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<uint8_t>(index++);
        }
    }
    FixedMatrix<uint8_t> valueArray(rows, cols);
    FixedMatrix<uint8_t> values({ { 0x05, 0x06, 0x07, 0x08, 0x09 },
    { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E },
    { 0x0F, 0x10, 0x11, 0x12, 0x13 } });
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));

    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, OperatorEquals)
{
    FixedMatrix<uint8_t> valueArrayRef1({ { 0x01, 0x02, 0x03, 0x04 },{ 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },{ 0x0D, 0x0E, 0x0F, 0x10 } });
    FixedMatrix<uint8_t> valueArrayRef2({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    FixedMatrix<uint8_t> valueArrayRef3({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E } });
    FixedMatrix<uint8_t> valueArrayRef4({ { 0x00, 0x01 },{ 0x02, 0x03 },{ 0x04, 0x05 },{ 0x06, 0x07 },
    { 0x08, 0x09 },{ 0x0A, 0x0B },{ 0x0C, 0x0D },{ 0x0E, 0x0F } });
    FixedMatrix<uint8_t> valueArrayDummy(0, 0);
    FixedMatrix<uint8_t> valueArrayData({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayRef4 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(FixedMatrixTest, OperatorNotEquals)
{
    FixedMatrix<uint8_t> valueArrayRef1({ { 0x01, 0x02, 0x03, 0x04 },{ 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },{ 0x0D, 0x0E, 0x0F, 0x10 } });
    FixedMatrix<uint8_t> valueArrayRef2({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    FixedMatrix<uint8_t> valueArrayRef3({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E } });
    FixedMatrix<uint8_t> valueArrayRef4({ { 0x00, 0x01 },{ 0x02, 0x03 },{ 0x04, 0x05 },{ 0x06, 0x07 },
    { 0x08, 0x09 },{ 0x0A, 0x0B },{ 0x0C, 0x0D },{ 0x0E, 0x0F } });
    FixedMatrix<uint8_t> valueArrayDummy(0, 0);
    FixedMatrix<uint8_t> valueArrayData({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayRef4 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(FixedMatrixTest, OperatorIndex)
{
    FixedMatrix<uint8_t> target({ { 0x01, 0x02, 0x03, 0x04 },
    { 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },
    { 0x0D, 0x0E, 0x0F, 0x10 } });
    EXPECT_EQ(0x01, target(0, 0));
    EXPECT_EQ(0x02, target(0, 1));
    EXPECT_EQ(0x03, target(0, 2));
    EXPECT_EQ(0x04, target(0, 3));
    EXPECT_EQ(0x05, target(1, 0));
    EXPECT_EQ(0x06, target(1, 1));
    EXPECT_EQ(0x07, target(1, 2));
    EXPECT_EQ(0x08, target(1, 3));
    EXPECT_EQ(0x09, target(2, 0));
    EXPECT_EQ(0x0A, target(2, 1));
    EXPECT_EQ(0x0B, target(2, 2));
    EXPECT_EQ(0x0C, target(2, 3));
    EXPECT_EQ(0x0D, target(3, 0));
    EXPECT_EQ(0x0E, target(3, 1));
    EXPECT_EQ(0x0F, target(3, 2));
    EXPECT_EQ(0x10, target(3, 3));
    target(3, 3) = 0xFF;
    EXPECT_EQ(0xFF, target(3, 3));
    EXPECT_THROW(target(static_cast<size_t>(-1), 0), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(target(0, static_cast<size_t>(-1)), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(target(3, 4), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(target(4, 3), OSAL::ArgumentOutOfRangeException);
}

TEST_FIXTURE(FixedMatrixTest, OperatorIndexConst)
{
    FixedMatrix<uint8_t> target({ { 0x01, 0x02, 0x03, 0x04 },
    { 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },
    { 0x0D, 0x0E, 0x0F, 0x10 } });
    const FixedMatrix<uint8_t> &targetConst = target;
    EXPECT_EQ(0x01, targetConst(0, 0));
    EXPECT_EQ(0x02, targetConst(0, 1));
    EXPECT_EQ(0x03, targetConst(0, 2));
    EXPECT_EQ(0x04, targetConst(0, 3));
    EXPECT_EQ(0x05, targetConst(1, 0));
    EXPECT_EQ(0x06, targetConst(1, 1));
    EXPECT_EQ(0x07, targetConst(1, 2));
    EXPECT_EQ(0x08, targetConst(1, 3));
    EXPECT_EQ(0x09, targetConst(2, 0));
    EXPECT_EQ(0x0A, targetConst(2, 1));
    EXPECT_EQ(0x0B, targetConst(2, 2));
    EXPECT_EQ(0x0C, targetConst(2, 3));
    EXPECT_EQ(0x0D, targetConst(3, 0));
    EXPECT_EQ(0x0E, targetConst(3, 1));
    EXPECT_EQ(0x0F, targetConst(3, 2));
    EXPECT_EQ(0x10, targetConst(3, 3));
    EXPECT_THROW(targetConst(static_cast<size_t>(-1), 0), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst(0, static_cast<size_t>(-1)), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst(3, 4), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst(4, 3), OSAL::ArgumentOutOfRangeException);
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        memset(ref[x], 0, cols * sizeof(int32_t));
    }
    FixedMatrix<int32_t> valueArray(rows, cols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeDataLengthInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedMatrix<int32_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeDataLengthIncompleteDataInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    size_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if (index < size)
            {
                ref[x][y] = static_cast<int32_t>(index++);
            }
            else
            {
                ref[x][y] = 0;
            }
        }
    }

    FixedMatrix<int32_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeConstDataRowsColsInt32)
{
    const size_t rows = 5;
    const size_t cols = 6;
    const size_t dataRows = 4;
    const size_t dataCols = 5;
    int32_t ** data = new int32_t *[dataRows];
    int32_t index = 0;
    for (size_t x = 0; x < dataRows; x++)
    {
        data[x] = new int32_t[dataCols];
        for (size_t y = 0; y < dataCols; y++)
        {
            data[x][y] = index++;
        }
    }
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if ((x < dataRows) && (y < dataCols))
            {
                ref[x][y] = data[x][y];

            }
            else
            {
                ref[x][y] = 0;
            }
        }
    }
    FixedMatrix<int32_t> valueArray(rows, cols, const_cast<const int32_t **>(data), dataRows, dataCols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
    for (size_t x = 0; x < dataRows; x++)
    {
        delete[] data[x];
    }
    delete[] data;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorSizeNonConstDataRowsColsInt32)
{
    const size_t rows = 5;
    const size_t cols = 6;
    const size_t dataRows = 4;
    const size_t dataCols = 5;
    int32_t ** data = new int32_t *[dataRows];
    int32_t index = 0;
    for (size_t x = 0; x < dataRows; x++)
    {
        data[x] = new int32_t[dataCols];
        for (size_t y = 0; y < dataCols; y++)
        {
            data[x][y] = index++;
        }
    }
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if ((x < dataRows) && (y < dataCols))
            {
                ref[x][y] = data[x][y];

            }
            else
            {
                ref[x][y] = 0;
            }
        }
    }
    FixedMatrix<int32_t> valueArray(rows, cols, data, dataRows, dataCols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
    for (size_t x = 0; x < dataRows; x++)
    {
        delete[] data[x];
    }
    delete[] data;
}

TEST_FIXTURE(FixedMatrixTest, CopyConstructorInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedMatrix<int32_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    FixedMatrix<int32_t> target(valueArray);
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NE(target.Data(), valueArray.Data());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, MoveConstructorInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedMatrix<int32_t> valueArray(rows, cols, data, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    FixedMatrix<int32_t> target(std::move(valueArray));
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NE(target.Data(), valueArray.Data());

    EXPECT_EQ(size_t(0), valueArray.Rows());
    EXPECT_EQ(size_t(0), valueArray.Cols());
    EXPECT_NULL(valueArray.Data());

    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorInitializerInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ConstructorInitializerPartialInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            if ((x < 3) && (y == 4))
            {
                ref[x][y] = 0;
            }
            else
            {
                ref[x][y] = index;
            }
            index++;
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0, 1, 2, 3 },
    { 5, 6, 7, 8 },
    { 10, 11, 12, 13 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, CastToElementPointerInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    const FixedMatrix<int32_t> & valueArrayConst = valueArray;
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    EXPECT_TRUE(Util::Compare(ref, valueArrayConst.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, OperatorAssignInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    FixedMatrix<int32_t> target(0, 0);
    target = valueArray;
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NE(target.Data(), valueArray.Data());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, OperatorMoveInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    int32_t index = 0;
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = index++;
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    FixedMatrix<int32_t> target(0, 0);
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.Rows());
    EXPECT_EQ(size_t(0), valueArray.Cols());
    EXPECT_EQ(rows, target.Rows());
    EXPECT_EQ(cols, target.Cols());
    EXPECT_NULL(valueArray.Data());
    EXPECT_TRUE(Util::Compare(ref, target.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, ClearInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = 0;
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0, 1, 2, 3, 4 },
    { 5, 6, 7, 8, 9 },
    { 10, 11, 12, 13, 14 },
    { 15, 16, 17, 18, 19 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    valueArray.Clear();
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetInitializedArrayInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<int32_t>(index++);
        }
    }
    FixedMatrix<int32_t> valueArray({ { 1, 1, 1, 1 },
    { 2, 2, 2, 2, 2 },
    { 3, 3, 3, 3 },
    { 4, 4, 4, 4 } });
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    int32_t values[] = { 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13 };
    size_t size = sizeof(values) / sizeof(int32_t);
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, GetInt32)
{
    const size_t rows = 2;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
    }
    ref[0][0] = 0x0B;
    ref[0][1] = 0x0C;
    ref[0][2] = 0x0D;
    ref[0][3] = 0x0E;
    ref[0][4] = 0x0F;
    ref[1][0] = 0x10;
    ref[1][1] = 0x00;
    ref[1][2] = 0x00;
    ref[1][3] = 0x00;
    ref[1][4] = 0x00;
    FixedMatrix<int32_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    EXPECT_EQ((int32_t)0x01, valueArray.Get(0, 0));
    EXPECT_EQ((int32_t)0x02, valueArray.Get(0, 1));
    EXPECT_EQ((int32_t)0x03, valueArray.Get(0, 2));
    EXPECT_EQ((int32_t)0x04, valueArray.Get(0, 3));
    EXPECT_EQ((int32_t)0x05, valueArray.Get(0, 4));
    EXPECT_EQ((int32_t)0x06, valueArray.Get(1, 0));
    EXPECT_EQ((int32_t)0x07, valueArray.Get(1, 1));
    EXPECT_EQ((int32_t)0x08, valueArray.Get(1, 2));
    EXPECT_EQ((int32_t)0x09, valueArray.Get(1, 3));
    EXPECT_EQ((int32_t)0x0A, valueArray.Get(1, 4));
    FixedMatrix<int32_t> values = valueArray.Get(2, 0, 2, 5);
    EXPECT_EQ(size_t(2), values.Rows());
    EXPECT_EQ(size_t(5), values.Cols());
    EXPECT_TRUE(Util::Compare(ref, values.Data(),
        values.Rows(), values.Cols()));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, GetPtrInt32)
{
    int32_t ref[] =
    {
        0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x00, 0x00
    };
    FixedMatrix<int32_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    constexpr size_t size = sizeof(ref) / sizeof(int32_t);
    int32_t data[size];
    EXPECT_EQ(size, valueArray.Get(1, 1, data, size));
    EXPECT_TRUE(Util::Compare(ref, data, size));
}

TEST_FIXTURE(FixedMatrixTest, GetFixedArrayByRefInt32)
{
    const size_t rows = 2;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
    }
    ref[0][0] = 0x0B;
    ref[0][1] = 0x0C;
    ref[0][2] = 0x0D;
    ref[0][3] = 0x0E;
    ref[0][4] = 0x0F;
    ref[1][0] = 0x10;
    ref[1][1] = 0x00;
    ref[1][2] = 0x00;
    ref[1][3] = 0x00;
    ref[1][4] = 0x00;
    FixedMatrix<int32_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });

    FixedMatrix<int32_t> data(0, 0);
    valueArray.Get(2, 0, data, valueArray.Rows(), valueArray.Cols());
    EXPECT_EQ(rows, data.Rows());
    EXPECT_EQ(cols, data.Cols());
    EXPECT_TRUE(Util::Compare(ref, data.Data(),
        data.Rows(), data.Cols()));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, GetFixedArrayInt32)
{
    const size_t rows = 2;
    const size_t cols = 5;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
    }
    ref[0][0] = 0x0B;
    ref[0][1] = 0x0C;
    ref[0][2] = 0x0D;
    ref[0][3] = 0x0E;
    ref[0][4] = 0x0F;
    ref[1][0] = 0x10;
    ref[1][1] = 0x00;
    ref[1][2] = 0x00;
    ref[1][3] = 0x00;
    ref[1][4] = 0x00;
    FixedMatrix<int32_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    FixedMatrix<int32_t> data = valueArray.Get(2, 0, valueArray.Rows(), valueArray.Cols());
    EXPECT_EQ(rows, data.Rows());
    EXPECT_EQ(cols, data.Cols());
    EXPECT_TRUE(Util::Compare(ref, data.Data(),
        data.Rows(), data.Cols()));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetUninitializedArrayInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<int32_t>(index++);
        }
    }
    FixedMatrix<int32_t> valueArray(rows, cols);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    int32_t values[] = { 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13 };
    size_t size = sizeof(values) / sizeof(int32_t);
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values, size);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetFixedArrayInitializedArrayInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<int32_t>(index++);
        }
    }
    FixedMatrix<int32_t> valueArray({ { 0x01, 0x02, 0x03, 0x04, 0x05 },
    { 0x06, 0x07, 0x08, 0x09, 0x0A },
    { 0x0B, 0x0C, 0x0D, 0x0E, 0x0F },
    { 0x10 } });
    FixedMatrix<int32_t> values({ { 0x05, 0x06, 0x07, 0x08, 0x09 },
    { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E },
    { 0x0F, 0x10, 0x11, 0x12, 0x13 } });
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, SetFixedArrayUninitializedArrayInt32)
{
    const size_t rows = 4;
    const size_t cols = 5;
    size_t index = 0;
    int32_t ** ref = new int32_t *[rows];
    for (size_t x = 0; x < rows; x++)
    {
        ref[x] = new int32_t[cols];
        for (size_t y = 0; y < cols; y++)
        {
            ref[x][y] = static_cast<int32_t>(index++);
        }
    }
    FixedMatrix<int32_t> valueArray(rows, cols);
    FixedMatrix<int32_t> values({ { 0x05, 0x06, 0x07, 0x08, 0x09 },
    { 0x0A, 0x0B, 0x0C, 0x0D, 0x0E },
    { 0x0F, 0x10, 0x11, 0x12, 0x13 } });
    valueArray.Set(0, 0, 0x00);
    valueArray.Set(0, 1, 0x01);
    valueArray.Set(0, 2, 0x02);
    valueArray.Set(0, 3, 0x03);
    valueArray.Set(0, 4, 0x04);
    valueArray.Set(1, 0, values);
    EXPECT_EQ(rows, valueArray.Rows());
    EXPECT_EQ(cols, valueArray.Cols());
    EXPECT_TRUE(Util::Compare(ref, valueArray.Data(),
        rows, cols));
    for (size_t x = 0; x < rows; x++)
    {
        delete[] ref[x];
    }
    delete[] ref;
}

TEST_FIXTURE(FixedMatrixTest, OperatorEqualsInt32)
{
    FixedMatrix<int32_t> valueArrayRef1({ { 0x01, 0x02, 0x03, 0x04 },{ 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },{ 0x0D, 0x0E, 0x0F, 0x10 } });
    FixedMatrix<int32_t> valueArrayRef2({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    FixedMatrix<int32_t> valueArrayRef3({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E } });
    FixedMatrix<int32_t> valueArrayRef4({ { 0x00, 0x01 },{ 0x02, 0x03 },{ 0x04, 0x05 },{ 0x06, 0x07 },
    { 0x08, 0x09 },{ 0x0A, 0x0B },{ 0x0C, 0x0D },{ 0x0E, 0x0F } });
    FixedMatrix<int32_t> valueArrayDummy(0, 0);
    FixedMatrix<int32_t> valueArrayData({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayRef4 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(FixedMatrixTest, OperatorNotEqualsInt32)
{
    FixedMatrix<int32_t> valueArrayRef1({ { 0x01, 0x02, 0x03, 0x04 },{ 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },{ 0x0D, 0x0E, 0x0F, 0x10 } });
    FixedMatrix<int32_t> valueArrayRef2({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    FixedMatrix<int32_t> valueArrayRef3({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E } });
    FixedMatrix<int32_t> valueArrayRef4({ { 0x00, 0x01 },{ 0x02, 0x03 },{ 0x04, 0x05 },{ 0x06, 0x07 },
    { 0x08, 0x09 },{ 0x0A, 0x0B },{ 0x0C, 0x0D },{ 0x0E, 0x0F } });
    FixedMatrix<int32_t> valueArrayDummy(0, 0);
    FixedMatrix<int32_t> valueArrayData({ { 0x00, 0x01, 0x02, 0x03 },{ 0x04, 0x05, 0x06, 0x07 },
    { 0x08, 0x09, 0x0A, 0x0B },{ 0x0C, 0x0D, 0x0E, 0x0F } });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayRef4 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(FixedMatrixTest, OperatorIndexInt32)
{
    FixedMatrix<int32_t> target({ { 0x01, 0x02, 0x03, 0x04 },
    { 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },
    { 0x0D, 0x0E, 0x0F, 0x10 } });
    EXPECT_EQ(0x01, target(0, 0));
    EXPECT_EQ(0x02, target(0, 1));
    EXPECT_EQ(0x03, target(0, 2));
    EXPECT_EQ(0x04, target(0, 3));
    EXPECT_EQ(0x05, target(1, 0));
    EXPECT_EQ(0x06, target(1, 1));
    EXPECT_EQ(0x07, target(1, 2));
    EXPECT_EQ(0x08, target(1, 3));
    EXPECT_EQ(0x09, target(2, 0));
    EXPECT_EQ(0x0A, target(2, 1));
    EXPECT_EQ(0x0B, target(2, 2));
    EXPECT_EQ(0x0C, target(2, 3));
    EXPECT_EQ(0x0D, target(3, 0));
    EXPECT_EQ(0x0E, target(3, 1));
    EXPECT_EQ(0x0F, target(3, 2));
    EXPECT_EQ(0x10, target(3, 3));
    target(3, 3) = 0xFF;
    EXPECT_EQ(0xFF, target(3, 3));
    EXPECT_THROW(target(static_cast<size_t>(-1), 0), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(target(0, static_cast<size_t>(-1)), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(target(3, 4), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(target(4, 3), OSAL::ArgumentOutOfRangeException);
}

TEST_FIXTURE(FixedMatrixTest, OperatorIndexConstInt32)
{
    FixedMatrix<int32_t> target({ { 0x01, 0x02, 0x03, 0x04 },
    { 0x05, 0x06, 0x07, 0x08 },
    { 0x09, 0x0A, 0x0B, 0x0C },
    { 0x0D, 0x0E, 0x0F, 0x10 } });
    const FixedMatrix<int32_t> &targetConst = target;
    EXPECT_EQ(0x01, targetConst(0, 0));
    EXPECT_EQ(0x02, targetConst(0, 1));
    EXPECT_EQ(0x03, targetConst(0, 2));
    EXPECT_EQ(0x04, targetConst(0, 3));
    EXPECT_EQ(0x05, targetConst(1, 0));
    EXPECT_EQ(0x06, targetConst(1, 1));
    EXPECT_EQ(0x07, targetConst(1, 2));
    EXPECT_EQ(0x08, targetConst(1, 3));
    EXPECT_EQ(0x09, targetConst(2, 0));
    EXPECT_EQ(0x0A, targetConst(2, 1));
    EXPECT_EQ(0x0B, targetConst(2, 2));
    EXPECT_EQ(0x0C, targetConst(2, 3));
    EXPECT_EQ(0x0D, targetConst(3, 0));
    EXPECT_EQ(0x0E, targetConst(3, 1));
    EXPECT_EQ(0x0F, targetConst(3, 2));
    EXPECT_EQ(0x10, targetConst(3, 3));
    EXPECT_THROW(targetConst(static_cast<size_t>(-1), 0), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst(0, static_cast<size_t>(-1)), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst(3, 4), OSAL::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst(4, 3), OSAL::ArgumentOutOfRangeException);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
