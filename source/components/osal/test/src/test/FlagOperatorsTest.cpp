#include <unit-test-c++/UnitTestC++.h>

#include <osal/flagoperators.h>
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
    ALL_BUT_THREE = 0xFC,
    ALL_BUT_TWO = 0xFD,
    ALL_BUT_ONE = 0xFE,
    ALL = 0xFF,
};

DEFINE_FLAG_OPERATORS(MyFlags, uint8_t);

using MyFlag = Flag<MyFlags, uint8_t>;

template<>
std::vector<Core::EnumConversion<MyFlags>> Core::EnumSerializationInfo<OSAL::Test::MyFlags>::Info =
{
    { OSAL::Test::MyFlags::ZERO, "ZERO" },
    { OSAL::Test::MyFlags::ONE, "ONE" },
    { OSAL::Test::MyFlags::TWO, "TWO" },
    { OSAL::Test::MyFlags::THREE, "THREE" },
    { OSAL::Test::MyFlags::ALL_BUT_THREE, "ALL_BUT_THREE" },
    { OSAL::Test::MyFlags::ALL_BUT_TWO, "ALL_BUT_TWO" },
    { OSAL::Test::MyFlags::ALL_BUT_ONE, "ALL_BUT_ONE" },
    { OSAL::Test::MyFlags::ALL, "ALL" },
};

inline std::basic_ostream<OSAL::Char> & operator << (std::basic_ostream<OSAL::Char> & stream, const MyFlags & value)
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

TEST_FIXTURE(FlagOperatorsTest, Construct)
{
    MyFlag f;

    EXPECT_EQ(f, uint8_t{0});
    EXPECT_EQ(f, MyFlags::ZERO);
}

TEST_FIXTURE(FlagOperatorsTest, ConstructInitMyFlags)
{
    MyFlag f(MyFlags::ONE);

    EXPECT_EQ(f, uint8_t{1});
    EXPECT_EQ(f, MyFlags::ONE);
}

TEST_FIXTURE(FlagOperatorsTest, ConstructInitUInt8)
{
    MyFlag f(2);

    EXPECT_EQ(f, uint8_t{2});
    EXPECT_EQ(f, MyFlags::TWO);
}

TEST_FIXTURE(FlagOperatorsTest, Assignment)
{
    MyFlag other(MyFlags::ONE);
    MyFlag f;
    f = other;

    EXPECT_EQ(f, uint8_t{1});
    EXPECT_EQ(f, MyFlags::ONE);
}

TEST_FIXTURE(FlagOperatorsTest, AssignmentMyFlags)
{
    MyFlag f;
    f = MyFlags::ONE;

    EXPECT_EQ(f, uint8_t{1});
    EXPECT_EQ(f, MyFlags::ONE);
}

TEST_FIXTURE(FlagOperatorsTest, AssignmentUInt8)
{
    MyFlag f;
    f = 2;

    EXPECT_EQ(f, uint8_t{2});
    EXPECT_EQ(f, MyFlags::TWO);
}

TEST_FIXTURE(FlagOperatorsTest, OrOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);

    MyFlag actual = valueTwo | valueOne_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_T | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_B | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo | valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo | valueOne;

    actual = valueOne | valueTwo_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_T | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne | valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne | valueTwo;

    actual = valueThree | valueTwo_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueTwo;

    actual = valueThree | valueOne_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueOne;

    actual = valueThree | valueZero_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T | valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueZero_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueZero;

    actual = valueThree | valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T | valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B | valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree | valueThree;

    actual = valueZero | valueZero_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_T | valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_B | valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero | valueZero_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero | valueZero;
}

TEST_FIXTURE(FlagOperatorsTest, AndOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    uint8_t valueAll_B = 255;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);
    MyFlag valueAll(valueAll_T);

    MyFlag actual = valueTwo & valueOne_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo_T & valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo_B & valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo & valueOne_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo & valueOne;

    actual = valueOne & valueTwo_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne_T & valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne_B & valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne & valueTwo_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne & valueTwo;

    actual = valueThree & valueTwo_T;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_T & valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_B & valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree & valueTwo_B;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree & valueTwo;

    actual = valueThree & valueOne_T;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_T & valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_B & valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree & valueOne_B;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree & valueOne;

    actual = valueZero & valueThree_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_T & valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_B & valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero & valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero & valueThree;

    actual = valueThree & valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree & valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree & valueThree;

    actual = valueAll & valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll_T & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll_B & valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll & valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll & valueThree;
}

