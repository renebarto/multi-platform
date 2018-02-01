#include <unittest-c++/UnitTestC++.h>

#include "osal/Assert.h"

namespace OSAL {
namespace Test {

static constexpr size_t TestSize = 16;

class AssertTest : public UnitTestCpp::TestFixture
{
public:
    static void AssertionHandler(bool expression, const char * expressionText, const char * file, int line, const char * func)
    {
        _expression = expression;
        _expressionText = expressionText;
        _file = file;
        _line = line;
        _func = func;
    }
    static bool _expression;
    static const char * _expressionText;
    static const char * _file;
    static int _line;
    static const char * _func;

    void SetUp() override
    {
        _expression = false;
        _expressionText = nullptr;
        _file = nullptr;
        _line = 0;
        _func = nullptr;
    }
};

bool AssertTest::_expression = false;
const char * AssertTest::_expressionText = nullptr;
const char * AssertTest::_file = nullptr;
int AssertTest::_line = 0;
const char * AssertTest::_func = nullptr;

TEST_SUITE(osal)
{

TEST_FIXTURE(AssertTest, AssertSucceeds)
{
    AssertHandler * oldHandler = SetAssertionHandler(AssertionHandler);
    EXPECT_FALSE(_expression);
    EXPECT_NULL(_expressionText);
    EXPECT_NULL(_file);
    EXPECT_EQ(0, _line);
    EXPECT_NULL(_func);
    bool b = true;
    ASSERT(b == true);
    EXPECT_FALSE(_expression);
    EXPECT_NULL(_expressionText);
    EXPECT_NULL(_file);
    EXPECT_EQ(0, _line);
    EXPECT_NULL(_func);
    SetAssertionHandler(oldHandler);
}

TEST_FIXTURE(AssertTest, AssertFails)
{
    AssertHandler * oldHandler = SetAssertionHandler(AssertionHandler);
    EXPECT_FALSE(_expression);
    EXPECT_NULL(_expressionText);
    EXPECT_NULL(_file);
    EXPECT_EQ(0, _line);
    EXPECT_NULL(_func);
    bool b = false;
    const char * file = __FILE__;
    const char * func = __func__;
    ASSERT(b == true); int line = __LINE__;
    EXPECT_FALSE(_expression);
    EXPECT_EQ(string("b == true"), _expressionText);
    EXPECT_EQ(string(file), _file);
    EXPECT_EQ(line, _line);
    EXPECT_EQ(string(func), _func);
    SetAssertionHandler(oldHandler);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
