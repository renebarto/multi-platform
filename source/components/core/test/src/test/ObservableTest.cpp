#include <unit-test-c++/UnitTestC++.h>

#include "core/Observable.h"
#include "core/Util.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class ObservableTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ObservableTest::SetUp()
{
}

void ObservableTest::TearDown()
{
}

TEST_FIXTURE(ObservableTest, Construction)
{
    FAIL();
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
