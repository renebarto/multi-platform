#include <unit-test-c++/UnitTestC++.h>

#include "core/Nullable.h"
#include "core/Util.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class NullableTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

};

void NullableTest::SetUp()
{
}

void NullableTest::TearDown()
{
}

TEST_FIXTURE(NullableTest, Construction)
{
    FAIL();
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
