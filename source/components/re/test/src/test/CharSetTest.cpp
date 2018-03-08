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
    CharSet<char> charSet;
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, charSet.Count());
    EXPECT_EQ('\xFF', charSet.First());
}

TEST_FIXTURE(CharSetTest, ConstructSingleChar)
{
    char singleChar = 'a';
    CharSet<char> charSet(singleChar);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, ConstructRange)
{
    CharSet<char>::CharRange range('a', 'c');
    CharSet<char> charSet(range);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, ConstructCopy)
{
    CharSet<char>::CharRange range('a', 'c');
    CharSet<char> other(range);
    CharSet<char> charSet(other);
    for (char ch = 0; ch < other.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(other.Contains(ch));
        else
            EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, other.Count());
    EXPECT_EQ('a', other.First());
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, ConstructMove)
{
    CharSet<char>::CharRange range('a', 'c');
    CharSet<char> other(range);
    CharSet<char> charSet(std::move(other));
    for (char ch = 0; ch < other.CharMax; ++ch)
    {
        EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, other.Count());
    EXPECT_EQ('\xFF', other.First());
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, Assign)
{
    CharSet<char>::CharRange range('a', 'c');
    CharSet<char> other(range);
    CharSet<char> charSet;
    charSet = other;
    for (char ch = 0; ch < other.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(other.Contains(ch));
        else
            EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, other.Count());
    EXPECT_EQ('a', other.First());
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, AssignMove)
{
    CharSet<char>::CharRange range('a', 'c');
    CharSet<char> other(range);
    CharSet<char> charSet;
    charSet = std::move(other);
    for (char ch = 0; ch < other.CharMax; ++ch)
    {
        EXPECT_FALSE(other.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, other.Count());
    EXPECT_EQ('\xFF', other.First());
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, AddSingleChar)
{
    char singleChar = 'a';
    char secondChar = 'b';
    char otherChar = 'd';
    CharSet<char> charSet;
    charSet.Add(singleChar);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
    EXPECT_EQ('a', charSet.First());

    charSet.Add(secondChar);
    charSet.Add(otherChar);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((ch == singleChar) || (ch == secondChar) || (ch == otherChar))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
}

TEST_FIXTURE(CharSetTest, AddRange)
{
    CharSet<char>::CharRange firstRange('g', 'i');
    CharSet<char>::CharRange nonOverlappingBefore('d', 'e');
    CharSet<char>::CharRange nonOverlappingAfter('k', 'l');
    CharSet<char>::CharRange connectingBefore('d', 'f');
    CharSet<char>::CharRange connectingAfter('j', 'l');
    CharSet<char>::CharRange overlappingSub('g', 'h');
    CharSet<char>::CharRange overlappingExtending('a', 'z');
    CharSet<char>::CharRange otherRange('A', 'Z');
    CharSet<char> charSet;
    charSet.Add(firstRange);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if ((firstRange.from <= ch) && (firstRange.to >= ch))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, charSet.Count());
    EXPECT_EQ('g', charSet.First());

    charSet.Add(nonOverlappingBefore);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((nonOverlappingBefore.from <= ch) && (nonOverlappingBefore.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{5}, charSet.Count());
    EXPECT_EQ('d', charSet.First());

    charSet.Add(nonOverlappingAfter);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((nonOverlappingBefore.from <= ch) && (nonOverlappingBefore.to >= ch)) ||
            ((nonOverlappingAfter.from <= ch) && (nonOverlappingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{7}, charSet.Count());
    EXPECT_EQ('d', charSet.First());

    charSet.Add(connectingBefore);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((nonOverlappingAfter.from <= ch) && (nonOverlappingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{8}, charSet.Count());
    EXPECT_EQ('d', charSet.First());

    charSet.Add(connectingAfter);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((connectingAfter.from <= ch) && (connectingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{9}, charSet.Count());
    EXPECT_EQ('d', charSet.First());

    charSet.Add(overlappingSub);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((firstRange.from <= ch) && (firstRange.to >= ch)) ||
            ((connectingBefore.from <= ch) && (connectingBefore.to >= ch)) ||
            ((connectingAfter.from <= ch) && (connectingAfter.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{9}, charSet.Count());
    EXPECT_EQ('d', charSet.First());

    charSet.Add(overlappingExtending);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((overlappingExtending.from <= ch) && (overlappingExtending.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{26}, charSet.Count());
    EXPECT_EQ('a', charSet.First());

    charSet.Add(otherRange);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (((overlappingExtending.from <= ch) && (overlappingExtending.to >= ch)) ||
            ((otherRange.from <= ch) && (otherRange.to >= ch)))
            EXPECT_TRUE(charSet.Contains(ch));
            else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{52}, charSet.Count());
    EXPECT_EQ('A', charSet.First());
}

TEST_FIXTURE(CharSetTest, Clear)
{
    char singleChar = 'a';
    CharSet<char> charSet(singleChar);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
    charSet.Clear();
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, charSet.Count());
    EXPECT_EQ('\xFF', charSet.First());
}

TEST_FIXTURE(CharSetTest, Fill)
{
    char singleChar = 'a';
    CharSet<char> charSet(singleChar);
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(charSet.Contains(ch));
        else
            EXPECT_FALSE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, charSet.Count());
    EXPECT_EQ('a', charSet.First());
    charSet.Fill();
    for (char ch = 0; ch < charSet.CharMax; ++ch)
    {
        EXPECT_TRUE(charSet.Contains(ch));
    }
    EXPECT_EQ(size_t{128}, charSet.Count());
    EXPECT_EQ('\0', charSet.First());
}

TEST_FIXTURE(CharSetTest, Iterator)
{
    char firstChar = 'a';
    char lastChar = 'c';
    char firstCharSecondRange = 'd';
    char lastCharSecondRange = 'f';
    CharSet<char> charSet(firstChar);
    auto it = charSet.begin();
    EXPECT_FALSE(it == charSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(firstChar, it->to);
    ++it;
    EXPECT_TRUE(it == charSet.end());

    charSet.Clear();
    it = charSet.begin();
    EXPECT_TRUE(it == charSet.end());

    charSet.Add(CharSet<char>::CharRange(firstChar, lastChar));
    it = charSet.begin();
    EXPECT_FALSE(it == charSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(lastChar, it->to);
    ++it;
    EXPECT_TRUE(it == charSet.end());

    charSet.Add(CharSet<char>::CharRange(firstCharSecondRange, lastCharSecondRange));
    it = charSet.begin();
    EXPECT_FALSE(it == charSet.end());
    EXPECT_EQ(firstChar, it->from);
    EXPECT_EQ(lastCharSecondRange, it->to);
    ++it;
    EXPECT_TRUE(it == charSet.end());
}

TEST_FIXTURE(CharSetTest, Includes)
{
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> target(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> target(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> target;
    CharSet<char> empty;
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> target;
    CharSet<char> empty;
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));

    CharSet<char> expected;
    EXPECT_EQ(expected, target);
    target = firstRange | firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange | nonOverlappingBefore;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange | nonOverlappingAfter;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingBefore;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingAfter;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingSub;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingExtending;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange | otherRange;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(CharSetTest, OperatorMinus)
{
    CharSet<char> target;
    CharSet<char> empty;
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> expected('i');
    target = firstRange - overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange - overlappingExtending;
    EXPECT_EQ(empty, target);
    target = firstRange - otherRange;
    EXPECT_EQ(firstRange, target);
}

TEST_FIXTURE(CharSetTest, OperatorAndEquals)
{
    CharSet<char> target;
    CharSet<char> empty;
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> target;
    CharSet<char> empty;
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));

    CharSet<char> expected;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target |= nonOverlappingBefore;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= nonOverlappingAfter;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingBefore;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingAfter;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingSub;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingExtending;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= otherRange;
    expected = CharSet<char>(firstRange);
    expected.Add(CharSet<char>::CharRange('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(CharSetTest, OperatorMinusEquals)
{
    CharSet<char> target;
    CharSet<char> empty;
    CharSet<char> firstRange(CharSet<char>::CharRange('g', 'i'));
    CharSet<char> nonOverlappingBefore(CharSet<char>::CharRange('d', 'e'));
    CharSet<char> nonOverlappingAfter(CharSet<char>::CharRange('k', 'l'));
    CharSet<char> connectingBefore(CharSet<char>::CharRange('d', 'f'));
    CharSet<char> connectingAfter(CharSet<char>::CharRange('j', 'l'));
    CharSet<char> overlappingSub(CharSet<char>::CharRange('g', 'h'));
    CharSet<char> overlappingExtending(CharSet<char>::CharRange('a', 'z'));
    CharSet<char> otherRange(CharSet<char>::CharRange('A', 'Z'));
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
    CharSet<char> expected('i');
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
