#include <unittest-c++/UnitTestC++.h>
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

static const string LongName = _("longname");
static const string Description = _("Some description");
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

template<class T>
class CommandLineSwitchWithVariableAccessor : public CommandLineSwitchWithVariable<T>
{
public:
    CommandLineSwitchWithVariableAccessor(const OSAL::String & longName, const OSAL::String & description, T & variable, T value)
        : CommandLineSwitchWithVariable<T>(longName, description, variable, value)
    {}
    void SetLongName(const OSAL::String & name) { this->LongName(name); }
    void SetShortName(OSAL::Char name) { this->ShortName(name); }
    void SetDescription(const OSAL::String & description) { this->Description(description); }
};

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
    EXPECT_EQ(_(""), option.TextValue());
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
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(value, option.Value());
    EXPECT_EQ(value, variable);
    EXPECT_TRUE(option.FoundOption());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
