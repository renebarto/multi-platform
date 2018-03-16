#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/ValueSet.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class InputSetTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(re) {

TEST_FIXTURE(InputSetTest, Empty)
{
    ValueSet<char> target;
    ValueSet<char> expected = ValueSet<char>::Empty;

    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(InputSetTest, Full)
{
    ValueSet<char> target;
    target.Fill();
    ValueSet<char> expected = ValueSet<char>::All;

    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(InputSetTest, ConstructDefault)
{
    ValueSet<char> inputSet;
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, ConstructSingleChar)
{
    char singleChar = 'a';
    ValueSet<char> inputSet(singleChar);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, ConstructRange)
{
    ValueSet<char>::Range range('a', 'c');
    ValueSet<char> inputSet(range);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, ConstructCopy)
{
    ValueSet<char>::Range range('a', 'c');
    ValueSet<char> other(range);
    ValueSet<char> inputSet(other);
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(other.Contains(ch));
        else
            EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, other.Count());
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, ConstructMove)
{
    ValueSet<char>::Range range('a', 'c');
    ValueSet<char> other(range);
    ValueSet<char> inputSet(std::move(other));
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, other.Count());
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, Assign)
{
    ValueSet<char>::Range range('a', 'c');
    ValueSet<char> other(range);
    ValueSet<char> inputSet;
    inputSet = other;
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(other.Contains(ch));
        else
            EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, other.Count());
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, AssignMove)
{
    ValueSet<char>::Range range('a', 'c');
    ValueSet<char> other(range);
    ValueSet<char> inputSet;
    inputSet = std::move(other);
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, other.Count());
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, AddSingleChar)
{
    char singleChar = 'a';
    char secondChar = 'b';
    char otherChar = 'd';
    ValueSet<char> inputSet;
    inputSet.Add(singleChar);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, inputSet.Count());

    inputSet.Add(secondChar);
    inputSet.Add(otherChar);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((ch == singleChar) || (ch == secondChar) || (ch == otherChar))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, AddRange)
{
    ValueSet<char>::Range firstRange('g', 'i');
    ValueSet<char>::Range nonOverlappingBefore('d', 'e');
    ValueSet<char>::Range nonOverlappingAfter('k', 'l');
    ValueSet<char>::Range connectingBefore('d', 'f');
    ValueSet<char>::Range connectingAfter('j', 'l');
    ValueSet<char>::Range overlappingSub('g', 'h');
    ValueSet<char>::Range overlappingExtending('a', 'z');
    ValueSet<char>::Range otherRange('A', 'Z');
    ValueSet<char> inputSet;
    inputSet.Add(firstRange);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((firstRange.from <= ch) && (firstRange.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());

    inputSet.Add(nonOverlappingBefore);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((nonOverlappingBefore.from <= ch) && (nonOverlappingBefore.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{5}, inputSet.Count());

    inputSet.Add(nonOverlappingAfter);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((nonOverlappingBefore.from <= ch) && (nonOverlappingBefore.to >= ch)) ||
            ((nonOverlappingAfter.from <= ch) && (nonOverlappingAfter.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{7}, inputSet.Count());

    inputSet.Add(connectingBefore);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((nonOverlappingAfter.from <= ch) && (nonOverlappingAfter.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{8}, inputSet.Count());

    inputSet.Add(connectingAfter);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((connectingAfter.from <= ch) && (connectingAfter.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{9}, inputSet.Count());

    inputSet.Add(overlappingSub);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((connectingAfter.from <= ch) && (connectingAfter.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{9}, inputSet.Count());

    inputSet.Add(overlappingExtending);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((overlappingExtending.from <= ch) && (overlappingExtending.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{26}, inputSet.Count());

    inputSet.Add(otherRange);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (((overlappingExtending.from <= ch) && (overlappingExtending.to >= ch)) ||
            ((otherRange.from <= ch) && (otherRange.to >= ch)))
            EXPECT_TRUE(inputSet.Contains(ch));
            else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{52}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, Clear)
{
    char singleChar = 'a';
    ValueSet<char> inputSet(singleChar);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, inputSet.Count());
    inputSet.Clear();
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, Fill)
{
    char singleChar = 'a';
    ValueSet<char> inputSet(singleChar);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, inputSet.Count());
    inputSet.Fill();
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        EXPECT_TRUE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{128}, inputSet.Count());
}

TEST_FIXTURE(InputSetTest, Iterator)
{
    char firstChar = 'a';
    char lastChar = 'c';
    char firstCharSecondRange = 'd';
    char lastCharSecondRange = 'f';
    ValueSet<char> inputSet(firstChar);
    auto it = inputSet.begin();
    EXPECT_FALSE(it == inputSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(firstChar, it->to);
    ++it;
    EXPECT_TRUE(it == inputSet.end());

    inputSet.Clear();
    it = inputSet.begin();
    EXPECT_TRUE(it == inputSet.end());

    inputSet.Add(ValueSet<char>::Range(firstChar, lastChar));
    it = inputSet.begin();
    EXPECT_FALSE(it == inputSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(lastChar, it->to);
    ++it;
    EXPECT_TRUE(it == inputSet.end());

    inputSet.Add(ValueSet<char>::Range(firstCharSecondRange, lastCharSecondRange));
    it = inputSet.begin();
    EXPECT_FALSE(it == inputSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(lastCharSecondRange, it->to);
    ++it;
    EXPECT_TRUE(it == inputSet.end());
}

TEST_FIXTURE(InputSetTest, Includes)
{
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    EXPECT_FALSE(firstRange.Includes(nonOverlappingBefore));
    EXPECT_FALSE(firstRange.Includes(nonOverlappingAfter));
    EXPECT_FALSE(firstRange.Includes(connectingBefore));
    EXPECT_FALSE(firstRange.Includes(connectingAfter));
    EXPECT_TRUE(firstRange.Includes(overlappingSub));
    EXPECT_FALSE(firstRange.Includes(overlappingExtending));
    EXPECT_FALSE(firstRange.Includes(otherRange));
}

TEST_FIXTURE(InputSetTest, Overlaps)
{
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    EXPECT_FALSE(firstRange.Overlaps(nonOverlappingBefore));
    EXPECT_FALSE(firstRange.Overlaps(nonOverlappingAfter));
    EXPECT_FALSE(firstRange.Overlaps(connectingBefore));
    EXPECT_FALSE(firstRange.Overlaps(connectingAfter));
    EXPECT_TRUE(firstRange.Overlaps(overlappingSub));
    EXPECT_TRUE(firstRange.Overlaps(overlappingExtending));
    EXPECT_FALSE(firstRange.Overlaps(otherRange));
}

TEST_FIXTURE(InputSetTest, OperatorEquals)
{
    ValueSet<char> target(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    EXPECT_TRUE(target == firstRange);
    EXPECT_FALSE(target == nonOverlappingBefore);
    EXPECT_FALSE(target == nonOverlappingAfter);
    EXPECT_FALSE(target == connectingBefore);
    EXPECT_FALSE(target == connectingAfter);
    EXPECT_FALSE(target == overlappingSub);
    EXPECT_FALSE(target == overlappingExtending);
    EXPECT_FALSE(target == otherRange);
}

TEST_FIXTURE(InputSetTest, OperatorNotEquals)
{
    ValueSet<char> target(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    EXPECT_FALSE(target != firstRange);
    EXPECT_TRUE(target != nonOverlappingBefore);
    EXPECT_TRUE(target != nonOverlappingAfter);
    EXPECT_TRUE(target != connectingBefore);
    EXPECT_TRUE(target != connectingAfter);
    EXPECT_TRUE(target != overlappingSub);
    EXPECT_TRUE(target != overlappingExtending);
    EXPECT_TRUE(target != otherRange);
}

TEST_FIXTURE(InputSetTest, OperatorAnd)
{
    ValueSet<char> target;
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    target = firstRange & firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange & nonOverlappingBefore;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange & nonOverlappingAfter;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange & connectingBefore;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange & connectingAfter;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange & overlappingSub;
    EXPECT_EQ(overlappingSub, target);
    target = firstRange & overlappingExtending;
    EXPECT_EQ(firstRange, target);
    target = firstRange & otherRange;
    EXPECT_EQ(ValueSet<char>::Empty, target);
}

TEST_FIXTURE(InputSetTest, OperatorOr)
{
    ValueSet<char> target;
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));

    ValueSet<char> expected;
    EXPECT_EQ(expected, target);
    target = firstRange | firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange | nonOverlappingBefore;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange | nonOverlappingAfter;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingBefore;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingAfter;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingSub;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingExtending;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange | otherRange;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(InputSetTest, OperatorMinus)
{
    ValueSet<char> target;
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    target = firstRange - firstRange;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange - nonOverlappingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange - nonOverlappingAfter;
    EXPECT_EQ(firstRange, target);
    target = firstRange - connectingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange - connectingAfter;
    EXPECT_EQ(firstRange, target);
    ValueSet<char> expected('i');
    target = firstRange - overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange - overlappingExtending;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange - otherRange;
    EXPECT_EQ(firstRange, target);
}

TEST_FIXTURE(InputSetTest, OperatorNot)
{
    ValueSet<char> other(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> target = ~other;
    ValueSet<char> expected = ValueSet<char>::All - other;

    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(InputSetTest, OperatorAndEquals)
{
    ValueSet<char> target;
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    target = firstRange;
    target &= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target &= nonOverlappingBefore;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange;
    target &= nonOverlappingAfter;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange;
    target &= connectingBefore;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange;
    target &= connectingAfter;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange;
    target &= overlappingSub;
    EXPECT_EQ(overlappingSub, target);
    target = firstRange;
    target &= overlappingExtending;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target &= otherRange;
    EXPECT_EQ(ValueSet<char>::Empty, target);
}

TEST_FIXTURE(InputSetTest, OperatorOrEquals)
{
    ValueSet<char> target;
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));

    ValueSet<char> expected;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target |= nonOverlappingBefore;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= nonOverlappingAfter;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingBefore;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingAfter;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingSub;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingExtending;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= otherRange;
    expected = ValueSet<char>(firstRange);
    expected.Add(ValueSet<char>::Range('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(InputSetTest, OperatorMinusEquals)
{
    ValueSet<char> target;
    ValueSet<char> firstRange(ValueSet<char>::Range('g', 'i'));
    ValueSet<char> nonOverlappingBefore(ValueSet<char>::Range('d', 'e'));
    ValueSet<char> nonOverlappingAfter(ValueSet<char>::Range('k', 'l'));
    ValueSet<char> connectingBefore(ValueSet<char>::Range('d', 'f'));
    ValueSet<char> connectingAfter(ValueSet<char>::Range('j', 'l'));
    ValueSet<char> overlappingSub(ValueSet<char>::Range('g', 'h'));
    ValueSet<char> overlappingExtending(ValueSet<char>::Range('a', 'z'));
    ValueSet<char> otherRange(ValueSet<char>::Range('A', 'Z'));
    target = firstRange;
    target -= firstRange;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange;
    target -= nonOverlappingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target -= nonOverlappingAfter;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target -= connectingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target -= connectingAfter;
    EXPECT_EQ(firstRange, target);
    ValueSet<char> expected('i');
    target = firstRange;
    target -= overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target -= overlappingExtending;
    EXPECT_EQ(ValueSet<char>::Empty, target);
    target = firstRange;
    target -= otherRange;
    EXPECT_EQ(firstRange, target);
}

TEST_FIXTURE(InputSetTest, PrintSingleChar)
{
    ostringstream stream;

    ValueSet<char> inputSet('a');
    stream << inputSet;
    string expected = "a";
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(InputSetTest, PrintRange)
{
    ostringstream stream;

    ValueSet<char> inputSet(ValueSet<char>::Range('g', 'i'));
    stream << inputSet;
    string expected = "g-i";
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(InputSetTest, Print)
{
    ostringstream stream;

    ValueSet<char> inputSet(ValueSet<char>::Range('g', 'i'));
    inputSet |= ValueSet<char>::Range('k', 'l');
    stream << inputSet;
    string expected = "g-i,k-l";
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
