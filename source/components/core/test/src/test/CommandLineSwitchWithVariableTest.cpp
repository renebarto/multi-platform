#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOption.h"
#include "core/Util.h"

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

static const char ShortName = _('s');
static const string LongName = _("longname");
static const string Description = _("Some description");
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

TEST_FIXTURE(CommandLineSwitchWithVariableTest, Construction)
{
    int variable {};
    int value = 1234;
    CommandLineSwitchWithVariable<int> option(LongName, Description, variable, value);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(_('\0'), option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(value, option.Value());
    EXPECT_NE(value, variable);
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, ConstructionOptionFoundFlag)
{
//    OSAL::String textValue(_("Text"));
//    CommandLineSwitchWithVariable option(LongName, _('\0'), Description, textValue);
//
//    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
//    EXPECT_EQ(LongName, option.LongName());
//    EXPECT_EQ(_('\0'), option.ShortName());
//    EXPECT_EQ(Description, option.Description());
//    EXPECT_EQ(textValue, option.TextValue());
//    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, ConstructionArgType)
{
//    CommandLineSwitchWithVariable option(LongName, _('\0'), Description, ArgType);
//
//    EXPECT_EQ(ArgType, option.ArgType());
//    EXPECT_EQ(LongName, option.LongName());
//    EXPECT_EQ(_('\0'), option.ShortName());
//    EXPECT_EQ(Description, option.Description());
//    EXPECT_EQ(_(""), option.TextValue());
//    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, ConstructionOptionFoundFlagArgType)
{
//    OSAL::String textValue(_("Text"));
//    CommandLineSwitchWithVariable option(LongName, _('\0'), Description, textValue, ArgType);
//
//    EXPECT_EQ(ArgType, option.ArgType());
//    EXPECT_EQ(LongName, option.LongName());
//    EXPECT_EQ(_('\0'), option.ShortName());
//    EXPECT_EQ(Description, option.Description());
//    EXPECT_EQ(textValue, option.TextValue());
//    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, GetSet)
{
//    CommandLineSwitchWithVariable option(LongName, _('\0'), Description, ArgType);
//
//    option.LongName(LongName2);
//    option.ShortName(ShortName2);
//    option.Description(Description2);
//    option.OptionFoundFlag(&optionFound);
//
//    EXPECT_EQ(ArgType, option.ArgType());
//    EXPECT_EQ(ShortName2, option.ShortName());
//    EXPECT_EQ(LongName2, option.LongName());
//    EXPECT_EQ(Description2, option.Description());
//    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
//    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, FoundOption)
{
//    CommandLineSwitchWithVariable option(LongName, _('\0'), Description, ArgType);
//
//    option.LongName(LongName2);
//    option.ShortName(ShortName2);
//    option.Description(Description2);
//    option.OptionFoundFlag(&optionFound);
//    option.FoundOption(true);
//
//    EXPECT_EQ(ArgType, option.ArgType());
//    EXPECT_EQ(ShortName2, option.ShortName());
//    EXPECT_EQ(LongName2, option.LongName());
//    EXPECT_EQ(Description2, option.Description());
//    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
//    EXPECT_TRUE(optionFound);
//    EXPECT_TRUE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchWithVariableTest, Value)
{
//    CommandLineSwitchWithVariable option(LongName, _('\0'), Description, ArgType);
//
//    const string expected = "100";
//    option.Value(expected);
//
//    EXPECT_EQ(expected, option.Value());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
