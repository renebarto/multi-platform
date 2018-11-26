#include <unittest-cpp/UnitTestC++.h>

#include "core/iterator.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

class iteratorTest : public UnitTestCpp::TestFixture
{
};

TEST_FIXTURE(iteratorTest, Construct)
{
    vector<int> container({1, 2, 3, 4, 5});
    iterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(iteratorTest, ConstructEmptyContainer)
{
    vector<int> container;
    iterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(iteratorTest, PrefixIncrement)
{
    vector<int> container({1, 2, 3});
    iterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(2, ++iterator);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(3, ++iterator);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_THROW(++iterator, OSAL::RuntimeError);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(iteratorTest, PostfixIncrement)
{
    vector<int> container({1, 2, 3});
    iterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(1, iterator++);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(2, iterator++);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(3, iterator++);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(iteratorTest, Reset)
{
    vector<int> container({1, 2, 3});
    iterator<vector<int>> iterator(container);
    EXPECT_EQ(1, iterator++);
    EXPECT_EQ(2, iterator++);
    EXPECT_EQ(3, iterator++);
    iterator.Reset();
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(iteratorTest, ChangeValue)
{
    vector<int> container({1, 2, 3});
    iterator<vector<int>> iterator(container);
    iterator++ = 2;
    EXPECT_EQ(2, container[0]);
    EXPECT_EQ(2, container[1]);
    EXPECT_EQ(3, container[2]);
}

class constiteratorTest : public UnitTestCpp::TestFixture
{
public:
};

TEST_FIXTURE(constiteratorTest, Construct)
{
    vector<int> container({1, 2, 3, 4, 5});
    constiterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(constiteratorTest, ConstructEmptyContainer)
{
    vector<int> container({});
    constiterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(constiteratorTest, PrefixIncrement)
{
    vector<int> container({1, 2, 3});
    constiterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(2, ++iterator);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(3, ++iterator);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_THROW(++iterator, OSAL::RuntimeError);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(constiteratorTest, PostfixIncrement)
{
    vector<int> container({1, 2, 3});
    constiterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(1, iterator++);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(2, iterator++);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
    EXPECT_EQ(3, iterator++);
    EXPECT_FALSE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(constiteratorTest, Reset)
{
    vector<int> container({1, 2, 3});
    constiterator<vector<int>> iterator(container);
    EXPECT_EQ(1, iterator++);
    EXPECT_EQ(2, iterator++);
    EXPECT_EQ(3, iterator++);
    iterator.Reset();
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(constiteratorTest, ChangeValue)
{
    vector<int> container({1, 2, 3});
    constiterator<vector<int>> iterator(container);
    // iterator++ = 2; Fails compilation
    EXPECT_EQ(1, container[0]);
    EXPECT_EQ(2, container[1]);
    EXPECT_EQ(3, container[2]);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
