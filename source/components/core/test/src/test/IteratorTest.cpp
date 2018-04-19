#include <unittest-cpp/UnitTestC++.h>

#include "core/Iterator.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

class IteratorTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

};

void IteratorTest::SetUp()
{
}

void IteratorTest::TearDown()
{
}

TEST_FIXTURE(IteratorTest, Construct)
{
    vector<int> container({1, 2, 3, 4, 5});
    Iterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(IteratorTest, ConstructEmptyContainer)
{
    vector<int> container({});
    Iterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(IteratorTest, PrefixIncrement)
{
    vector<int> container({1, 2, 3});
    Iterator<vector<int>> iterator(container);
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

TEST_FIXTURE(IteratorTest, PostfixIncrement)
{
    vector<int> container({1, 2, 3});
    Iterator<vector<int>> iterator(container);
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

TEST_FIXTURE(IteratorTest, Reset)
{
    vector<int> container({1, 2, 3});
    Iterator<vector<int>> iterator(container);
    EXPECT_EQ(1, iterator++);
    EXPECT_EQ(2, iterator++);
    EXPECT_EQ(3, iterator++);
    iterator.Reset();
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(IteratorTest, ChangeValue)
{
    vector<int> container({1, 2, 3});
    Iterator<vector<int>> iterator(container);
    iterator++ = 2;
    EXPECT_EQ(2, container[0]);
    EXPECT_EQ(2, container[1]);
    EXPECT_EQ(3, container[2]);
}

class ConstIteratorTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

};

void ConstIteratorTest::SetUp()
{
}

void ConstIteratorTest::TearDown()
{
}

TEST_FIXTURE(ConstIteratorTest, Construct)
{
    vector<int> container({1, 2, 3, 4, 5});
    ConstIterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(ConstIteratorTest, ConstructEmptyContainer)
{
    vector<int> container({});
    ConstIterator<vector<int>> iterator(container);
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_TRUE(iterator.AtEnd());
}

TEST_FIXTURE(ConstIteratorTest, PrefixIncrement)
{
    vector<int> container({1, 2, 3});
    ConstIterator<vector<int>> iterator(container);
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

TEST_FIXTURE(ConstIteratorTest, PostfixIncrement)
{
    vector<int> container({1, 2, 3});
    ConstIterator<vector<int>> iterator(container);
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

TEST_FIXTURE(ConstIteratorTest, Reset)
{
    vector<int> container({1, 2, 3});
    ConstIterator<vector<int>> iterator(container);
    EXPECT_EQ(1, iterator++);
    EXPECT_EQ(2, iterator++);
    EXPECT_EQ(3, iterator++);
    iterator.Reset();
    EXPECT_TRUE(iterator.AtBegin());
    EXPECT_FALSE(iterator.AtEnd());
}

TEST_FIXTURE(ConstIteratorTest, ChangeValue)
{
    vector<int> container({1, 2, 3});
    ConstIterator<vector<int>> iterator(container);
    // iterator++ = 2; Fails compilation
    EXPECT_EQ(1, container[0]);
    EXPECT_EQ(2, container[1]);
    EXPECT_EQ(3, container[2]);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