TEST_FIXTURE(FlagOperatorsTest, XorOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    uint8_t valueAll_B = 255;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);
    MyFlag valueAll(valueAll_T);

    MyFlag actual = valueTwo ^ valueOne_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_T ^ valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_B ^ valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo ^ valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo ^ valueOne;

    actual = valueOne ^ valueTwo_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_T ^ valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B ^ valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne ^ valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne ^ valueTwo;

    actual = valueThree ^ valueTwo_T;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_T ^ valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_B ^ valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree ^ valueTwo_B;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree ^ valueTwo;

    actual = valueThree ^ valueOne_T;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_T ^ valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_B ^ valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree ^ valueOne_B;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree ^ valueOne;

    actual = valueZero ^ valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero_T ^ valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero_B ^ valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero ^ valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero ^ valueThree;

    actual = valueThree ^ valueThree_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree_T ^ valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree_B ^ valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree ^ valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree ^ valueThree;

    actual = valueAll ^ valueThree_T;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll_T ^ valueThree;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll_B ^ valueThree;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll ^ valueThree_B;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll ^ valueThree;
}

TEST_FIXTURE(FlagOperatorsTest, OrEqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);

    MyFlag actual = valueTwo;
    actual |= valueOne_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_T;
    actual |= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_B;
    actual |= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo;
    actual |= valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo;
    actual |= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne;
    actual |= valueTwo_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_T;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual |= valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual |= valueTwo_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne;
    actual |= valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_T;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual |= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual |= valueZero_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueZero_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual |= valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual |= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueZero;
    actual |= valueZero_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_T;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_B;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero;
    actual |= valueZero_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero;
    actual |= valueZero;
    EXPECT_EQ(MyFlags::ZERO, actual);
}

TEST_FIXTURE(FlagOperatorsTest, AndEqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    uint8_t valueAll_B = 255;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);
    MyFlag valueAll(valueAll_T);

    MyFlag actual = valueTwo;
    actual &= valueOne_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo_T;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo_B;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo;
    actual &= valueOne_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueTwo;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueOne;
    actual &= valueTwo_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne_T;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne_B;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne;
    actual &= valueTwo_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueOne;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueThree;
    actual &= valueTwo_T;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_T;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_B;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree;
    actual &= valueTwo_B;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree;
    actual &= valueTwo;
    EXPECT_EQ(MyFlags::TWO, actual);

    actual = valueThree;
    actual &= valueOne_T;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_T;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_B;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree;
    actual &= valueOne_B;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree;
    actual &= valueOne;
    EXPECT_EQ(MyFlags::ONE, actual);

    actual = valueZero;
    actual &= valueThree_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_T;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero_B;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero;
    actual &= valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueZero;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueThree;
    actual &= valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_T;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree_B;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual &= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueThree;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueAll;
    actual &= valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll_T;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll_B;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll;
    actual &= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueAll;
    actual &= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, XorEqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueTwo_B = 2;
    uint8_t valueThree_B = 3;
    uint8_t valueAll_B = 255;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);
    MyFlag valueAll(valueAll_T);

    MyFlag actual = valueTwo;
    actual ^= valueOne_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_T;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo_B;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo;
    actual ^= valueOne_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueTwo;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueOne;
    actual ^= valueTwo_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_T;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne_B;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual ^= valueTwo_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueOne;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual ^= valueTwo_T;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_T;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree_B;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree;
    actual ^= valueTwo_B;
    EXPECT_EQ(MyFlags::ONE, actual);
    actual = valueThree;
    actual ^= valueTwo;
    EXPECT_EQ(MyFlags::ONE, actual);

    actual = valueThree;
    actual ^= valueOne_T;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_T;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree_B;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree;
    actual ^= valueOne_B;
    EXPECT_EQ(MyFlags::TWO, actual);
    actual = valueThree;
    actual ^= valueOne;
    EXPECT_EQ(MyFlags::TWO, actual);

    actual = valueZero;
    actual ^= valueThree_T;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero_T;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero_B;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero;
    actual ^= valueThree_B;
    EXPECT_EQ(MyFlags::THREE, actual);
    actual = valueZero;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::THREE, actual);

    actual = valueThree;
    actual ^= valueThree_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree_T;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree_B;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree;
    actual ^= valueThree_B;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = valueThree;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = valueAll;
    actual ^= valueThree_T;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll_T;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll_B;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll;
    actual ^= valueThree_B;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = valueAll;
    actual ^= valueThree;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
}

