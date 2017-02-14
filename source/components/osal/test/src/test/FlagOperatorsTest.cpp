#include <unit-test-c++/UnitTestC++.h>

#undef DEFINE_FLAG_OPERATORS
#include <osal/flagoperators.h>

using namespace std;

namespace OSAL {
namespace Test {

enum class MyFlags : uint8_t
{
    ZERO = 0x00,
    ONE = 0x01,
    TWO = 0x02,
    THREE = 0x03,
    ALL_BUT_THREE = 0xFC,
    ALL_BUT_TWO = 0xFD,
    ALL_BUT_ONE = 0xFE,
    ALL = 0xFF,
};

DEFINE_FLAG_OPERATORS(MyFlags, uint8_t);

class FlagOperatorsTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void FlagOperatorsTest::SetUp()
{
}

void FlagOperatorsTest::TearDown()
{
}

//TEST_FIXTURE(FlagOperatorsTest, EnumClass)
//{
//    Flag<MyFlags> f;
//}

TEST_FIXTURE(FlagOperatorsTest, OrOperator)
{
    MyFlags actual = MyFlags::TWO | MyFlags::ONE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ONE | MyFlags::TWO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE | MyFlags::TWO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE | MyFlags::ONE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE | MyFlags::ZERO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE | MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ZERO | MyFlags::ZERO;

    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, AndOperator)
{
    MyFlags actual = MyFlags::TWO & MyFlags::ONE;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::ONE & MyFlags::TWO;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::THREE & MyFlags::TWO;

    EXPECT_EQ(MyFlags::TWO, actual);

    actual = MyFlags::THREE & MyFlags::ONE;

    EXPECT_EQ(MyFlags::ONE, actual);

    actual = MyFlags::ZERO & MyFlags::THREE;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::THREE & MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ALL & MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, XorOperator)
{
    MyFlags actual = MyFlags::TWO ^ MyFlags::ONE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ONE ^ MyFlags::TWO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE ^ MyFlags::TWO;

    EXPECT_EQ(MyFlags::ONE, actual);

    actual = MyFlags::THREE ^ MyFlags::ONE;

    EXPECT_EQ(MyFlags::TWO, actual);

    actual = MyFlags::ZERO ^ MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE ^ MyFlags::THREE;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::ALL ^ MyFlags::ALL;

    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, OrEqualsOperator)
{
    MyFlags actual = MyFlags::TWO;
    actual |= MyFlags::ONE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ONE;
    actual |= MyFlags::TWO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE;
    actual |= MyFlags::TWO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE;
    actual |= MyFlags::ONE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE;
    actual |= MyFlags::ZERO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE;
    actual |= MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ZERO;
    actual |= MyFlags::ZERO;

    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, AndEqualsOperator)
{
    MyFlags actual = MyFlags::TWO;
    actual &= MyFlags::ONE;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::ONE;
    actual &= MyFlags::TWO;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::THREE;
    actual &= MyFlags::TWO;

    EXPECT_EQ(MyFlags::TWO, actual);

    actual = MyFlags::THREE;
    actual &= MyFlags::ONE;

    EXPECT_EQ(MyFlags::ONE, actual);

    actual = MyFlags::ZERO;
    actual &= MyFlags::THREE;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::THREE;
    actual &= MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ALL;
    actual &= MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, XorEqualsOperator)
{
    MyFlags actual = MyFlags::TWO;
    actual ^= MyFlags::ONE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::ONE;
    actual ^= MyFlags::TWO;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE;
    actual ^= MyFlags::TWO;

    EXPECT_EQ(MyFlags::ONE, actual);

    actual = MyFlags::THREE;
    actual ^= MyFlags::ONE;

    EXPECT_EQ(MyFlags::TWO, actual);

    actual = MyFlags::ZERO;
    actual ^= MyFlags::THREE;

    EXPECT_EQ(MyFlags::THREE, actual);

    actual = MyFlags::THREE;
    actual ^= MyFlags::THREE;

    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = MyFlags::ALL;
    actual ^= MyFlags::ALL;

    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, BitwiseNotOperator)
{
    MyFlags actual = ~MyFlags::TWO;

    EXPECT_EQ(MyFlags::ALL_BUT_TWO, actual);

    actual = ~MyFlags::ONE;

    EXPECT_EQ(MyFlags::ALL_BUT_ONE, actual);

    actual = ~MyFlags::THREE;

    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);

