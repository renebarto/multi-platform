#include <unittest-cpp/UnitTestC++.h>

#include <type_traits>
#include <osal/FlagOperators.h>
#include <core/serialization/Serialization.h>
#include <core/serialization/SerializationImpl.h>

using namespace std;

namespace OSAL {
namespace Test {

enum class MyFlags : uint8_t
{
    ZERO = 0x00,
    ONE = 0x01,
    TWO = 0x02,
    THREE = 0x03,
    ALL_BUTHREE = 0xFC,
    ALL_BUTWO = 0xFD,
    ALL_BUT_ONE = 0xFE,
    ALL = 0xFF,
};

} // namespace Test

template<>
struct is_flag<Test::MyFlags>
{
    static constexpr bool value=true;
};

} // namespace OSAL


WARNING_PUSH
WARNING_DISABLE(4592)
template<>
std::vector<Core::EnumConversion<OSAL::Test::MyFlags>> Core::EnumSerializationInfo<OSAL::Test::MyFlags>::Info =
{
    { OSAL::Test::MyFlags::ZERO, "ZERO" },
    { OSAL::Test::MyFlags::ONE, "ONE" },
    { OSAL::Test::MyFlags::TWO, "TWO" },
    { OSAL::Test::MyFlags::THREE, "THREE" },
    { OSAL::Test::MyFlags::ALL_BUTHREE, "ALL_BUT_THREE" },
    { OSAL::Test::MyFlags::ALL_BUTWO, "ALL_BUT_TWO" },
    { OSAL::Test::MyFlags::ALL_BUT_ONE, "ALL_BUT_ONE" },
    { OSAL::Test::MyFlags::ALL, "ALL" },
};
WARNING_POP

