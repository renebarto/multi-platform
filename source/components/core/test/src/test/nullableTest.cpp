#include <unittest-cpp/UnitTestC++.h>

#include "core/nullable.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class nullableTest : public UnitTestCpp::TestFixture
{
};

class Value
{
public:
    Value() : _value() {}
    // cppcheck-suppress noExplicitConstructor
    Value(int value) : _value(value) {}
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

TEST_FIXTURE(nullableTest, Construction)
{
    nullable<Value> target;

    EXPECT_FALSE(target.HasValue());
}

TEST_FIXTURE(nullableTest, ConstructionValue)
{
    Value value = 1234;
    nullable<Value> target(value);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());
}

TEST_FIXTURE(nullableTest, ConstructionCopy)
{
    Value value = 1234;
    nullable<Value> other(value);
    nullable<Value> target(other);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_TRUE(other.HasValue());
    EXPECT_EQ(value, other.Value());
}

TEST_FIXTURE(nullableTest, ConstructionMove)
{
    Value value = 1234;
    nullable<Value> other(value);
    nullable<Value> target(std::move(other));

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_FALSE(other.HasValue());
}

TEST_FIXTURE(nullableTest, AssignmentValue)
{
    Value value = 1234;
    nullable<Value> target;
    target = value;

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());
}

TEST_FIXTURE(nullableTest, AssignmentCopy)
{
    Value value = 1234;
    nullable<Value> other(value);
    nullable<Value> target;
    target = other;

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_TRUE(other.HasValue());
    EXPECT_EQ(value, other.Value());
}

TEST_FIXTURE(nullableTest, AssignmentMove)
{
    Value value = 1234;
    nullable<Value> other(value);
    nullable<Value> target;
    target = std::move(other);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    EXPECT_FALSE(other.HasValue());
}

TEST_FIXTURE(nullableTest, SetNull)
{
    Value value = 1234;
    nullable<Value> target(value);

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());

    target.SetNull();

    EXPECT_FALSE(target.HasValue());
}

TEST_FIXTURE(nullableTest, ValueRefConst)
{
    Value value = 1234;
    const nullable<Value> target(value);
    const Value &constRef = value;

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());
    EXPECT_EQ(constRef, target.Value());
}

TEST_FIXTURE(nullableTest, ValueRef)
{
    Value value = 1234;
    nullable<Value> target(value);
    Value & ref = target.Value();

    EXPECT_TRUE(target.HasValue());
    EXPECT_EQ(value, target.Value());
    EXPECT_EQ(value, ref);
    ref = 123;
    EXPECT_EQ(123, ref);
    EXPECT_EQ(123, target.Value());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
