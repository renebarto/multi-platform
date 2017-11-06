#include <unit-test-c++/UnitTestC++.h>

#include "core/Nullable.h"
#include "core/Util.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class NullableTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

};

void NullableTest::SetUp()
{
}

void NullableTest::TearDown()
{
}

class Value
{
public:
    Value() : _value() {}
    explicit Value(int value) : _value(value) {}
    Value(const Value & other) : _value(other._value) {}
    Value(Value && other) : _value(other._value) { other._value = 0; }
    Value & operator = (int value)
    {
        _value = value;
        return *this;
    }
    Value & operator = (const Value & other)
    {
        if (&other != this)
            _value = other._value;
        return *this;
    }
    Value & operator = (Value && other)
    {
        if (&other != this)
        {
            _value = other._value;
            other._value = 0;
        }
        return *this;
    }

    bool operator == (const Value & other) const { return _value == other._value; }
    bool operator != (const Value & other) const { return !operator == (other); }

    operator int () const { return _value; }

private:
    int _value;
};

inline void PrintTo(const Value & value, std::ostream & stream)
{
    stream << static_cast<int>(value);
}

TEST_FIXTURE(NullableTest, Construction)
{
    Nullable<Value> target;

    EXPECT_FALSE(target.HasValue());
}

TEST_FIXTURE(NullableTest, ConstructionValue)
{
    Value value = 1234;
    Nullable<Value> target(value);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());
    Value & valueRef = target.Value();
    valueRef = 123;
    EXPECT_EQ(123, valueRef);
}

TEST_FIXTURE(NullableTest, ConstructionCopy)
{
    Value value = 1234;
    Nullable<Value> other(value);
    Nullable<Value> target(other);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_TRUE(other.HasValue());
    EXPECT_EQ(value, other.Value());
}

TEST_FIXTURE(NullableTest, ConstructionMove)
{
    Value value = 1234;
    Nullable<Value> other(value);
    Nullable<Value> target(std::move(other));

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_FALSE(other.HasValue());
}

TEST_FIXTURE(NullableTest, AssignmentValue)
{
    Value value = 1234;
    Nullable<Value> target;
    target = value;

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());
}

TEST_FIXTURE(NullableTest, AssignmentCopy)
{
    Value value = 1234;
    Nullable<Value> other(value);
    Nullable<Value> target;
    target = other;

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_TRUE(other.HasValue());
    EXPECT_EQ(value, other.Value());
}

TEST_FIXTURE(NullableTest, AssignmentMove)
{
    Value value = 1234;
    Nullable<Value> other(value);
    Nullable<Value> target;
    target = std::move(other);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_FALSE(other.HasValue());
}

TEST_FIXTURE(NullableTest, SetNull)
{
    Value value = 1234;
    Nullable<Value> target(value);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    target.SetNull();

    EXPECT_FALSE(target.HasValue());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
