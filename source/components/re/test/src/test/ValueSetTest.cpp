#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "re/ValueSet.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class ValueSetTest : public UnitTestCpp::TestFixture
{
};

using CharValueSet = ValueSet<char, int>;

TEST_SUITE(re) {

TEST_FIXTURE(ValueSetTest, Empty)
{
    CharValueSet target;
    CharValueSet expected = CharValueSet::Empty;

    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(ValueSetTest, Full)
{
    CharValueSet target;
    target.Fill();
    CharValueSet expected = CharValueSet::All;

    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(ValueSetTest, ConstructDefault)
{
    CharValueSet inputSet;
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{0}, inputSet.Count());
}

TEST_FIXTURE(ValueSetTest, ConstructSingleChar)
{
    char singleChar = 'a';
    CharValueSet inputSet(singleChar);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if (ch == singleChar)
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{1}, inputSet.Count());
}

TEST_FIXTURE(ValueSetTest, ConstructRange)
{
    CharValueSet::Range range('a', 'c');
    CharValueSet inputSet(range);
    for (char ch = 0; ch < inputSet.MaxValue; ++ch)
    {
        if ((range.from <= ch) && (range.to >= ch))
            EXPECT_TRUE(inputSet.Contains(ch));
        else
            EXPECT_FALSE(inputSet.Contains(ch));
    }
    EXPECT_EQ(size_t{3}, inputSet.Count());
}

TEST_FIXTURE(ValueSetTest, ConstructCopy)
{
    CharValueSet::Range range('a', 'c');
    CharValueSet other(range);
    CharValueSet inputSet(other);
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

TEST_FIXTURE(ValueSetTest, ConstructMove)
{
    CharValueSet::Range range('a', 'c');
    CharValueSet other(range);
    CharValueSet inputSet(std::move(other));
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

TEST_FIXTURE(ValueSetTest, Assign)
{
    CharValueSet::Range range('a', 'c');
    CharValueSet other(range);
    CharValueSet inputSet;
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

TEST_FIXTURE(ValueSetTest, AssignMove)
{
    CharValueSet::Range range('a', 'c');
    CharValueSet other(range);
    CharValueSet inputSet;
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

TEST_FIXTURE(ValueSetTest, AddSingleChar)
{
    char singleChar = 'a';
    char secondChar = 'b';
    char otherChar = 'd';
    CharValueSet inputSet;
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

TEST_FIXTURE(ValueSetTest, AddRange)
{
    CharValueSet::Range firstRange('g', 'i');
    CharValueSet::Range nonOverlappingBefore('d', 'e');
    CharValueSet::Range nonOverlappingAfter('k', 'l');
    CharValueSet::Range connectingBefore('d', 'f');
    CharValueSet::Range connectingAfter('j', 'l');
    CharValueSet::Range overlappingSub('g', 'h');
    CharValueSet::Range overlappingExtending('a', 'z');
    CharValueSet::Range otherRange('A', 'Z');
    CharValueSet inputSet;
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

TEST_FIXTURE(ValueSetTest, Clear)
{
    char singleChar = 'a';
    CharValueSet inputSet(singleChar);
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

TEST_FIXTURE(ValueSetTest, Fill)
{
    char singleChar = 'a';
    CharValueSet inputSet(singleChar);
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

TEST_FIXTURE(ValueSetTest, Includes)
{
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    EXPECT_FALSE(firstRange.Includes(nonOverlappingBefore));
    EXPECT_FALSE(firstRange.Includes(nonOverlappingAfter));
    EXPECT_FALSE(firstRange.Includes(connectingBefore));
    EXPECT_FALSE(firstRange.Includes(connectingAfter));
    EXPECT_TRUE(firstRange.Includes(overlappingSub));
    EXPECT_FALSE(firstRange.Includes(overlappingExtending));
    EXPECT_FALSE(firstRange.Includes(otherRange));
}

TEST_FIXTURE(ValueSetTest, Overlaps)
{
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    EXPECT_FALSE(firstRange.Overlaps(nonOverlappingBefore));
    EXPECT_FALSE(firstRange.Overlaps(nonOverlappingAfter));
    EXPECT_FALSE(firstRange.Overlaps(connectingBefore));
    EXPECT_FALSE(firstRange.Overlaps(connectingAfter));
    EXPECT_TRUE(firstRange.Overlaps(overlappingSub));
    EXPECT_TRUE(firstRange.Overlaps(overlappingExtending));
    EXPECT_FALSE(firstRange.Overlaps(otherRange));
}

TEST_FIXTURE(ValueSetTest, OperatorEquals)
{
    CharValueSet target(CharValueSet::Range('g', 'i'));
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    EXPECT_TRUE(target == firstRange);
    EXPECT_FALSE(target == nonOverlappingBefore);
    EXPECT_FALSE(target == nonOverlappingAfter);
    EXPECT_FALSE(target == connectingBefore);
    EXPECT_FALSE(target == connectingAfter);
    EXPECT_FALSE(target == overlappingSub);
    EXPECT_FALSE(target == overlappingExtending);
    EXPECT_FALSE(target == otherRange);
}

TEST_FIXTURE(ValueSetTest, OperatorNotEquals)
{
    CharValueSet target(CharValueSet::Range('g', 'i'));
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    EXPECT_FALSE(target != firstRange);
    EXPECT_TRUE(target != nonOverlappingBefore);
    EXPECT_TRUE(target != nonOverlappingAfter);
    EXPECT_TRUE(target != connectingBefore);
    EXPECT_TRUE(target != connectingAfter);
    EXPECT_TRUE(target != overlappingSub);
    EXPECT_TRUE(target != overlappingExtending);
    EXPECT_TRUE(target != otherRange);
}

TEST_FIXTURE(ValueSetTest, OperatorAnd)
{
    CharValueSet target;
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    target = firstRange & firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange & nonOverlappingBefore;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange & nonOverlappingAfter;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange & connectingBefore;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange & connectingAfter;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange & overlappingSub;
    EXPECT_EQ(overlappingSub, target);
    target = firstRange & overlappingExtending;
    EXPECT_EQ(firstRange, target);
    target = firstRange & otherRange;
    EXPECT_EQ(CharValueSet::Empty, target);
}

TEST_FIXTURE(ValueSetTest, OperatorOr)
{
    CharValueSet target;
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));

    CharValueSet expected;
    EXPECT_EQ(expected, target);
    target = firstRange | firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange | nonOverlappingBefore;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange | nonOverlappingAfter;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingBefore;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange | connectingAfter;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingSub;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange | overlappingExtending;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange | otherRange;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(ValueSetTest, OperatorMinus)
{
    CharValueSet target;
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    target = firstRange - firstRange;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange - nonOverlappingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange - nonOverlappingAfter;
    EXPECT_EQ(firstRange, target);
    target = firstRange - connectingBefore;
    EXPECT_EQ(firstRange, target);
    target = firstRange - connectingAfter;
    EXPECT_EQ(firstRange, target);
    CharValueSet expected('i');
    target = firstRange - overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange - overlappingExtending;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange - otherRange;
    EXPECT_EQ(firstRange, target);
}

TEST_FIXTURE(ValueSetTest, OperatorNot)
{
    CharValueSet other(CharValueSet::Range('a', 'z'));
    CharValueSet target = ~other;
    CharValueSet expected = CharValueSet::All - other;

    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(ValueSetTest, OperatorAndEquals)
{
    CharValueSet target;
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    target = firstRange;
    target &= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target &= nonOverlappingBefore;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange;
    target &= nonOverlappingAfter;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange;
    target &= connectingBefore;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange;
    target &= connectingAfter;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange;
    target &= overlappingSub;
    EXPECT_EQ(overlappingSub, target);
    target = firstRange;
    target &= overlappingExtending;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target &= otherRange;
    EXPECT_EQ(CharValueSet::Empty, target);
}

TEST_FIXTURE(ValueSetTest, OperatorOrEquals)
{
    CharValueSet target;
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));

    CharValueSet expected;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= firstRange;
    EXPECT_EQ(firstRange, target);
    target = firstRange;
    target |= nonOverlappingBefore;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('d', 'e'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= nonOverlappingAfter;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('k', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingBefore;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('d', 'f'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= connectingAfter;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('j', 'l'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingSub;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('g', 'h'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= overlappingExtending;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('a', 'z'));
    EXPECT_EQ(expected, target);
    target = firstRange;
    target |= otherRange;
    expected = CharValueSet(firstRange);
    expected.Add(CharValueSet::Range('A', 'Z'));
    EXPECT_EQ(expected, target);
}

TEST_FIXTURE(ValueSetTest, OperatorMinusEquals)
{
    CharValueSet target;
    CharValueSet firstRange(CharValueSet::Range('g', 'i'));
    CharValueSet nonOverlappingBefore(CharValueSet::Range('d', 'e'));
    CharValueSet nonOverlappingAfter(CharValueSet::Range('k', 'l'));
    CharValueSet connectingBefore(CharValueSet::Range('d', 'f'));
    CharValueSet connectingAfter(CharValueSet::Range('j', 'l'));
    CharValueSet overlappingSub(CharValueSet::Range('g', 'h'));
    CharValueSet overlappingExtending(CharValueSet::Range('a', 'z'));
    CharValueSet otherRange(CharValueSet::Range('A', 'Z'));
    target = firstRange;
    target -= firstRange;
    EXPECT_EQ(CharValueSet::Empty, target);
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
    CharValueSet expected('i');
    target = firstRange;
    target -= overlappingSub;
    EXPECT_EQ(expected, target);
    target = firstRange;
    target -= overlappingExtending;
    EXPECT_EQ(CharValueSet::Empty, target);
    target = firstRange;
    target -= otherRange;
    EXPECT_EQ(firstRange, target);
}

TEST_FIXTURE(ValueSetTest, PrintSingleChar)
{
    ostringstream stream;

    CharValueSet inputSet('a');
    stream << inputSet;
    string expected = "a";
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(ValueSetTest, PrintRange)
{
    ostringstream stream;

    CharValueSet inputSet(CharValueSet::Range('g', 'i'));
    stream << inputSet;
    string expected = "g-i";
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(ValueSetTest, Print)
{
    ostringstream stream;

    CharValueSet inputSet(CharValueSet::Range('g', 'i'));
    inputSet |= CharValueSet::Range('k', 'l');
    stream << inputSet;
    string expected = "g-i,k-l";
    EXPECT_EQ(expected, stream.str());
}

} // TEST_SUITE(re)

} // namespace Test
} // namespace RE
