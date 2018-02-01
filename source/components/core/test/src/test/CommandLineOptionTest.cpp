#include <unittest-c++/UnitTestC++.h>
#include "core/CommandLineOption.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineOptionTest : public UnitTestCpp::TestFixture
{
};

static const CommandLineArgumentType ArgType = CommandLineArgumentType::OptionalArgument;
static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_SUITE(core) {

class CommandLineOptionAccessor : public CommandLineOption
{
public:
    CommandLineOptionAccessor(const string & longName, char shortName, const string & description)
        : CommandLineOption(longName, shortName, description)
    {}
    CommandLineOptionAccessor(const string & longName, char shortName, const string & description,
                              CommandLineArgumentType argType)
        : CommandLineOption(longName, shortName, description, argType)
    {}
    CommandLineOptionAccessor(const string & longName, char shortName, const string & description,
                              string & textVariable)
        : CommandLineOption(longName, shortName, description, textVariable)
    {}
    CommandLineOptionAccessor(const string & longName, char shortName, const string & description,
                              string & textVariable,
                              CommandLineArgumentType argType)
        : CommandLineOption(longName, shortName, description, textVariable, argType)
    {}
    void SetLongName(const string & name) { LongName(name); }
    void SetShortName(char name) { ShortName(name); }
    void SetDescription(const string & description) { Description(description); }
};

TEST_FIXTURE(CommandLineOptionTest, PrintArgType)
{
    std::basic_ostringstream<char> stream;
    stream << CommandLineArgumentType::NoArgument;
    EXPECT_EQ("None", stream.str());

    stream.str("");
    stream << CommandLineArgumentType::OptionalArgument;
    EXPECT_EQ("Optional", stream.str());

    stream.str("");
    stream << CommandLineArgumentType::RequiredArgument;
    EXPECT_EQ("Required", stream.str());
}

TEST_FIXTURE(CommandLineOptionTest, Construction)
{
    CommandLineOption option(LongName, ShortName, Description);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ("", option.TextValue());
    EXPECT_FALSE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionOptionTextVariable)
{
    string textValue("Text");
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
    EXPECT_EQ("", option.TextValue());
    EXPECT_FALSE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionOptionTextVariableArgType)
{
    string textValue("Text");
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
    string textValue("Text");
    string newTextValue("1234");
    CommandLineOption option(LongName, ShortName, Description, textValue, ArgType);

    EXPECT_EQ(textValue, option.TextValue());
    EXPECT_EQ("", option.Argument());

    option.TextValue(newTextValue);

    EXPECT_EQ(newTextValue, option.TextValue());
    EXPECT_EQ(newTextValue, textValue);
    EXPECT_EQ("", option.Argument());
}

TEST_FIXTURE(CommandLineOptionTest, GetSetArgument)
{
    string textValue;
    string argument("Text");
    CommandLineOption option(LongName, ShortName, Description, textValue, ArgType);

    EXPECT_EQ(textValue, option.TextValue());
    EXPECT_EQ("", option.Argument());

    option.Argument(argument);

    EXPECT_EQ(argument, option.TextValue());
    EXPECT_EQ(argument, textValue);
    EXPECT_EQ(argument, option.Argument());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