    actual = ~MyFlags::ZERO;

    EXPECT_EQ(MyFlags::ALL, actual);

    actual = ~MyFlags::ALL;

    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, LogicalNotOperator)
{
    bool actual = !MyFlags::ZERO;

    EXPECT_EQ(true, actual);

    actual = !MyFlags::ONE;

    EXPECT_EQ(false, actual);

    actual = !MyFlags::ALL;

    EXPECT_EQ(false, actual);
}

TEST_FIXTURE(FlagOperatorsTest, EqualsOperator)
{
    MyFlags flags1 = MyFlags::ZERO;
    MyFlags flags2 = MyFlags::ZERO;
    MyFlags flags3 = MyFlags::ONE;
    MyFlags flags4 = MyFlags::ALL;
    uint8_t value1 = 0;
    uint8_t value2 = 1;

    EXPECT_TRUE(flags1 == flags1);
    EXPECT_TRUE(flags1 == flags2);
    EXPECT_TRUE(flags2 == flags1);
    EXPECT_FALSE(flags1 == flags3);
    EXPECT_FALSE(flags3 == flags1);
    EXPECT_FALSE(flags1 == flags4);
    EXPECT_FALSE(flags4 == flags1);
    EXPECT_TRUE(flags1 == value1);
    EXPECT_TRUE(value1 == flags1);
    EXPECT_FALSE(flags1 == value2);
    EXPECT_FALSE(value2 == flags1);
    EXPECT_TRUE(flags3 == value2);
    EXPECT_TRUE(value2 == flags3);
}

TEST_FIXTURE(FlagOperatorsTest, NotEqualsOperator)
{
    MyFlags flags1 = MyFlags::ZERO;
    MyFlags flags2 = MyFlags::ZERO;
    MyFlags flags3 = MyFlags::ONE;
    MyFlags flags4 = MyFlags::ALL;
    uint8_t value1 = 0;
    uint8_t value2 = 1;

    EXPECT_FALSE(flags1 != flags1);
    EXPECT_FALSE(flags1 != flags2);
    EXPECT_FALSE(flags2 != flags1);
    EXPECT_TRUE(flags1 != flags3);
    EXPECT_TRUE(flags3 != flags1);
    EXPECT_TRUE(flags1 != flags4);
    EXPECT_TRUE(flags4 != flags1);
    EXPECT_FALSE(flags1 != value1);
    EXPECT_FALSE(value1 != flags1);
    EXPECT_TRUE(flags1 != value2);
    EXPECT_TRUE(value2 != flags1);
    EXPECT_FALSE(flags3 != value2);
    EXPECT_FALSE(value2 != flags3);
}

TEST_FIXTURE(FlagOperatorsTest, CommonExpressions)
{
    MyFlags flags1 = MyFlags::ONE;
    MyFlags flags2 = MyFlags::TWO;

    EXPECT_TRUE((flags1 & flags2) == MyFlags::ZERO);
    EXPECT_TRUE((flags1 & flags2) == 0);
    EXPECT_TRUE(MyFlags::ZERO == (flags1 & flags2));
    EXPECT_TRUE(0 == (flags1 & flags2));
    EXPECT_TRUE((flags1 | flags2) == MyFlags::THREE);
    EXPECT_TRUE((flags1 | flags2) == 3);
    EXPECT_TRUE(MyFlags::THREE == (flags1 | flags2));
    EXPECT_TRUE(3 == (flags1 | flags2));
    EXPECT_TRUE((flags1 ^ flags2) == MyFlags::THREE);
    EXPECT_TRUE((flags1 ^ flags2) == 3);
    EXPECT_TRUE(MyFlags::THREE == (flags1 ^ flags2));
    EXPECT_TRUE(3 == (flags1 ^ flags2));
    EXPECT_TRUE((flags1 & MyFlags::THREE) == MyFlags::ONE);
    EXPECT_TRUE((flags1 | MyFlags::TWO) == MyFlags::THREE);
    EXPECT_TRUE(MyFlags::ZERO == (flags1 & flags2));
    EXPECT_TRUE((flags1 ^ MyFlags::ALL) == MyFlags::ALL_BUT_ONE);
    EXPECT_TRUE(((flags1 | flags2) & MyFlags::ALL) == (MyFlags::ONE | MyFlags::TWO));
}

} // namespace Test
} // namespace OSAL
