#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOption.h"
#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineOptionWithVariableTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void CommandLineOptionWithVariableTest::SetUp()
{
}

void CommandLineOptionWithVariableTest::TearDown()
{
}

static const char ShortName = _('s');
static const string LongName = _("longname");
static const string Description = _("Some description");
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

template<class T>
class CommandLineOptionWithVariableAccessor : public CommandLineOptionWithVariable<T>
{
public:
    CommandLineOptionWithVariableAccessor(const OSAL::String & longName, OSAL::Char shortName,
                                          const OSAL::String & description, T & variable)
        : CommandLineOptionWithVariable<T>(longName, shortName, description, variable)
    {}
    void SetLongName(const OSAL::String & name) { this->LongName(name); }
    void SetShortName(OSAL::Char name) { this->ShortName(name); }
    void SetDescription(const OSAL::String & description) { this->Description(description); }
};

TEST_FIXTURE(CommandLineOptionWithVariableTest, Construction)
{
    int initialValue {};
    int variable = initialValue;
    CommandLineOptionWithVariable<int> option(LongName, ShortName, Description, variable);

    EXPECT_EQ(CommandLineArgumentType::RequiredArgument, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(initialValue, option.Variable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithVariableTest, GetSet)
{
    int initialValue {};
    int variable = initialValue;
    CommandLineOptionWithVariableAccessor<int> option(LongName, ShortName, Description, variable);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);

    EXPECT_EQ(CommandLineArgumentType::RequiredArgument, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_EQ(initialValue, option.Variable());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithVariableTest, FoundOption)
{
    int initialValue {};
    int variable = initialValue;
    CommandLineOptionWithVariableAccessor<int> option(LongName, ShortName, Description, variable);

    option.SetLongName(LongName2);
    option.SetShortName(ShortName2);
    option.SetDescription(Description2);
    option.SetOptionFound();
    variable = 1234;

    EXPECT_EQ(CommandLineArgumentType::RequiredArgument, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(_(""), option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_NE(initialValue, option.Variable());
    EXPECT_TRUE(option.FoundOption());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
