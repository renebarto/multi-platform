#include <unit-test-c++/UnitTestC++.h>

#include "core/CommandLineParser.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class CommandLineParserTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
    void CheckDefinedVerboseBrief(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_("verbose")));
        EXPECT_TRUE(parser.HaveOption(_("brief")));
    }
    void CheckDefinedAdd(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('a')));
        EXPECT_TRUE(parser.HaveOption(_("add")));
    }
    void CheckDefinedAddOnly(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.HaveOption(_('a')));
        EXPECT_TRUE(parser.HaveOption(_("add")));
    }
    void CheckNotDefinedAdd(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.HaveOption(_('a')));
        EXPECT_FALSE(parser.HaveOption(_("add")));
    }
    void CheckDefinedAppend(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('b')));
        EXPECT_TRUE(parser.HaveOption(_("append")));
    }
    void CheckDefinedDelete(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('d')));
        EXPECT_TRUE(parser.HaveOption(_("delete")));
    }
    void CheckDefinedCreate(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('c')));
        EXPECT_TRUE(parser.HaveOption(_("create")));
    }
    void CheckDefinedFile(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('f')));
        EXPECT_TRUE(parser.HaveOption(_("file")));
    }
    void CheckDefinedX(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('x')));
        EXPECT_TRUE(parser.HaveOption(_("x")));
    }
    void CheckDefinedY(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption(_('y')));
        EXPECT_TRUE(parser.HaveOption(_("y")));
    }
    void CheckDefinedOptions(const CommandLineParser &parser) const
    {
        CheckDefinedVerboseBrief(parser);
        CheckDefinedAdd(parser);
        CheckDefinedAppend(parser);
        CheckDefinedDelete(parser);
        CheckDefinedCreate(parser);
        CheckDefinedFile(parser);
    }
    void CheckFoundVerbose(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_("verbose")));
    }
    void CheckNotFoundVerbose(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_("verbose")));
    }
    void CheckFoundBrief(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_("brief")));
    }
    void CheckNotFoundBrief(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_("brief")));
    }
    void CheckFoundAdd(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('a')));
        EXPECT_TRUE(parser.FoundOption(_("add")));
    }
    void CheckNotFoundAdd(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('a')));
        EXPECT_FALSE(parser.FoundOption(_("add")));
    }
    void CheckFoundAppend(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('b')));
        EXPECT_TRUE(parser.FoundOption(_("append")));
    }
    void CheckNotFoundAppend(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('b')));
        EXPECT_FALSE(parser.FoundOption(_("append")));
    }
    void CheckFoundDelete(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('d')));
        EXPECT_TRUE(parser.FoundOption(_("delete")));
    }
    void CheckNotFoundDelete(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('d')));
        EXPECT_FALSE(parser.FoundOption(_("delete")));
    }
    void CheckFoundDelete(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckFoundDelete(parser);
        EXPECT_EQ(value, parser.GetOption(_('d')));
        EXPECT_EQ(value, parser.GetOption(_("delete")));
    }
    void CheckFoundCreate(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('c')));
        EXPECT_TRUE(parser.FoundOption(_("create")));
    }
    void CheckNotFoundCreate(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('c')));
        EXPECT_FALSE(parser.FoundOption(_("create")));
    }
    void CheckFoundCreate(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckFoundCreate(parser);
        EXPECT_EQ(value, parser.GetOption(_('c')));
        EXPECT_EQ(value, parser.GetOption(_("create")));
    }
    void CheckFoundFile(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('f')));
        EXPECT_TRUE(parser.FoundOption(_("file")));
    }
    void CheckNotFoundFile(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('f')));
        EXPECT_FALSE(parser.FoundOption(_("file")));
    }
    void CheckFoundFile(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption(_('f')));
        EXPECT_EQ(value, parser.GetOption(_("file")));
    }
    void CheckFoundX(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('x')));
        EXPECT_TRUE(parser.FoundOption(_("x")));
    }
    void CheckNotFoundX(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('x')));
        EXPECT_FALSE(parser.FoundOption(_("x")));
    }
    void CheckFoundX(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckFoundX(parser);
        EXPECT_EQ(value, parser.GetOption(_('x')));
        EXPECT_EQ(value, parser.GetOption(_("x")));
    }
    void CheckNotFoundY(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption(_('y')));
        EXPECT_FALSE(parser.FoundOption(_("y")));
    }
    void CheckFoundY(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption(_('y')));
        EXPECT_TRUE(parser.FoundOption(_("y")));
    }
    void CheckFoundY(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckFoundY(parser);
        EXPECT_EQ(value, parser.GetOption(_('y')));
        EXPECT_EQ(value, parser.GetOption(_("y")));
    }
    void CheckNotFoundOptions(const CommandLineParser &parser) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckNotFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckNotFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckNotFoundFile(parser);
    }
    void CheckFoundVerboseOnly(const CommandLineParser &parser) const
    {
        CheckFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckNotFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckNotFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckNotFoundFile(parser);
    }
    void CheckFoundAddOnly(const CommandLineParser &parser) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckNotFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckNotFoundFile(parser);
    }
    void CheckFoundAddAppend(const CommandLineParser &parser) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckFoundAdd(parser);
        CheckFoundAppend(parser);
        CheckNotFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckNotFoundFile(parser);
    }
    void CheckFoundDeleteOnly(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckNotFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckNotFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption(_('d')));
        EXPECT_EQ(value, parser.GetOption(_("delete")));
    }
    void CheckFoundFileOnly(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckNotFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckNotFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption(_('f')));
        EXPECT_EQ(value, parser.GetOption(_("file")));
    }
};

