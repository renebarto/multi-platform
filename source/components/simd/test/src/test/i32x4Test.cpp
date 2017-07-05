#include <unit-test-c++/UnitTestC++.h>

#include <simd/i32x4.h>

using namespace std;

namespace Simd {
namespace Test {

class i32x4Test : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void i32x4Test::SetUp()
{
}

void i32x4Test::TearDown()
{
}

TEST_FIXTURE(i32x4Test, Construct)
{
    i32x4 target;

//    EXPECT_EQ(f, uint8_t{0});
//    EXPECT_EQ(f, MyFlags::ZERO);
}

} // namespace Test
} // namespace Simd
