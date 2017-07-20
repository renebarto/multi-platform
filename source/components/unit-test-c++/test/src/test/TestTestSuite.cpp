#include <unit-test-c++/UnitTestC++.h>

TEST_SUITE(unit_test_cpp)
{

TEST_SUITE(MyTestSuite)
{

TEST(SuiteNameIsCorrect)
{
    ASSERT_EQ("MyTestSuite", GetSuiteName());
}

}

} // TEST_SUITE(unit_test_cpp)

TEST(DefaultSuiteNameIsCorrect)
{
    ASSERT_EQ("", GetSuiteName());
}

