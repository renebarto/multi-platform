#include <unittest-cpp/UnitTestC++.h>

#include "core/CommandLineParser.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

class CommandLineParserTest : public UnitTestCpp::TestFixture
{
public:
    CommandLineParserTest()
        : _stream()
        , _console(_stream)
    {}
    
    std::ostringstream _stream;
    OSAL::Console _console;

    void CheckDefinedVerboseBrief(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption("verbose"));
        EXPECT_TRUE(parser.HaveOption("brief"));
    }
    void CheckDefinedAdd(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('a'));
        EXPECT_TRUE(parser.HaveOption("add"));
    }
    void CheckDefinedAddOnly(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.HaveOption('a'));
        EXPECT_TRUE(parser.HaveOption("add"));
    }
    void CheckNotDefinedAdd(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.HaveOption('a'));
        EXPECT_FALSE(parser.HaveOption("add"));
    }
    void CheckDefinedAppend(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('b'));
        EXPECT_TRUE(parser.HaveOption("append"));
    }
    void CheckDefinedDelete(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('d'));
        EXPECT_TRUE(parser.HaveOption("delete"));
    }
    void CheckDefinedCreate(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('c'));
        EXPECT_TRUE(parser.HaveOption("create"));
    }
    void CheckDefinedFile(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('f'));
        EXPECT_TRUE(parser.HaveOption("file"));
    }
    void CheckDefinedX(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('x'));
        EXPECT_TRUE(parser.HaveOption("x"));
    }
    void CheckDefinedY(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.HaveOption('y'));
        EXPECT_TRUE(parser.HaveOption("y"));
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
        EXPECT_TRUE(parser.FoundOption("verbose"));
    }
    void CheckNotFoundVerbose(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption("verbose"));
    }
    void CheckFoundBrief(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption("brief"));
    }
    void CheckNotFoundBrief(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption("brief"));
    }
    void CheckFoundAdd(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('a'));
        EXPECT_TRUE(parser.FoundOption("add"));
    }
    void CheckNotFoundAdd(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('a'));
        EXPECT_FALSE(parser.FoundOption("add"));
    }
    void CheckFoundAppend(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('b'));
        EXPECT_TRUE(parser.FoundOption("append"));
    }
    void CheckNotFoundAppend(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('b'));
        EXPECT_FALSE(parser.FoundOption("append"));
    }
    void CheckFoundDelete(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('d'));
        EXPECT_TRUE(parser.FoundOption("delete"));
    }
    void CheckNotFoundDelete(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('d'));
        EXPECT_FALSE(parser.FoundOption("delete"));
    }
    void CheckFoundDelete(const CommandLineParser &parser, const string & value) const
    {
        CheckFoundDelete(parser);
        EXPECT_EQ(value, parser.GetOption('d'));
        EXPECT_EQ(value, parser.GetOption("delete"));
    }
    void CheckFoundCreate(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('c'));
        EXPECT_TRUE(parser.FoundOption("create"));
    }
    void CheckNotFoundCreate(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('c'));
        EXPECT_FALSE(parser.FoundOption("create"));
    }
    void CheckFoundCreate(const CommandLineParser &parser, const string & value) const
    {
        CheckFoundCreate(parser);
        EXPECT_EQ(value, parser.GetOption('c'));
        EXPECT_EQ(value, parser.GetOption("create"));
    }
    void CheckFoundFile(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('f'));
        EXPECT_TRUE(parser.FoundOption("file"));
    }
    void CheckNotFoundFile(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('f'));
        EXPECT_FALSE(parser.FoundOption("file"));
    }
    void CheckFoundFile(const CommandLineParser &parser, const string & value) const
    {
        CheckFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption('f'));
        EXPECT_EQ(value, parser.GetOption("file"));
    }
    void CheckFoundX(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('x'));
        EXPECT_TRUE(parser.FoundOption("x"));
    }
    void CheckNotFoundX(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('x'));
        EXPECT_FALSE(parser.FoundOption("x"));
    }
    void CheckFoundX(const CommandLineParser &parser, const string & value) const
    {
        CheckFoundX(parser);
        EXPECT_EQ(value, parser.GetOption('x'));
        EXPECT_EQ(value, parser.GetOption("x"));
    }
    void CheckNotFoundY(const CommandLineParser &parser) const
    {
        EXPECT_FALSE(parser.FoundOption('y'));
        EXPECT_FALSE(parser.FoundOption("y"));
    }
    void CheckFoundY(const CommandLineParser &parser) const
    {
        EXPECT_TRUE(parser.FoundOption('y'));
        EXPECT_TRUE(parser.FoundOption("y"));
    }
    void CheckFoundY(const CommandLineParser &parser, const string & value) const
    {
        CheckFoundY(parser);
        EXPECT_EQ(value, parser.GetOption('y'));
        EXPECT_EQ(value, parser.GetOption("y"));
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
    void CheckFoundDeleteOnly(const CommandLineParser &parser, const string & value) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckNotFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckNotFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption('d'));
        EXPECT_EQ(value, parser.GetOption("delete"));
    }
    void CheckFoundFileOnly(const CommandLineParser &parser, const string & value) const
    {
        CheckNotFoundVerbose(parser);
        CheckNotFoundBrief(parser);
        CheckNotFoundAdd(parser);
        CheckNotFoundAppend(parser);
        CheckNotFoundDelete(parser);
        CheckNotFoundCreate(parser);
        CheckFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption('f'));
        EXPECT_EQ(value, parser.GetOption("file"));
    }
};

