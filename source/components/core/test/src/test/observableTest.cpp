#include <unittest-cpp/UnitTestC++.h>

#include "core/observable.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class observableTest : public UnitTestCpp::TestFixture
{
};

using Value = int;

TEST_FIXTURE(observableTest, Construction)
{
    observable<Value> target;
    EXPECT_EQ(size_t{0}, target.count());
}

TEST_FIXTURE(observableTest, add)
{
    int value1 = 123;
    int value2 = 234;
    observable<Value> target;

    target.add(&value1);
    target.add(&value2);
    EXPECT_EQ(size_t{2}, target.count());
    std::vector<Value *> result;
    target.forall([&result](Value * value){ result.push_back(value); });
    EXPECT_EQ(size_t{2}, result.size());
    EXPECT_EQ(&value1, result[0]);
    EXPECT_EQ(&value2, result[1]);
}

TEST_FIXTURE(observableTest, remove)
{
    int value1 = 123;
    int value2 = 234;
    int value3 = 345;
    observable<Value> target;

    target.add(&value1);
    target.add(&value2);
    target.remove(&value3);
    EXPECT_EQ(size_t{2}, target.count());
    std::vector<Value *> result;
    target.forall([&result](Value * value){ result.push_back(value); });
    EXPECT_EQ(size_t{2}, result.size());
    EXPECT_EQ(&value1, result[0]);
    EXPECT_EQ(&value2, result[1]);

    target.remove(&value1);
    EXPECT_EQ(size_t{1}, target.count());
    result.clear();
    target.forall([&result](Value * value){ result.push_back(value); });
    EXPECT_EQ(size_t{1}, result.size());
    EXPECT_EQ(&value2, result[0]);

    target.remove(&value2);
    EXPECT_EQ(size_t{0}, target.count());
    result.clear();
    target.forall([&result](Value * value){ result.push_back(value); });
    EXPECT_EQ(size_t{0}, result.size());
}

TEST_FIXTURE(observableTest, forall)
{
    int value1 = 123;
    int value2 = 234;
    observable<Value> target;

    target.add(&value1);
    target.add(&value2);
    std::vector<Value *> result;
    target.forall([&result](Value * value){ result.push_back(value); });
    EXPECT_EQ(size_t{2}, result.size());
    EXPECT_EQ(&value1, result[0]);
    EXPECT_EQ(&value2, result[1]);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