TEST_FIXTURE(FlagOperatorsTest, BitwiseNotOperator)
{
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueTwo_T = MyFlags::TWO;
    MyFlags valueThree_T = MyFlags::THREE;
    MyFlags valueAllButOne_T = MyFlags::ALL_BUT_ONE;
    MyFlags valueAllButTwo_T = MyFlags::ALL_BUT_TWO;
    MyFlags valueAllButThree_T = MyFlags::ALL_BUT_THREE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueTwo(valueTwo_T);
    MyFlag valueThree(valueThree_T);
    MyFlag valueAllButOne(valueAllButOne_T);
    MyFlag valueAllButTwo(valueAllButTwo_T);
    MyFlag valueAllButThree(valueAllButThree_T);
    MyFlag valueAll(valueAll_T);

    MyFlag actual = ~valueOne_T;
    EXPECT_EQ(MyFlags::ALL_BUT_ONE, actual);
    actual = ~valueOne;
    EXPECT_EQ(MyFlags::ALL_BUT_ONE, actual);

    actual = ~valueTwo_T;
    EXPECT_EQ(MyFlags::ALL_BUT_TWO, actual);
    actual = ~valueTwo;
    EXPECT_EQ(MyFlags::ALL_BUT_TWO, actual);

    actual = ~valueThree_T;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
    actual = ~valueThree;
    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);

    actual = ~valueAll_T;
    EXPECT_EQ(MyFlags::ZERO, actual);
    actual = ~valueAll;
    EXPECT_EQ(MyFlags::ZERO, actual);

    actual = ~static_cast<MyFlag>(valueZero_T);
    EXPECT_EQ(MyFlags::ALL, actual);
    actual = ~valueZero;
    EXPECT_EQ(MyFlags::ALL, actual);
}

TEST_FIXTURE(FlagOperatorsTest, LogicalNotOperator)
{
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueAll(valueAll_T);

    bool actual = !valueZero_T;
    EXPECT_TRUE(actual);

    actual = !valueZero;
    EXPECT_TRUE(actual);

    actual = !valueOne_T;
    EXPECT_FALSE(actual);

    actual = !valueOne;
    EXPECT_FALSE(actual);

    actual = !valueAll_T;
    EXPECT_FALSE(actual);

    actual = !valueAll;
    EXPECT_FALSE(actual);
}

TEST_FIXTURE(FlagOperatorsTest, SafeBoolOperator)
{
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero(valueZero_T);
    MyFlag valueOne(valueOne_T);
    MyFlag valueAll(valueAll_T);

    bool actual = static_cast<MyFlag>(valueZero_T);
    EXPECT_FALSE(actual);

    actual = valueZero;
    EXPECT_FALSE(actual);

    actual = static_cast<MyFlag>(valueOne_T);
    EXPECT_TRUE(actual);

    actual = valueOne;
    EXPECT_TRUE(actual);

    actual = static_cast<MyFlag>(valueAll_T);
    EXPECT_TRUE(actual);

    actual = valueAll;
    EXPECT_TRUE(actual);
}

