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

static const CommandLineArgumentType ArgType = CommandLineArgumentType::OptionalArgument;
static const char ShortName = _('s');
static const string LongName = _("longname");
static const string Description = _("Some description");
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

class CommandLineOptionAccessor : public CommandLineOption
{
public:
    CommandLineOptionAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description)
        : CommandLineOption(longName, shortName, description)
    {}
    CommandLineOptionAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                              CommandLineArgumentType argType)
        : CommandLineOption(longName, shortName, description, argType)
    {}
    CommandLineOptionAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                              OSAL::String & textVariable)
        : CommandLineOption(longName, shortName, description, textVariable)
    {}
    CommandLineOptionAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                              OSAL::String & textVariable,
                              CommandLineArgumentType argType)
        : CommandLineOption(longName, shortName, description, textVariable, argType)
    {}
    void SetLongName(const OSAL::String & name) { LongName(name); }
    void SetShortName(OSAL::Char name) { ShortName(name); }
    void SetDescription(const OSAL::String & description) { Description(description); }
};

TEST_FIXTURE(CommandLineOptionTest, PrintArgType)
{
    std::basic_ostringstream<OSAL::Char> stream;
    stream << CommandLineArgumentType::NoArgument;
    EXPECT_EQ(_("None"), stream.str());

    stream.str(_(""));
    stream << CommandLineArgumentType::OptionalArgument;
    EXPECT_EQ(_("Optional"), stream.str());

    stream.str(_(""));
    stream << CommandLineArgumentType::RequiredArgument;
    EXPECT_EQ(_("Required"), stream.str());
}

TEST_FIXTURE(CommandLineOptionTest, Construction)
{
    CommandLineOption option(LongName, ShortName, Description);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
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

    EXPECT_EQ(CommandLineArgumentType::RequiredArgument, option.ArgType());
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
    CommandLineOptionAccessor option(LongName, ShortName, Description, ArgType);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);
    option.SetOptionFound();

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_TRUE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, GetSetTextValue)
{
    OSAL::String textValue(_("Text"));
    OSAL::String newTextValue(_("1234"));
    CommandLineOption option(LongName, ShortName, Description, textValue, ArgType);

    EXPECT_EQ(textValue, option.TextValue());
    EXPECT_EQ(_(""), option.Argument());

    option.TextValue(newTextValue);

    EXPECT_EQ(newTextValue, option.TextValue());
    EXPECT_EQ(newTextValue, textValue);
    EXPECT_EQ(_(""), option.Argument());
}

TEST_FIXTURE(CommandLineOptionTest, GetSetArgument)
{
    OSAL::String textValue;
    OSAL::String argument(_("Text"));
    CommandLineOption option(LongName, ShortName, Description, textValue, ArgType);

    EXPECT_EQ(textValue, option.TextValue());
    EXPECT_EQ(_(""), option.Argument());

    option.Argument(argument);

    EXPECT_EQ(argument, option.TextValue());
    EXPECT_EQ(argument, textValue);
    EXPECT_EQ(argument, option.Argument());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
