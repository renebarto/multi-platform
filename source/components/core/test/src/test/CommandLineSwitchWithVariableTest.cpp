#include <unittest-cpp/UnitTestC++.h>

#include "core/CommandLineOption.h"
//#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineSwitchWithVariableTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void CommandLineSwitchWithVariableTest::SetUp()
{
}

void CommandLineSwitchWithVariableTest::TearDown()
{
}

static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_SUITE(core) {

template<class T>
class CommandLineSwitchWithVariableAccessor : public CommandLineSwitchWithVariable<T>
{
public:
    CommandLineSwitchWithVariableAccessor(const std::string & longName, const std::string & description, T & variable, T value)
        : CommandLineSwitchWithVariable<T>(longName, description, variable, value)
    {}
    void SetLongName(const std::string & name) { this->LongName(name); }
    void SetShortName(char name) { this->ShortName(name); }
    void SetDescription(const std::string & description) { this->Description(description); }
};

TEST_FIXTURE(CommandLineSwitchWithVariableTest, Construction)
{
    int variable {};
    int value = 1234;
    CommandLineSwitchWithVariable<int> option(LongName, Description, variable, value);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ('\0', option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ("", option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(value, option.Value());
    EXPECT_NE(value, variable);
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, GetSet)
{
    int variable {};
    int value = 1234;
    CommandLineSwitchWithVariableAccessor<int> option(LongName, Description, variable, value);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ("", option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(value, option.Value());
    EXPECT_NE(value, variable);
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, FoundOption)
{
    int variable {};
    int value = 1234;
    CommandLineSwitchWithVariableAccessor<int> option(LongName, Description, variable, value);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);
    option.SetOptionFound();

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ("", option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(value, option.Value());
    EXPECT_EQ(value, variable);
    EXPECT_TRUE(option.FoundOption());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