TEST_FIXTURE(FlagOperatorsTest, EqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueAll_B = 255;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero = MyFlags::ZERO;
    MyFlag valueZero2 = MyFlags::ZERO;
    MyFlag valueOne = MyFlags::ONE;
    MyFlag valueAll = MyFlags::ALL;

    EXPECT_TRUE(valueZero == valueZero);
    EXPECT_TRUE(valueZero2 == valueZero);
    EXPECT_TRUE(valueZero == valueZero2);
    EXPECT_TRUE(valueZero_T == valueZero);
    EXPECT_TRUE(valueZero == valueZero_T);
    EXPECT_TRUE(valueZero_B == valueZero);
    EXPECT_TRUE(valueZero == valueZero_B);

    EXPECT_FALSE(valueZero == valueOne);
    EXPECT_FALSE(valueOne == valueZero);
    EXPECT_FALSE(valueZero == valueOne_T);
    EXPECT_FALSE(valueOne_T == valueZero);
    EXPECT_FALSE(valueZero == valueOne_B);
    EXPECT_FALSE(valueOne_B == valueZero);

    EXPECT_FALSE(valueZero == valueAll);
    EXPECT_FALSE(valueAll == valueZero);
    EXPECT_FALSE(valueZero == valueAll_T);
    EXPECT_FALSE(valueAll_T == valueZero);
    EXPECT_FALSE(valueZero == valueAll_B);
    EXPECT_FALSE(valueAll_B == valueZero);

    EXPECT_FALSE(valueAll == valueOne);
    EXPECT_FALSE(valueOne == valueAll);
    EXPECT_FALSE(valueAll == valueOne_T);
    EXPECT_FALSE(valueOne_T == valueAll);
    EXPECT_FALSE(valueAll == valueOne_B);
    EXPECT_FALSE(valueOne_B == valueAll);
}

TEST_FIXTURE(FlagOperatorsTest, NotEqualsOperator)
{
    uint8_t valueZero_B = 0;
    uint8_t valueOne_B = 1;
    uint8_t valueAll_B = 255;
    MyFlags valueZero_T = MyFlags::ZERO;
    MyFlags valueOne_T = MyFlags::ONE;
    MyFlags valueAll_T = MyFlags::ALL;
    MyFlag valueZero = MyFlags::ZERO;
    MyFlag valueZero2 = MyFlags::ZERO;
    MyFlag valueOne = MyFlags::ONE;
    MyFlag valueAll = MyFlags::ALL;

    EXPECT_FALSE(valueZero != valueZero);
    EXPECT_FALSE(valueZero2 != valueZero);
    EXPECT_FALSE(valueZero != valueZero2);
    EXPECT_FALSE(valueZero_T != valueZero);
    EXPECT_FALSE(valueZero != valueZero_T);
    EXPECT_FALSE(valueZero_B != valueZero);
    EXPECT_FALSE(valueZero != valueZero_B);

    EXPECT_TRUE(valueZero != valueOne);
    EXPECT_TRUE(valueOne != valueZero);
    EXPECT_TRUE(valueZero != valueOne_T);
    EXPECT_TRUE(valueOne_T != valueZero);
    EXPECT_TRUE(valueZero != valueOne_B);
    EXPECT_TRUE(valueOne_B != valueZero);

    EXPECT_TRUE(valueZero != valueAll);
    EXPECT_TRUE(valueAll != valueZero);
    EXPECT_TRUE(valueZero != valueAll_T);
    EXPECT_TRUE(valueAll_T != valueZero);
    EXPECT_TRUE(valueZero != valueAll_B);
    EXPECT_TRUE(valueAll_B != valueZero);

    EXPECT_TRUE(valueAll != valueOne);
    EXPECT_TRUE(valueOne != valueAll);
    EXPECT_TRUE(valueAll != valueOne_T);
    EXPECT_TRUE(valueOne_T != valueAll);
    EXPECT_TRUE(valueAll != valueOne_B);
    EXPECT_TRUE(valueOne_B != valueAll);
}