namespace OSAL {
namespace Test {

inline std::ostream & operator << (std::ostream & stream, const MyFlags & value)
{
    stream << Core::Serialize(value);
    return stream;
}

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

TEST_SUITE(osal)
{

TEST_FIXTURE(FlagOperatorsTest, OrOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;

    MyFlags actual = valueTwo | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_B | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo | valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo | valueOne;

    actual = valueOne | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne | valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree | valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueZero_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree | valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueZero | valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_B | valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero | valueZero_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, AndOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    uint8_t valueAll_B = 255;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;
    MyFlags valueAll = MyFlags::ALL;

    MyFlags actual = valueTwo & valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo_B & valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo & valueOne_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueOne & valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne_B & valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne & valueTwo_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueThree & valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_B & valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree & valueTwo_B;
    EXPECT_EQ(MyFlags::TWO, actual);

    actual = valueThree & valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_B & valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree & valueOne_B;
    EXPECT_EQ(MyFlags::ONE, actual);

    actual = valueZero & valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_B & valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero & valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueThree & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree & valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueAll & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll_B & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll & valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, XorOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    uint8_t valueAll_B = 255;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;
    MyFlags valueAll = MyFlags::ALL;

    MyFlags actual = valueTwo ^ valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo ^ valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo ^ valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne ^ valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B ^ valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne ^ valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree ^ valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_B ^ valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree ^ valueTwo_B;
    EXPECT_EQ(MyFlags::ONE, actual);

    actual = valueThree ^ valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_B ^ valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree ^ valueOne_B;
    EXPECT_EQ(MyFlags::TWO, actual);

    actual = valueZero ^ valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero_B ^ valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero ^ valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree ^ valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree_B ^ valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree ^ valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueAll ^ valueThree;
    EXPECT_EQ(MyFlags::ALL_BUTHREE, actual);
    actual = valueAll_B ^ valueThree;
    EXPECT_EQ(MyFlags::ALL_BUTHREE, actual);
    actual = valueAll ^ valueThree_B;
    EXPECT_EQ(MyFlags::ALL_BUTHREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, OrEqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;

    MyFlags actual = valueTwo;
    actual |= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo;
    actual |= valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual |= valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual |= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueZero_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueZero;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero;
    actual |= valueZero_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, AndEqualsOperator)
{
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;
    MyFlags valueAll = MyFlags::ALL;

    MyFlags actual = valueTwo;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo;
    actual &= valueOne_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueOne;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne;
    actual &= valueTwo_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueThree;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree;
    actual &= valueTwo_B;
    EXPECT_EQ(MyFlags::TWO, actual);

    actual = valueThree;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree;
    actual &= valueOne_B;
    EXPECT_EQ(MyFlags::ONE, actual);

    actual = valueZero;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero;
    actual &= valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueThree;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual &= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueAll;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll;
    actual &= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, XorEqualsOperator)
{
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;
    MyFlags valueAll = MyFlags::ALL;

    MyFlags actual = valueTwo;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo;
    actual ^= valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual ^= valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree;
    actual ^= valueTwo_B;
    EXPECT_EQ(MyFlags::ONE, actual);

    actual = valueThree;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree;
    actual ^= valueOne_B;
    EXPECT_EQ(MyFlags::TWO, actual);

    actual = valueZero;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero;
    actual ^= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree;
    actual ^= valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueAll;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ALL_BUTHREE, actual);
    actual = valueAll;
    actual ^= valueThree_B;
    EXPECT_EQ(MyFlags::ALL_BUTHREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, BitwiseNotOperator)
{
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueTwo = MyFlags::TWO;
    MyFlags valueThree = MyFlags::THREE;
    MyFlags valueAllButOne = MyFlags::ALL_BUT_ONE;
    MyFlags valueAllButTwo = MyFlags::ALL_BUTWO;
    MyFlags valueAllButThree = MyFlags::ALL_BUTHREE;
    MyFlags valueAll = MyFlags::ALL;

    MyFlags actual = ~valueOne;
    EXPECT_EQ(valueAllButOne, actual);

    actual = ~valueTwo;
    EXPECT_EQ(valueAllButTwo, actual);

    actual = ~valueThree;
    EXPECT_EQ(valueAllButThree, actual);

    actual = ~valueAll;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = ~valueZero;
    EXPECT_EQ(MyFlags::ALL, actual);
}

TEST_FIXTURE(FlagOperatorsTest, LogicalNotOperator)
{
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueAll = MyFlags::ALL;

    bool actual = !valueZero;
    EXPECT_TRUE(actual);

    actual = !valueOne;
    EXPECT_FALSE(actual);

    actual = !valueAll;
    EXPECT_FALSE(actual);
}

TEST_FIXTURE(FlagOperatorsTest, EqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueAll_B = 255;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueAll = MyFlags::ALL;

    EXPECT_TRUE(valueZero == valueZero);
    EXPECT_TRUE(valueZero_B == valueZero);
    EXPECT_TRUE(valueZero == valueZero_B);

    EXPECT_FALSE(valueZero == valueOne);
    EXPECT_FALSE(valueOne == valueZero);
    EXPECT_FALSE(valueZero == valueOne_B);
    EXPECT_FALSE(valueOne_B == valueZero);

    EXPECT_FALSE(valueZero == valueAll);
    EXPECT_FALSE(valueAll == valueZero);
    EXPECT_FALSE(valueZero == valueAll_B);
    EXPECT_FALSE(valueAll_B == valueZero);

    EXPECT_FALSE(valueAll == valueOne);
    EXPECT_FALSE(valueOne == valueAll);
    EXPECT_FALSE(valueAll == valueOne_B);
    EXPECT_FALSE(valueOne_B == valueAll);
}

TEST_FIXTURE(FlagOperatorsTest, NotEqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueAll_B = 255;
    MyFlags valueZero = MyFlags::ZERO;
    MyFlags valueOne = MyFlags::ONE;
    MyFlags valueAll = MyFlags::ALL;

    EXPECT_FALSE(valueZero != valueZero);
    EXPECT_FALSE(valueZero_B != valueZero);
    EXPECT_FALSE(valueZero != valueZero_B);

    EXPECT_TRUE(valueZero != valueOne);
    EXPECT_TRUE(valueOne != valueZero);
    EXPECT_TRUE(valueZero != valueOne_B);
    EXPECT_TRUE(valueOne_B != valueZero);

    EXPECT_TRUE(valueZero != valueAll);
    EXPECT_TRUE(valueAll != valueZero);
    EXPECT_TRUE(valueZero != valueAll_B);
    EXPECT_TRUE(valueAll_B != valueZero);

    EXPECT_TRUE(valueAll != valueOne);
    EXPECT_TRUE(valueOne != valueAll);
    EXPECT_TRUE(valueAll != valueOne_B);
    EXPECT_TRUE(valueOne_B != valueAll);
}

TEST_FIXTURE(FlagOperatorsTest, CommonExpressions)
{
    MyFlags flags1 = MyFlags::ONE;
    MyFlags flags2 = MyFlags::TWO;

    EXPECT_TRUE((flags1 & flags2) == MyFlags::ZERO);
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

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