void CommandLineParserTest::SetUp()
{
}

void CommandLineParserTest::TearDown()
{
}

static const OSAL::String ApplicationName = _("CommandLineParserTest");

TEST_FIXTURE(CommandLineParserTest, Construct)
{
    OSAL::Console console;
    CommandLineParser parser(console);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckNotDefinedAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n  CommandLineParserTest [OPTION...]\n\n\n  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoArgumentNoValue)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    parser.AddSwitch(_("add"), _('a'), _("Add me"));

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n  CommandLineParserTest [OPTION...]\n\n"
              "  -a, --add                 Add me\n\n"
              "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleSwitchNoVariable)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    parser.AddSwitch(_("add"), _('a'), _("Add me"));

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add                 Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleSwitchWithVariable)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int variable {};
    int value = 1;
    parser.AddSwitchWithVariable(_("add"), variable, value, _("Add me"));

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  --add                     Add me\n\n"
              "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableOptionalArgument)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    OSAL::String argument;
    parser.AddOptionOptionalArgument(_("add"), _('a'), _("Add me"), argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  -a, --add [argument]      Add me\n\n"
              "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableOptionalArgumentNoShortName)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    OSAL::String argument;
    parser.AddOptionOptionalArgument(_("add"), _("Add me"), argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  --add [argument]          Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableRequiredArgument)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    OSAL::String argument;
    parser.AddOptionRequiredArgument(_("add"), _('a'), _("Add me"), argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add <argument>      Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableRequiredArgumentNoShortName)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    OSAL::String argument;
    parser.AddOptionRequiredArgument(_("add"), _("Add me"), argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  --add <argument>          Add me\n\n"
              "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableOptionalArgument)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int variable {};
    parser.AddOptionOptionalArgument<int>(_("add"), _('a'), _("Add me"), variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add [argument]      Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableOptionalArgumentNoShortName)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int variable {};
    parser.AddOptionOptionalArgument<int>(_("add"), _("Add me"), variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  --add [argument]          Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableRequiredArgument)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int variable {};
    parser.AddOptionRequiredArgument<int>(_("add"), _('a'), _("Add me"), variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add <argument>      Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableRequiredArgumentNoShortName)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int variable {};
    parser.AddOptionRequiredArgument<int>(_("add"), _("Add me"), variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  --add <argument>          Add me\n\n"
                    "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupMultipleOptions)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ(_("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  --verbose                 Verbose output\n"
              "  --brief                   Brief output\n"
              "  -a, --add                 Add\n"
              "  -b, --append              Append\n"
              "  -d, --delete [argument]   Delete\n"
              "  -c, --create [argument]   Create\n"
              "  -f, --file <argument>     File\n"
              "  -x, --x [argument]        X\n"
              "  -y, --y <argument>        Y\n\n"
              "  --help                    Show help options\n\n"), parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, ParseNoArguments)
{
    int argc = 1;
    const OSAL::Char ** argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgumentValue)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--verbose");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(1, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundVerboseOnly(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgument)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--add");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddOnly(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseMultipleOptionsNoArgument)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-ab");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddAppend(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalNoArgument)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-d");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _(""));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalNoArgument)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--delete");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _(""));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgumentEqualsSign)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-d=abc"); // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_("abc"), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _("abc"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgumentEqualsSign)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--delete=abc"); // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_("abc"), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _("abc"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgument)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-d");
    argv[2]= _("abc"); // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _(""));
    EXPECT_EQ(_("abc"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgument)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--delete");
    argv[2] = _("abc"); // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _(""));
    EXPECT_EQ(_("abc"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredNoArgument)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-f");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_FALSE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredNoArgument)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--file");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_FALSE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgument)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-f");
    argv[2] = _("abc");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgument)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--file");
    argv[2] = _("abc");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgumentEqualsSign)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-f=abc"); // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgumentEqualsSign)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--file=abc"); // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseNonOption)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgumentValueExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--verbose");
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(1, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundVerboseOnly(parser);
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgumentExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--add");
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddOnly(parser);
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseMultipleOptionsNoArgumentExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-ab");
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddAppend(parser);
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalNoArgumentExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-d");
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _(""));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalNoArgumentExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--delete");
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _(""));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgumentExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-d=abc"); // Optional arguments require = sign
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_("abc"), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgumentExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--delete=abc"); // Optional arguments require = sign
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_("abc"), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgumentExtraNonOption)
{
    int argc = 4;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-f");
    argv[2] = _("abc");
    argv[3] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgumentExtraNonOption)
{
    int argc = 4;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--file");
    argv[2] = _("abc");
    argv[3] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgumentEqualsSignExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-f=abc"); // Optional arguments require = sign
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgumentEqualsSignExtraNonOption)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--file=abc"); // Optional arguments require = sign
    argv[2] = _("test");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseMoreComplexArguments)
{
    int argc = 8;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-a");
    argv[2] = _("test");
    argv[3] = _("-bf");
    argv[4] = _("abc");
    argv[5] = _("-dc=");
    argv[6] = _("def");
    argv[7] = _("--brief");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_("c="), deleteString);
    EXPECT_EQ(_(""), createString);
    EXPECT_EQ(_("abc"), fileString);
    EXPECT_EQ(size_t{2}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundVerbose(parser);
    CheckFoundBrief(parser);
    CheckFoundAdd(parser);
    CheckFoundAppend(parser);
    CheckFoundDelete(parser, _("c="));
    CheckNotFoundCreate(parser);
    CheckFoundFile(parser, _("abc"));
    EXPECT_EQ(_("test"), parser.GetNonOption(0));
    EXPECT_EQ(_("def"), parser.GetNonOption(1));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseQuotedString)
{
    int argc = 4;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("-a");
    argv[2] = _("\"a b\"");
    argv[3] = _("-c=\"d e f\"");
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, 1, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, 0, _("Brief output"));
    parser.AddSwitch(_("add"), _('a'), _("Add"));
    parser.AddSwitch(_("append"), _('b'), _("Append"));
    parser.AddOptionOptionalArgument(_("delete"), _('d'), _("Delete"), deleteString);
    parser.AddOptionOptionalArgument(_("create"), _('c'), _("Create"), createString);
    parser.AddOptionRequiredArgument(_("file"), _('f'), _("File"), fileString);
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ(_(""), deleteString);
    EXPECT_EQ(_("d e f"), createString);
    EXPECT_EQ(_(""), fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundVerbose(parser);
    CheckNotFoundBrief(parser);
    CheckFoundAdd(parser);
    CheckNotFoundAppend(parser);
    CheckNotFoundDelete(parser);
    CheckFoundCreate(parser, _("d e f"));
    CheckNotFoundFile(parser);
    EXPECT_EQ(_("a b"), parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSwitchWithBoolVariableBrief)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--brief");
    OSAL::Console console;
    CommandLineParser parser(console);
    bool verboseFlag = true;
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, true, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, false, _("Brief output"));

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_FALSE(verboseFlag);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedVerboseBrief(parser);
    CheckNotFoundVerbose(parser);
    CheckFoundBrief(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSwitchWithBoolVariableVerbose)
{
    int argc = 2;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--verbose");
    OSAL::Console console;
    CommandLineParser parser(console);
    bool verboseFlag {};
    parser.AddSwitchWithVariable(_("verbose"), verboseFlag, true, _("Verbose output"));
    parser.AddSwitchWithVariable(_("brief"), verboseFlag, false, _("Brief output"));

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_TRUE(verboseFlag);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedVerboseBrief(parser);
    CheckFoundVerbose(parser);
    CheckNotFoundBrief(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionWithIntVariableEqualsSign)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--x=1234");
    argv[2] = _("-y=5678");
    OSAL::Console console;
    CommandLineParser parser(console);
    int x {};
    int y {};
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(1234, x);
    EXPECT_EQ(5678, y);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedX(parser);
    CheckFoundX(parser, _("1234"));
    CheckDefinedY(parser);
    CheckFoundY(parser, _("5678"));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionWithIntVariable)
{
    int argc = 3;
    const OSAL::Char * * argv = new const OSAL::Char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = _("--y");
    argv[2] = _("5678");
    OSAL::Console console;
    CommandLineParser parser(console);
    int x {};
    int y {};
    parser.AddOptionOptionalArgument<int>(_("x"), _('x'), _("X"), x);
    parser.AddOptionRequiredArgument<int>(_("y"), _('y'), _("Y"), y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, x);
    EXPECT_EQ(5678, y);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedX(parser);
    CheckNotFoundX(parser);
    CheckDefinedY(parser);
    CheckFoundY(parser, _("5678"));

    delete [] argv;
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core