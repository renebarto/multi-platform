#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOption.h"
#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineOptionTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void CommandLineOptionTest::SetUp()
{
}

void CommandLineOptionTest::TearDown()
{
}

static const CommandLineArgumentType ArgTypeDefault = CommandLineArgumentType::NoArgument;
static const CommandLineArgumentType ArgType = CommandLineArgumentType::OptionalArgument;
static const char ShortName = _('s');
static const string LongName = _("longname");
static const string Description = _("Some description");
static const CommandLineArgumentType ArgType2 = CommandLineArgumentType::RequiredArgument;
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

class CommandLineOptionAccessor : public CommandLineOption
{
public:
    CommandLineOptionAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument)
        : CommandLineOption(longName, shortName, description, argType)
    {}
    CommandLineOptionAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      OSAL::String & textVariable,
                      CommandLineArgumentType argType = CommandLineArgumentType::RequiredArgument)
        : CommandLineOption(longName, shortName, description, textVariable, argType)
    {}
    void SetLongName(const OSAL::String & name) { LongName(name); }
    void SetShortName(OSAL::Char name) { ShortName(name); }
    void SetDescription(const OSAL::String & description) { Description(description); }
};

TEST_FIXTURE(CommandLineOptionTest, Construction)
{
    CommandLineOption option(LongName, ShortName, Description);

    EXPECT_EQ(ArgTypeDefault, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_FALSE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionOptionTextVariable)
{
    OSAL::String textValue(_("Text"));
    CommandLineOption option(LongName, ShortName, Description, textValue);

    EXPECT_EQ(ArgTypeDefault, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(textValue, option.TextValue());
    EXPECT_FALSE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionArgType)
{
    CommandLineOption option(LongName, ShortName, Description, ArgType);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_FALSE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionOptionTextVariableArgType)
{
    OSAL::String textValue(_("Text"));
    CommandLineOption option(LongName, ShortName, Description, textValue, ArgType);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(textValue, option.TextValue());
    EXPECT_FALSE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, GetSet)
{
//    CommandLineOption option(LongName, ShortName, Description, ArgType);
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

TEST_FIXTURE(CommandLineOptionTest, FoundOption)
{
//    CommandLineOption option(LongName, ShortName, Description, ArgType);
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

TEST_FIXTURE(CommandLineOptionTest, Value)
{
//    CommandLineOption option(LongName, ShortName, Description, ArgType);
//
//    const string expected = "100";
//    option.Value(expected);
//
//    EXPECT_EQ(expected, option.Value());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
