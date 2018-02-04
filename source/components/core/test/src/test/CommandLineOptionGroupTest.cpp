#include <unittest-cpp/UnitTestC++.h>
#include "core/CommandLineOptionGroup.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineOptionGroupTest : public UnitTestCpp::TestFixture
{
};


static const string GroupName = "group";
static const string GroupDescription = "Group description";
static const string GroupName2 = "group2";
static const string GroupDescription2 = "Group2 description";

static const CommandLineArgumentType ArgTypeNone = CommandLineArgumentType::NoArgument;
static const CommandLineArgumentType ArgTypeRequired = CommandLineArgumentType::RequiredArgument;
static const CommandLineArgumentType ArgTypeOptional = CommandLineArgumentType::OptionalArgument;
static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_SUITE(core) {

TEST_FIXTURE(CommandLineOptionGroupTest, Construction)
{
    CommandLineOptionGroup optionGroup(GroupName);

    EXPECT_EQ(GroupName, optionGroup.Name());
    EXPECT_EQ("", optionGroup.Description());
    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, ConstructionDescription)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    EXPECT_EQ(GroupName, optionGroup.Name());
    EXPECT_EQ(GroupDescription, optionGroup.Description());
    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, GetSet)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.Name(GroupName2);
    optionGroup.Description(GroupDescription2);

    EXPECT_EQ(GroupName2, optionGroup.Name());
    EXPECT_EQ(GroupDescription2, optionGroup.Description());
    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, Add)
{
    int variable1 {};
    int value2 = 1234;
    int variable2 {};
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
    CommandLineOption::Ptr option1(new CommandLineOption(LongName, ShortName, Description));
    CommandLineOption::Ptr option2(new CommandLineOptionWithVariable<int>(LongName2, ShortName2,
                                                                          Description2, variable1));
    CommandLineOption::Ptr option3(new CommandLineSwitchNoVariable(LongName, ShortName, Description));
    CommandLineOption::Ptr option4(new CommandLineSwitchWithVariable<int>(LongName2, Description2, variable2, value2));

    optionGroup.AddOption(option1);
    optionGroup.AddOption(option2);
    optionGroup.AddOption(option3);
    optionGroup.AddOption(option4);

    EXPECT_EQ(GroupName, optionGroup.Name());
    EXPECT_EQ(GroupDescription, optionGroup.Description());
    EXPECT_EQ(size_t(4), optionGroup.Options().size());

    EXPECT_EQ(option1, optionGroup.Options()[0]);
    EXPECT_EQ(option2, optionGroup.Options()[1]);
    EXPECT_EQ(option3, optionGroup.Options()[2]);
    EXPECT_EQ(option4, optionGroup.Options()[3]);
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddSwitchNoVariable)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddSwitch(LongName, ShortName, Description);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeNone, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ(ShortName, option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ("", option->TextValue());
    EXPECT_TRUE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_FALSE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddSwitchWithVariable)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    int variable {};
    int value = 1234;
    optionGroup.AddSwitchWithVariable<int>(LongName, variable, value, Description);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeNone, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ('\0', option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ("", option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_TRUE(option->IsSwitchWithVariable());
    EXPECT_FALSE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
    auto switchWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(option);
    ASSERT_NOT_NULL(switchWithVariable);
    EXPECT_EQ(variable, switchWithVariable->Variable());
    EXPECT_EQ(value, switchWithVariable->Value());
    EXPECT_NE(value, variable);
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgument)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionOptionalArgument(LongName, ShortName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeOptional, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ(ShortName, option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_FALSE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgumentNoShortName)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionOptionalArgument(LongName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeOptional, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ('\0', option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_FALSE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgument)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionRequiredArgument(LongName, ShortName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeRequired, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ(ShortName, option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_FALSE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgumentNoShortName)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionRequiredArgument(LongName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeRequired, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ('\0', option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_FALSE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgumentWithVariable)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionOptionalArgumentWithVariable(LongName, ShortName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeOptional, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ(ShortName, option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_TRUE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
    auto optionWithVariable = dynamic_pointer_cast<CommandLineOptionWithVariable<string>>(option);
    ASSERT_NOT_NULL(optionWithVariable);
    EXPECT_EQ(variable, optionWithVariable->Variable());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgumentWithVariableNoShortName)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionOptionalArgumentWithVariable(LongName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeOptional, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ('\0', option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_TRUE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
    auto optionWithVariable = dynamic_pointer_cast<CommandLineOptionWithVariable<string>>(option);
    ASSERT_NOT_NULL(optionWithVariable);
    EXPECT_EQ(variable, optionWithVariable->Variable());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgumentWithVariable)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionRequiredArgumentWithVariable(LongName, ShortName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeRequired, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ(ShortName, option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_TRUE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
    auto optionWithVariable = dynamic_pointer_cast<CommandLineOptionWithVariable<string>>(option);
    ASSERT_NOT_NULL(optionWithVariable);
    EXPECT_EQ(variable, optionWithVariable->Variable());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgumentWithVariableNoShortName)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    string variable;
    optionGroup.AddOptionRequiredArgumentWithVariable(LongName, Description, variable);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    auto option = optionGroup.Options()[0];
    ASSERT_NOT_NULL(option);
    EXPECT_EQ(ArgTypeRequired, option->ArgType());
    EXPECT_EQ(LongName, option->LongName());
    EXPECT_EQ('\0', option->ShortName());
    EXPECT_EQ(Description, option->Description());
    EXPECT_EQ(variable, option->TextValue());
    EXPECT_FALSE(option->IsSwitchNoVariable());
    EXPECT_FALSE(option->IsSwitchWithVariable());
    EXPECT_TRUE(option->IsOptionWithVariable());
    EXPECT_FALSE(option->FoundOption());
    auto optionWithVariable = dynamic_pointer_cast<CommandLineOptionWithVariable<string>>(option);
    ASSERT_NOT_NULL(optionWithVariable);
    EXPECT_EQ(variable, optionWithVariable->Variable());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