TEST_FIXTURE(FlagOperatorsTest, CommonExpressions)
{
    MyFlag flags1 = MyFlags::ONE;
    MyFlag flags2 = MyFlags::TWO;

    EXPECT_TRUE((flags1 & flags2) == MyFlags::ZERO);
    EXPECT_TRUE(MyFlag(flags1 & flags2) == 0);
    EXPECT_TRUE(MyFlags::ZERO == (flags1 & flags2));
    EXPECT_TRUE(0 == (flags1 & flags2));
//    EXPECT_TRUE((flags1 | flags2) == MyFlags::THREE);
//    EXPECT_TRUE((flags1 | flags2) == 3);
//    EXPECT_TRUE(MyFlags::THREE == (flags1 | flags2));
//    EXPECT_TRUE(3 == (flags1 | flags2));
//    EXPECT_TRUE((flags1 ^ flags2) == MyFlags::THREE);
//    EXPECT_TRUE((flags1 ^ flags2) == 3);
//    EXPECT_TRUE(MyFlags::THREE == (flags1 ^ flags2));
//    EXPECT_TRUE(3 == (flags1 ^ flags2));
//    EXPECT_TRUE((flags1 & MyFlags::THREE) == MyFlags::ONE);
//    EXPECT_TRUE((flags1 | MyFlags::TWO) == MyFlags::THREE);
//    EXPECT_TRUE(MyFlags::ZERO == (flags1 & flags2));
//    EXPECT_TRUE((flags1 ^ MyFlags::ALL) == MyFlags::ALL_BUT_ONE);
//    EXPECT_TRUE(((flags1 | flags2) & MyFlags::ALL) == (MyFlags::ONE | MyFlags::TWO));
}

