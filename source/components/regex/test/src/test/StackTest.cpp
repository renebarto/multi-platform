#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "regex/Stack.h"
#include "regex/TestData.h"

using namespace std;

namespace Regex {
namespace Test {

class StackTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(regex) {

TEST_FIXTURE(StackTest, ConstructDefault)
{
    Stack<int> stack;
    EXPECT_EQ(size_t{0}, stack.Count());
    const int Value {};
    EXPECT_FALSE(stack.AtTop(Value));
    EXPECT_EQ(ssize_t{-1}, stack.Find(Value));
}

TEST_FIXTURE(StackTest, Push)
{
    Stack<int> stack;
    EXPECT_EQ(size_t{0}, stack.Count());
    const int Value {1};
    stack.Push(Value);
    EXPECT_EQ(size_t{1}, stack.Count());
    EXPECT_TRUE(stack.AtTop(Value));
    EXPECT_EQ(Value, stack.At(0));
    EXPECT_EQ(ssize_t{0}, stack.Find(Value));
}

TEST_FIXTURE(StackTest, PopTop)
{
    Stack<int> stack;
    EXPECT_EQ(size_t{0}, stack.Count());
    const int Value {1};
    stack.Push(Value);
    EXPECT_EQ(Value, stack.Top());
    EXPECT_EQ(size_t{1}, stack.Count());
    EXPECT_EQ(Value, stack.Pop());
    EXPECT_EQ(size_t{0}, stack.Count());
}

TEST_FIXTURE(StackTest, RemoveTopElements)
{
    Stack<int> stack;
    EXPECT_EQ(size_t{0}, stack.Count());
    const int Value1 {1};
    const int Value2 {2};
    stack.Push(Value1);
    stack.Push(Value2);
    EXPECT_EQ(Value2, stack.At(0));
    EXPECT_EQ(Value1, stack.At(1));
    EXPECT_EQ(ssize_t{0}, stack.Find(Value2));
    EXPECT_EQ(ssize_t{1}, stack.Find(Value1));
    stack.RemoveTopElements(1);
    EXPECT_EQ(size_t{1}, stack.Count());
    EXPECT_EQ(Value1, stack.At(0));
    EXPECT_EQ(ssize_t{0}, stack.Find(Value1));
    EXPECT_EQ(Value1, stack.Pop());
    EXPECT_EQ(size_t{0}, stack.Count());
}

} // TEST_SUITE(regex)

} // namespace Test
} // namespace Regex
