#include <unittest-c++/UnitTestC++.h>
#include "core/CommandLineOption.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineSwitchNoVariableTest : public UnitTestCpp::TestFixture
{
};

static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_SUITE(core) {

class CommandLineSwitchNoVariableAccessor : public CommandLineSwitchNoVariable
{
public:
    CommandLineSwitchNoVariableAccessor(const string & longName, char shortName, const string & description)
    : CommandLineSwitchNoVariable(longName, shortName, description)
    {}
    void SetLongName(const string & name) { LongName(name); }
    void SetShortName(char name) { ShortName(name); }
    void SetDescription(const string & description) { Description(description); }
};

TEST_FIXTURE(CommandLineSwitchNoVariableTest, Construction)
{
    CommandLineSwitchNoVariable option(LongName, ShortName, Description);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ("", option.TextValue());
    EXPECT_TRUE(option.IsSwitchNoVariable());
    EXPECT_FALSE(option.IsSwitchWithVariable());
    EXPECT_FALSE(option.IsOptionWithVariable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchNoVariableTest, GetSet)
{
    CommandLineSwitchNoVariableAccessor option(LongName, ShortName, Description);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineSwitchNoVariableTest, FoundOption)
{
    CommandLineSwitchNoVariableAccessor option(LongName, ShortName, Description);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);
    option.SetOptionFound();

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_TRUE(option.FoundOption());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
