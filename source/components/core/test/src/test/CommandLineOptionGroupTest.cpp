#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOptionGroup.h"
#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

class CommandLineOptionGroupTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    OSAL::String theParameterText;
    int theParameter;
    bool optionFound;
};


void CommandLineOptionGroupTest::SetUp()
{
}

void CommandLineOptionGroupTest::TearDown()
{
}

static const string GroupName = _("group");
static const string GroupDescription = _("Group description");
static const string GroupName2 = _("group2");
static const string GroupDescription2 = _("Group2 description");

static const CommandLineArgumentType ArgType = CommandLineArgumentType::NoArgument;
static const CommandLineArgumentType ArgTypeRequired = CommandLineArgumentType::RequiredArgument;
static const CommandLineArgumentType ArgTypeOptional = CommandLineArgumentType::OptionalArgument;
static const char ShortName = _('s');
static const string LongName = _("longname");
static const string Description = _("Some description");
static const char ShortName2 = _('t');
static const string LongName2 = _("othername");
static const string Description2 = _("Some other description");

TEST_SUITE(core) {

TEST_FIXTURE(CommandLineOptionGroupTest, Construction)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    EXPECT_EQ(GroupName, optionGroup.Name());
//    EXPECT_EQ(GroupDescription, optionGroup.Description());
//    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, GetSet)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    optionGroup.Name(GroupName2);
//    optionGroup.Description(GroupDescription2);
//
//    EXPECT_EQ(GroupName2, optionGroup.Name());
//    EXPECT_EQ(GroupDescription2, optionGroup.Description());
//    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, Add)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//    CommandLineOption::Ptr option1(new CommandLineOption(LongName, ShortName, Description));
//    CommandLineOption::Ptr option2(new CommandLineOptionWithVariable<int>(LongName2, ShortName2,
//                                                                          Description2, theParameter));
//
//    optionGroup.AddOption(option1);
//    optionGroup.AddOption(option2);
//
//    EXPECT_EQ(GroupName, optionGroup.Name());
//    EXPECT_EQ(GroupDescription, optionGroup.Description());
//    EXPECT_EQ(size_t(2), optionGroup.Options().size());
//    EXPECT_EQ(ArgType, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    EXPECT_EQ(ArgTypeRequired, optionGroup.Options()[1]->ArgType());
//    EXPECT_EQ(LongName2, optionGroup.Options()[1]->LongName());
//    EXPECT_EQ(ShortName2, optionGroup.Options()[1]->ShortName());
//    EXPECT_EQ(Description2, optionGroup.Options()[1]->Description());
//    EXPECT_NOT_NULL(dynamic_cast<const CommandLineOptionWithVariable<int> *>(optionGroup.Options()[1].get()));
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionNoArgument)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    optionGroup.AddSwitch(LongName, ShortName, Description);
//    EXPECT_EQ(size_t(1), optionGroup.Options().size());
//    EXPECT_EQ(ArgType, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    EXPECT_FALSE(optionGroup.Options()[0]->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionNoArgumentFoundOption)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    int variable;
//    int value = 3;
//    optionGroup.AddSwitchWithVariable<int>(LongName, variable, value, Description);
//    EXPECT_EQ(size_t(1), optionGroup.Options().size());
//    EXPECT_EQ(ArgType, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(_('\0'), optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    EXPECT_FALSE(optionGroup.Options()[0]->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgument)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    OSAL::String variable;
//    optionGroup.AddOptionRequiredArgument(LongName, Description, variable);
//    EXPECT_EQ(size_t(1), optionGroup.Options().size());
//    EXPECT_EQ(ArgTypeRequired, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    const CommandLineOptionWithVariable<int> * option =
//        dynamic_cast<const CommandLineOptionWithVariable<int> *>(optionGroup.Options()[0].get());
//    EXPECT_NOT_NULL(option);
//    EXPECT_EQ(variable, option->TextValue());
//    EXPECT_FALSE(optionGroup.Options()[0]->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgumentFoundOption)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    optionGroup.AddOptionRequiredArgumentWithVariable(LongName, ShortName, Description, theParameter);
//    EXPECT_EQ(size_t(1), optionGroup.Options().size());
//    EXPECT_EQ(ArgTypeRequired, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    CommandLineOptionWithVariable<int> * option =
//        dynamic_cast<CommandLineOptionWithVariable<int> *>(optionGroup.Options()[0].get());
//    EXPECT_NOT_NULL(option);
//    EXPECT_EQ(theParameter, option->Variable());
//    EXPECT_FALSE(optionGroup.Options()[0]->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgument)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    OSAL::String variable;
//    optionGroup.AddOptionOptionalArgument(LongName, ShortName, Description, variable);
//    EXPECT_EQ(size_t(1), optionGroup.Options().size());
//    EXPECT_EQ(ArgTypeOptional, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    const CommandLineOption * option =
//        dynamic_cast<const CommandLineOption *>(optionGroup.Options()[0].get());
//    EXPECT_NOT_NULL(option);
//    EXPECT_EQ(variable, option->TextValue());
//    EXPECT_FALSE(optionGroup.Options()[0]->FoundOption());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgumentFoundOption)
{
//    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
//
//    optionGroup.AddOptionOptionalArgumentWithVariable(LongName, ShortName, Description, theParameter);
//    EXPECT_EQ(size_t(1), optionGroup.Options().size());
//    EXPECT_EQ(ArgTypeOptional, optionGroup.Options()[0]->ArgType());
//    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
//    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
//    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
//    CommandLineOptionWithVariable<int> * option =
//        dynamic_cast<CommandLineOptionWithVariable<int> *>(optionGroup.Options()[0].get());
//    EXPECT_NOT_NULL(option);
//    EXPECT_EQ(theParameter, option->Variable());
//    EXPECT_FALSE(optionGroup.Options()[0]->FoundOption());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
