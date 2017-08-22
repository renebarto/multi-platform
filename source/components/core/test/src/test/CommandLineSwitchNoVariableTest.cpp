#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOption.h"
#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineSwitchNoVariableTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void CommandLineSwitchNoVariableTest::SetUp()
{
}

void CommandLineSwitchNoVariableTest::TearDown()
{
}

static const char ShortName = _('s');
static const string LongName = _("longname");
static const string Description = _("Some description");
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

class CommandLineSwitchNoVariableAccessor : public CommandLineSwitchNoVariable
{
public:
    CommandLineSwitchNoVariableAccessor(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description)
    : CommandLineSwitchNoVariable(longName, shortName, description)
    {}
    void SetLongName(const OSAL::String & name) { LongName(name); }
    void SetShortName(OSAL::Char name) { ShortName(name); }
    void SetDescription(const OSAL::String & description) { Description(description); }
};

TEST_FIXTURE(CommandLineSwitchNoVariableTest, Construction)
{
    CommandLineSwitchNoVariable option(LongName, ShortName, Description);

    EXPECT_EQ(CommandLineArgumentType::NoArgument, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
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