static const string ApplicationName = "CommandLineParserTest";

TEST_FIXTURE(CommandLineParserTest, Construct)
{
    CommandLineParser parser(_console);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckNotDefinedAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n  CommandLineParserTest [OPTION...]\n\n\n  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoArgumentNoValue)
{
    CommandLineParser parser(_console);
    parser.AddSwitch("add", 'a', "Add me");

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n  CommandLineParserTest [OPTION...]\n\n"
              "  -a, --add                 Add me\n\n"
              "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleSwitchNoVariable)
{
    CommandLineParser parser(_console);
    parser.AddSwitch("add", 'a', "Add me");

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add                 Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleSwitchWithVariable)
{
    CommandLineParser parser(_console);
    int variable {};
    int value = 1;
    parser.AddSwitchWithVariable("add", variable, value, "Add me");

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  --add                     Add me\n\n"
              "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableOptionalArgument)
{
    CommandLineParser parser(_console);
    string argument;
    parser.AddOptionOptionalArgument("add", 'a', "Add me", argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  -a, --add [argument]      Add me\n\n"
              "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableOptionalArgumentNoShortName)
{
    CommandLineParser parser(_console);
    string argument;
    parser.AddOptionOptionalArgument("add", "Add me", argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  --add [argument]          Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableRequiredArgument)
{
    CommandLineParser parser(_console);
    string argument;
    parser.AddOptionRequiredArgument("add", 'a', "Add me", argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add <argument>      Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoVariableRequiredArgumentNoShortName)
{
    CommandLineParser parser(_console);
    string argument;
    parser.AddOptionRequiredArgument("add", "Add me", argument);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  --add <argument>          Add me\n\n"
              "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableOptionalArgument)
{
    CommandLineParser parser(_console);
    int variable {};
    parser.AddOptionOptionalArgument<int>("add", 'a', "Add me", variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add [argument]      Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableOptionalArgumentNoShortName)
{
    CommandLineParser parser(_console);
    int variable {};
    parser.AddOptionOptionalArgument<int>("add", "Add me", variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  --add [argument]          Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableRequiredArgument)
{
    CommandLineParser parser(_console);
    int variable {};
    parser.AddOptionRequiredArgument<int>("add", 'a', "Add me", variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  -a, --add <argument>      Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionWithVariableRequiredArgumentNoShortName)
{
    CommandLineParser parser(_console);
    int variable {};
    parser.AddOptionRequiredArgument<int>("add", "Add me", variable);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
                    "  CommandLineParserTest [OPTION...]\n\n"
                    "  --add <argument>          Add me\n\n"
                    "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupMultipleOptions)
{
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    parser.Parse(0, nullptr);

    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
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
              "  --help                    Show help options\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, ParseNoArguments)
{
    int argc = 1;
    const char ** argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgumentValue)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--verbose";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(1, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundVerboseOnly(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--add";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddOnly(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseMultipleOptionsNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-ab";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddAppend(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgumentEqualsSign)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d=abc"; // Optional arguments require = sign
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("abc", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "abc");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgumentEqualsSign)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete=abc"; // Optional arguments require = sign
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("abc", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "abc");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgument)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d";
    argv[2]= "abc"; // Optional arguments require = sign
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "");
    EXPECT_EQ("abc", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgument)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete";
    argv[2] = "abc"; // Optional arguments require = sign
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "");
    EXPECT_EQ("abc", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_FALSE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_FALSE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgument)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f";
    argv[2] = "abc";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgument)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file";
    argv[2] = "abc";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgumentEqualsSign)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f=abc"; // Optional arguments require = sign
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgumentEqualsSign)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file=abc"; // Optional arguments require = sign
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseNonOption)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundOptions(parser);
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgumentValueExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--verbose";
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(1, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundVerboseOnly(parser);
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionNoArgumentExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--add";
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddOnly(parser);
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseMultipleOptionsNoArgumentExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-ab";
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundAddAppend(parser);
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalNoArgumentExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d";
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalNoArgumentExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete";
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgumentExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d=abc"; // Optional arguments require = sign
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("abc", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgumentExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete=abc"; // Optional arguments require = sign
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("abc", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundDeleteOnly(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgumentExtraNonOption)
{
    int argc = 4;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f";
    argv[2] = "abc";
    argv[3] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgumentExtraNonOption)
{
    int argc = 4;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file";
    argv[2] = "abc";
    argv[3] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredWithArgumentEqualsSignExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f=abc"; // Optional arguments require = sign
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongRequiredWithArgumentEqualsSignExtraNonOption)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file=abc"; // Optional arguments require = sign
    argv[2] = "test";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckFoundFileOnly(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseMoreComplexArguments)
{
    int argc = 8;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-a";
    argv[2] = "test";
    argv[3] = "-bf";
    argv[4] = "abc";
    argv[5] = "-dc=";
    argv[6] = "def";
    argv[7] = "--brief";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("c=", deleteString);
    EXPECT_EQ("", createString);
    EXPECT_EQ("abc", fileString);
    EXPECT_EQ(size_t{2}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundVerbose(parser);
    CheckFoundBrief(parser);
    CheckFoundAdd(parser);
    CheckFoundAppend(parser);
    CheckFoundDelete(parser, "c=");
    CheckNotFoundCreate(parser);
    CheckFoundFile(parser, "abc");
    EXPECT_EQ("test", parser.GetNonOption(0));
    EXPECT_EQ("def", parser.GetNonOption(1));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseQuotedString)
{
    int argc = 4;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-a";
    argv[2] = "\"a b\"";
    argv[3] = "-c=\"d e f\"";
    CommandLineParser parser(_console);
    int verboseFlag {};
    string deleteString {};
    string createString {};
    string fileString {};
    int x {};
    int y {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, verboseFlag);
    EXPECT_EQ("", deleteString);
    EXPECT_EQ("d e f", createString);
    EXPECT_EQ("", fileString);
    EXPECT_EQ(size_t{1}, parser.NumNonOptions());
    CheckDefinedOptions(parser);
    CheckNotFoundVerbose(parser);
    CheckNotFoundBrief(parser);
    CheckFoundAdd(parser);
    CheckNotFoundAppend(parser);
    CheckNotFoundDelete(parser);
    CheckFoundCreate(parser, "d e f");
    CheckNotFoundFile(parser);
    EXPECT_EQ("a b", parser.GetNonOption(0));

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseSwitchWithBoolVariableBrief)
{
    int argc = 2;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--brief";
    CommandLineParser parser(_console);
    bool verboseFlag = true;
    parser.AddSwitchWithVariable("verbose", verboseFlag, true, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, false, "Brief output");

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
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--verbose";
    CommandLineParser parser(_console);
    bool verboseFlag {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, true, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, false, "Brief output");

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
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--x=1234";
    argv[2] = "-y=5678";
    CommandLineParser parser(_console);
    int x {};
    int y {};
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(1234, x);
    EXPECT_EQ(5678, y);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedX(parser);
    CheckFoundX(parser, "1234");
    CheckDefinedY(parser);
    CheckFoundY(parser, "5678");

    delete [] argv;
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionWithIntVariable)
{
    int argc = 3;
    const char * * argv = new const char *[static_cast<size_t>(argc)];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--y";
    argv[2] = "5678";
    CommandLineParser parser(_console);
    int x {};
    int y {};
    parser.AddOptionOptionalArgument<int>("x", 'x', "X", x);
    parser.AddOptionRequiredArgument<int>("y", 'y', "Y", y);

    EXPECT_TRUE(parser.Parse(argc, argv));

    EXPECT_EQ(0, x);
    EXPECT_EQ(5678, y);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedX(parser);
    CheckNotFoundX(parser);
    CheckDefinedY(parser);
    CheckFoundY(parser, "5678");

    delete [] argv;
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core