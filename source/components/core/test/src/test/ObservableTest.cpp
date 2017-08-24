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

using Value = int;

TEST_FIXTURE(ObservableTest, Construction)
{
    Observable<Value> target;
    EXPECT_EQ(target.begin(), target.end());
}

TEST_FIXTURE(ObservableTest, AddObserver)
{
    int value1 = 123;
    int value2 = 234;
    Observable<Value> target;

    target.AddObserver(&value1);
    target.AddObserver(&value2);
    EXPECT_NE(target.begin(), target.end());
    auto iterator = target.begin();
    EXPECT_EQ(&value1, *iterator);
    ++iterator;
    EXPECT_EQ(&value2, *iterator);
    ++iterator;
    EXPECT_EQ(target.end(), iterator);
}

TEST_FIXTURE(ObservableTest, RemoveObserver)
{
    int value1 = 123;
    int value2 = 234;
    int value3 = 345;
    Observable<Value> target;

    target.AddObserver(&value1);
    target.AddObserver(&value2);
    target.RemoveObserver(&value3);
    EXPECT_NE(target.begin(), target.end());
    auto iterator = target.begin();
    EXPECT_EQ(&value1, *iterator);
    ++iterator;
    EXPECT_EQ(&value2, *iterator);
    ++iterator;
    EXPECT_EQ(target.end(), iterator);

    target.RemoveObserver(&value1);
    EXPECT_NE(target.begin(), target.end());
    iterator = target.begin();
    EXPECT_EQ(&value2, *iterator);
    ++iterator;
    EXPECT_EQ(target.end(), iterator);

    target.RemoveObserver(&value2);
    EXPECT_EQ(target.begin(), target.end());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
