#include <unittest-c++/UnitTestC++.h>
#include "core/CommandLineOption.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineOptionWithVariableTest : public UnitTestCpp::TestFixture
{
};

static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_SUITE(core) {

template<class T>
class CommandLineOptionWithVariableAccessor : public CommandLineOptionWithVariable<T>
{
public:
    CommandLineOptionWithVariableAccessor(const string & longName, char shortName,
                                          const string & description, T & variable)
        : CommandLineOptionWithVariable<T>(longName, shortName, description, variable)
    {}
    void SetLongName(const string & name) { this->LongName(name); }
    void SetShortName(char name) { this->ShortName(name); }
    void SetDescription(const string & description) { this->Description(description); }
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
    EXPECT_EQ("", option.TextValue());
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
    EXPECT_EQ("", option.TextValue());
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
    EXPECT_EQ("", option.TextValue());
    EXPECT_EQ(variable, option.Variable());
    EXPECT_NE(initialValue, option.Variable());
    EXPECT_TRUE(option.FoundOption());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