//TEST_FIXTURE(FlagOperatorsTest, DefineOrOperator)
//{
//    MyFlags actual = MyFlags::TWO | MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ONE | MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE | MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE | MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE | MyFlags::ZERO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE | MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ZERO | MyFlags::ZERO;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, DefineAndOperator)
//{
//    MyFlags actual = MyFlags::TWO & MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::ONE & MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::THREE & MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::TWO, actual);
//
//    actual = MyFlags::THREE & MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::ONE, actual);
//
//    actual = MyFlags::ZERO & MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::THREE & MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ALL & MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, DefineXorOperator)
//{
//    MyFlags actual = MyFlags::TWO ^ MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ONE ^ MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE ^ MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::ONE, actual);
//
//    actual = MyFlags::THREE ^ MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::TWO, actual);
//
//    actual = MyFlags::ZERO ^ MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE ^ MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::ALL ^ MyFlags::ALL;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, OrEqualsOperator)
//{
//    MyFlags actual = MyFlags::TWO;
//    actual |= MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ONE;
//    actual |= MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE;
//    actual |= MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE;
//    actual |= MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE;
//    actual |= MyFlags::ZERO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE;
//    actual |= MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ZERO;
//    actual |= MyFlags::ZERO;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, AndEqualsOperator)
//{
//    MyFlags actual = MyFlags::TWO;
//    actual &= MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::ONE;
//    actual &= MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::THREE;
//    actual &= MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::TWO, actual);
//
//    actual = MyFlags::THREE;
//    actual &= MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::ONE, actual);
//
//    actual = MyFlags::ZERO;
//    actual &= MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::THREE;
//    actual &= MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ALL;
//    actual &= MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, XorEqualsOperator)
//{
//    MyFlags actual = MyFlags::TWO;
//    actual ^= MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::ONE;
//    actual ^= MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE;
//    actual ^= MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::ONE, actual);
//
//    actual = MyFlags::THREE;
//    actual ^= MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::TWO, actual);
//
//    actual = MyFlags::ZERO;
//    actual ^= MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::THREE, actual);
//
//    actual = MyFlags::THREE;
//    actual ^= MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//
//    actual = MyFlags::ALL;
//    actual ^= MyFlags::ALL;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, BitwiseNotOperator)
//{
//    MyFlags actual = ~MyFlags::TWO;
//
//    EXPECT_EQ(MyFlags::ALL_BUT_TWO, actual);
//
//    actual = ~MyFlags::ONE;
//
//    EXPECT_EQ(MyFlags::ALL_BUT_ONE, actual);
//
//    actual = ~MyFlags::THREE;
//
//    EXPECT_EQ(MyFlags::ALL_BUT_THREE, actual);
//
//    actual = ~MyFlags::ZERO;
//
//    EXPECT_EQ(MyFlags::ALL, actual);
//
//    actual = ~MyFlags::ALL;
//
//    EXPECT_EQ(MyFlags::ZERO, actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, LogicalNotOperator)
//{
//    bool actual = !MyFlags::ZERO;
//
//    EXPECT_TRUE(actual);
//
//    actual = !MyFlags::ONE;
//
//    EXPECT_FALSE(actual);
//
//    actual = !MyFlags::ALL;
//
//    EXPECT_FALSE(actual);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, EqualsOperator)
//{
//    MyFlags flags1 = MyFlags::ZERO;
//    MyFlags flags2 = MyFlags::ZERO;
//    MyFlags flags3 = MyFlags::ONE;
//    MyFlags flags4 = MyFlags::ALL;
//    uint8_t value1 = 0;
//    uint8_t value2 = 1;
//
//    EXPECT_TRUE(flags1 == flags1);
//    EXPECT_TRUE(flags1 == flags2);
//    EXPECT_TRUE(flags2 == flags1);
//    EXPECT_FALSE(flags1 == flags3);
//    EXPECT_FALSE(flags3 == flags1);
//    EXPECT_FALSE(flags1 == flags4);
//    EXPECT_FALSE(flags4 == flags1);
//    EXPECT_TRUE(flags1 == value1);
//    EXPECT_TRUE(value1 == flags1);
//    EXPECT_FALSE(flags1 == value2);
//    EXPECT_FALSE(value2 == flags1);
//    EXPECT_TRUE(flags3 == value2);
//    EXPECT_TRUE(value2 == flags3);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, NotEqualsOperator)
//{
//    MyFlags flags1 = MyFlags::ZERO;
//    MyFlags flags2 = MyFlags::ZERO;
//    MyFlags flags3 = MyFlags::ONE;
//    MyFlags flags4 = MyFlags::ALL;
//    uint8_t value1 = 0;
//    uint8_t value2 = 1;
//
//    EXPECT_FALSE(flags1 != flags1);
//    EXPECT_FALSE(flags1 != flags2);
//    EXPECT_FALSE(flags2 != flags1);
//    EXPECT_TRUE(flags1 != flags3);
//    EXPECT_TRUE(flags3 != flags1);
//    EXPECT_TRUE(flags1 != flags4);
//    EXPECT_TRUE(flags4 != flags1);
//    EXPECT_FALSE(flags1 != value1);
//    EXPECT_FALSE(value1 != flags1);
//    EXPECT_TRUE(flags1 != value2);
//    EXPECT_TRUE(value2 != flags1);
//    EXPECT_FALSE(flags3 != value2);
//    EXPECT_FALSE(value2 != flags3);
//}
//
//TEST_FIXTURE(FlagOperatorsTest, CommonExpressions)
//{
//    MyFlags flags1 = MyFlags::ONE;
//    MyFlags flags2 = MyFlags::TWO;
//
//    EXPECT_TRUE((flags1 & flags2) == MyFlags::ZERO);
//    EXPECT_TRUE((flags1 & flags2) == 0);
//    EXPECT_TRUE(MyFlags::ZERO == (flags1 & flags2));
//    EXPECT_TRUE(0 == (flags1 & flags2));
//    EXPECT_TRUE((flags1 | flags2) == MyFlags::THREE);
//    EXPECT_TRUE((flags1 | flags2) == 3);
//    EXPECT_TRUE(MyFlags::THREE == (flags1 | flags2));
//    EXPECT_TRUE(3 == (flags1 | flags2));
//    EXPECT_TRUE((flags1 ^ flags2) == MyFlags::THREE);
//    EXPECT_TRUE((flags1 ^ flags2) == 3);
//    EXPECT_TRUE(MyFlags::THREE == (flags1 ^ flags2));
//    EXPECT_TRUE(3 == (flags1 ^ flags2));
//    EXPECT_TRUE((flags1 & MyFlags::THREE) == MyFlags::ONE);
//    EXPECT_TRUE((flags1 | MyFlags::TWO) == MyFlags::THREE);
//    EXPECT_TRUE(MyFlags::ZERO == (flags1 & flags2));
//    EXPECT_TRUE((flags1 ^ MyFlags::ALL) == MyFlags::ALL_BUT_ONE);
//    EXPECT_TRUE(((flags1 | flags2) & MyFlags::ALL) == (MyFlags::ONE | MyFlags::TWO));
//}

} // namespace Test
} // namespace OSAL
