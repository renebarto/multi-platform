#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/CharSet.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class CharSetTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(re) {

TEST_FIXTURE(CharSetTest, ConstructDefault)
{
    CharSet charSet;
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, ConstructSingleChar)
{
    char singleChar = 'a';
    CharSet charSet(singleChar);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, ConstructRange)
{
    CharSet::Range range('a', 'c');
    CharSet charSet(range);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, ConstructCopy)
{
    CharSet::Range range('a', 'c');
    CharSet other(range);
    CharSet charSet(other);
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(other.Contains(ch));
        else
            EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, other.Count());
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, ConstructMove)
{
    CharSet::Range range('a', 'c');
    CharSet other(range);
    CharSet charSet(std::move(other));
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, other.Count());
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, Assign)
{
    CharSet::Range range('a', 'c');
    CharSet other(range);
    CharSet charSet;
    charSet = other;
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(other.Contains(ch));
        else
            EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, other.Count());
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, AssignMove)
{
    CharSet::Range range('a', 'c');
    CharSet other(range);
    CharSet charSet;
    charSet = std::move(other);
    for (char ch = 0; ch < other.MaxValue; ++ch)
    {
        EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, other.Count());
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, AddSingleChar)
{
    char singleChar = 'a';
    char secondChar = 'b';
    char otherChar = 'd';
    CharSet charSet;
    charSet.Add(singleChar);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());

    charSet.Add(secondChar);
    charSet.Add(otherChar);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((ch == singleChar) || (ch == secondChar) || (ch == otherChar))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, AddRange)
{
    CharSet::Range firstRange('g', 'i');
    CharSet::Range nonOverlappingBefore('d', 'e');
    CharSet::Range nonOverlappingAfter('k', 'l');
    CharSet::Range connectingBefore('d', 'f');
    CharSet::Range connectingAfter('j', 'l');
    CharSet::Range overlappingSub('g', 'h');
    CharSet::Range overlappingExtending('a', 'z');
    CharSet::Range otherRange('A', 'Z');
    CharSet charSet;
    charSet.Add(firstRange);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if ((firstRange.from <= ch) && (firstRange.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());

    charSet.Add(nonOverlappingBefore);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((nonOverlappingBefore.from <= ch) && (nonOverlappingBefore.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{5}, charSet.Count());

    charSet.Add(nonOverlappingAfter);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((nonOverlappingBefore.from <= ch) && (nonOverlappingBefore.to >= ch)) ||
            ((nonOverlappingAfter.from <= ch) && (nonOverlappingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{7}, charSet.Count());

    charSet.Add(connectingBefore);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((nonOverlappingAfter.from <= ch) && (nonOverlappingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{8}, charSet.Count());

    charSet.Add(connectingAfter);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((connectingAfter.from <= ch) && (connectingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{9}, charSet.Count());

    charSet.Add(overlappingSub);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((connectingAfter.from <= ch) && (connectingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{9}, charSet.Count());

    charSet.Add(overlappingExtending);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((overlappingExtending.from <= ch) && (overlappingExtending.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{26}, charSet.Count());

    charSet.Add(otherRange);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (((overlappingExtending.from <= ch) && (overlappingExtending.to >= ch)) ||
            ((otherRange.from <= ch) && (otherRange.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
            else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{52}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, Clear)
{
    char singleChar = 'a';
    CharSet charSet(singleChar);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
    charSet.Clear();
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, Fill)
{
    char singleChar = 'a';
    CharSet charSet(singleChar);
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
    charSet.Fill();
    for (char ch = 0; ch < charSet.MaxValue; ++ch)
    {
        EXPECT_TRUE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{128}, charSet.Count());
}

TEST_FIXTURE(CharSetTest, Iterator)
{
    char firstChar = 'a';
    char lastChar = 'c';
    char firstCharSecondRange = 'd';
    char lastCharSecondRange = 'f';
    CharSet charSet(firstChar);
    auto it = charSet.begin();
    EXPECT_FALSE(it == charSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(firstChar, it->to);
    ++it;
    EXPECT_TRUE(it == charSet.end());

    charSet.Clear();
    it = charSet.begin();
    EXPECT_TRUE(it == charSet.end());

    charSet.Add(CharSet::Range(firstChar, lastChar));
    it = charSet.begin();
    EXPECT_FALSE(it == charSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(lastChar, it->to);
    ++it;
    EXPECT_TRUE(it == charSet.end());

    charSet.Add(CharSet::Range(firstCharSecondRange, lastCharSecondRange));
    it = charSet.begin();
    EXPECT_FALSE(it == charSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(lastCharSecondRange, it->to);
    ++it;
    EXPECT_TRUE(it == charSet.end());
}

TEST_FIXTURE(CharSetTest, Includes)
{
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    EXPECT_FALSE(firstRange.Includes(nonOverlappingBefore));
    EXPECT_FALSE(firstRange.Includes(nonOverlappingAfter));
    EXPECT_FALSE(firstRange.Includes(connectingBefore));
    EXPECT_FALSE(firstRange.Includes(connectingAfter));
    EXPECT_TRUE(firstRange.Includes(overlappingSub));
    EXPECT_FALSE(firstRange.Includes(overlappingExtending));
    EXPECT_FALSE(firstRange.Includes(otherRange));
}

TEST_FIXTURE(CharSetTest, Overlaps)
{
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    EXPECT_FALSE(firstRange.Overlaps(nonOverlappingBefore));
    EXPECT_FALSE(firstRange.Overlaps(nonOverlappingAfter));
    EXPECT_FALSE(firstRange.Overlaps(connectingBefore));
    EXPECT_FALSE(firstRange.Overlaps(connectingAfter));
    EXPECT_TRUE(firstRange.Overlaps(overlappingSub));
    EXPECT_TRUE(firstRange.Overlaps(overlappingExtending));
    EXPECT_FALSE(firstRange.Overlaps(otherRange));
}

TEST_FIXTURE(CharSetTest, OperatorEquals)
{
    CharSet target(CharSet::Range('g', 'i'));
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    EXPECT_TRUE(target == firstRange);
    EXPECT_FALSE(target == nonOverlappingBefore);
    EXPECT_FALSE(target == nonOverlappingAfter);
    EXPECT_FALSE(target == connectingBefore);
    EXPECT_FALSE(target == connectingAfter);
    EXPECT_FALSE(target == overlappingSub);
    EXPECT_FALSE(target == overlappingExtending);
    EXPECT_FALSE(target == otherRange);
}

TEST_FIXTURE(CharSetTest, OperatorNotEquals)
{
    CharSet target(CharSet::Range('g', 'i'));
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    EXPECT_FALSE(target != firstRange);
    EXPECT_TRUE(target != nonOverlappingBefore);
    EXPECT_TRUE(target != nonOverlappingAfter);
    EXPECT_TRUE(target != connectingBefore);
    EXPECT_TRUE(target != connectingAfter);
    EXPECT_TRUE(target != overlappingSub);
    EXPECT_TRUE(target != overlappingExtending);
    EXPECT_TRUE(target != otherRange);
}

TEST_FIXTURE(CharSetTest, OperatorAnd)
{
    CharSet target;
    CharSet empty;
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    target = firstRange & firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange & nonOverlappingBefore;
    EXPECT_EQ(empty, target);
    target = firstRange & nonOverlappingAfter;
    EXPECT_EQ(empty, target);
    target = firstRange & connectingBefore;
    EXPECT_EQ(empty, target);
    target = firstRange & connectingAfter;
    EXPECT_EQ(empty, target);
    target = firstRange & overlappingSub;
    EXPECT_EQ(overlappingSub, target);
    target = firstRange & overlappingExtending;
    EXPECT_EQ(firstRange, target);
    target = firstRange & otherRange;
    EXPECT_EQ(empty, target);
}

TEST_FIXTURE(CharSetTest, OperatorOr)
{
    CharSet target;
    CharSet empty;
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));

    CharSet expected;
    EXPECT_EQ(expected, target);
    target = firstRange | firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange | nonOverlappingBefore;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange | nonOverlappingAfter;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingBefore;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingAfter;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingSub;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingExtending;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange | otherRange;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(CharSetTest, OperatorMinus)
{
    CharSet target;
    CharSet empty;
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    target = firstRange - firstRange;
    EXPECT_EQ(empty, target);
    target = firstRange - nonOverlappingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange - nonOverlappingAfter;
    EXPECT_EQ(firstRange, target);
    target = firstRange - connectingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange - connectingAfter;
    EXPECT_EQ(firstRange, target);
    CharSet expected('i');
    target = firstRange - overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange - overlappingExtending;
    EXPECT_EQ(empty, target);
    target = firstRange - otherRange;
    EXPECT_EQ(firstRange, target);
}

TEST_FIXTURE(CharSetTest, OperatorAndEquals)
{
    CharSet target;
    CharSet empty;
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    target = firstRange;
    target &= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target &= nonOverlappingBefore;
    EXPECT_EQ(empty, target);
    target = firstRange;
    target &= nonOverlappingAfter;
    EXPECT_EQ(empty, target);
    target = firstRange;
    target &= connectingBefore;
    EXPECT_EQ(empty, target);
    target = firstRange;
    target &= connectingAfter;
    EXPECT_EQ(empty, target);
    target = firstRange;
    target &= overlappingSub;
    EXPECT_EQ(overlappingSub, target);
    target = firstRange;
    target &= overlappingExtending;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target &= otherRange;
    EXPECT_EQ(empty, target);
}

TEST_FIXTURE(CharSetTest, OperatorOrEquals)
{
    CharSet target;
    CharSet empty;
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));

    CharSet expected;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target |= nonOverlappingBefore;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= nonOverlappingAfter;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingBefore;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingAfter;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingSub;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingExtending;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= otherRange;
    expected = CharSet(firstRange);
    expected.Add(CharSet::Range('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(CharSetTest, OperatorMinusEquals)
{
    CharSet target;
    CharSet empty;
    CharSet firstRange(CharSet::Range('g', 'i'));
    CharSet nonOverlappingBefore(CharSet::Range('d', 'e'));
    CharSet nonOverlappingAfter(CharSet::Range('k', 'l'));
    CharSet connectingBefore(CharSet::Range('d', 'f'));
    CharSet connectingAfter(CharSet::Range('j', 'l'));
    CharSet overlappingSub(CharSet::Range('g', 'h'));
    CharSet overlappingExtending(CharSet::Range('a', 'z'));
    CharSet otherRange(CharSet::Range('A', 'Z'));
    target = firstRange;
    target -= firstRange;
    EXPECT_EQ(empty, target);
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
    CharSet expected('i');
    target = firstRange;
    target -= overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target -= overlappingExtending;
    EXPECT_EQ(empty, target);
    target = firstRange;
    target -= otherRange;
    EXPECT_EQ(firstRange, target);
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
