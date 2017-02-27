#include <unit-test-c++/UnitTestC++.h>

#include "core/CommandLineParser.h"

namespace Core {
namespace Test {

class CommandLineParserTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
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
    void CheckFoundDelete(const CommandLineParser &parser, const OSAL::String & value) const
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
    void CheckFoundCreate(const CommandLineParser &parser, const OSAL::String & value) const
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
    void CheckFoundFile(const CommandLineParser &parser, const OSAL::String & value) const
    {
        CheckFoundFile(parser);
        EXPECT_EQ(value, parser.GetOption('f'));
        EXPECT_EQ(value, parser.GetOption("file"));
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
        EXPECT_EQ(value, parser.GetOption('d'));
        EXPECT_EQ(value, parser.GetOption("delete"));
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
        EXPECT_EQ(value, parser.GetOption('f'));
        EXPECT_EQ(value, parser.GetOption("file"));
    }
};

void CommandLineParserTest::SetUp()
{
}

void CommandLineParserTest::TearDown()
{
}

static const OSAL::String ApplicationName = "CommandLineParserTest";

TEST_FIXTURE(CommandLineParserTest, Construct)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckNotDefinedAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n  CommandLineParserTest [OPTION...]\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoArgumentNoValue)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    parser.AddSwitch("add", 'a', "Add me");
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n  CommandLineParserTest [OPTION...]\n\n  -a, --add                 Add me\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionNoArgumentWithValue)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int variable;
    parser.AddSwitchWithVariable("add", variable, 1, "Add me");
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAddOnly(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  --add                     Add me\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionOptionalArgument)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    OSAL::String argument;
    parser.AddOptionOptionalArgument("add", 'a', "Add me", argument);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  -a, --add [argument]      Add me\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupSingleOptionRequiredArgument)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    OSAL::String argument;
    parser.AddOptionRequiredArgument("add", 'a', "Add me", argument);
    EXPECT_EQ(size_t{0}, parser.NumNonOptions());
    CheckDefinedAdd(parser);
    CheckNotFoundAdd(parser);
    EXPECT_TRUE(parser.AutoHandleHelp());
    EXPECT_FALSE(parser.ShouldShowHelp());
    EXPECT_EQ("Usage:\nMain: \n\n"
              "  CommandLineParserTest [OPTION...]\n\n"
              "  -a, --add <argument>      Add me\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, SetupMultipleOptions)
{
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);
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
                  "  -f, --file <argument>     File\n\n", parser.GetHelp(ApplicationName));
}

TEST_FIXTURE(CommandLineParserTest, ParseNoArguments)
{
    int argc = 1;
    const char ** argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--verbose";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--add";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-ab";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionOptionalWithArgument)
{
    int argc = 2;
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d=abc"; // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionLongOptionalWithArgument)
{
    int argc = 2;
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete=abc"; // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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

TEST_FIXTURE(CommandLineParserTest, ParseSingleOptionRequiredNoArgument)
{
    int argc = 2;
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f";
    argv[2] = "abc";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file";
    argv[2] = "abc";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f=abc"; // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file=abc"; // Optional arguments require = sign
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--verbose";
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--add";
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-ab";
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d";
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete";
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-d=abc"; // Optional arguments require = sign
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--delete=abc"; // Optional arguments require = sign
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f";
    argv[2] = "abc";
    argv[3] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file";
    argv[2] = "abc";
    argv[3] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-f=abc"; // Optional arguments require = sign
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--file=abc"; // Optional arguments require = sign
    argv[2] = "test";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-a";
    argv[2] = "test";
    argv[3] = "-bf";
    argv[4] = "abc";
    argv[5] = "-dc=";
    argv[6] = "def";
    argv[7] = "--brief";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "-a";
    argv[2] = "\"a b\"";
    argv[3] = "-c=\"d e f\"";
    OSAL::Console console;
    CommandLineParser parser(console);
    int verboseFlag {};
    OSAL::String deleteString {};
    OSAL::String createString {};
    OSAL::String fileString {};
    parser.AddSwitchWithVariable("verbose", verboseFlag, 1, "Verbose output");
    parser.AddSwitchWithVariable("brief", verboseFlag, 0, "Brief output");
    parser.AddSwitch("add", 'a', "Add");
    parser.AddSwitch("append", 'b', "Append");
    parser.AddOptionOptionalArgument("delete", 'd', "Delete", deleteString);
    parser.AddOptionOptionalArgument("create", 'c', "Create", createString);
    parser.AddOptionRequiredArgument("file", 'f', "File", fileString);

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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--brief";
    OSAL::Console console;
    CommandLineParser parser(console);
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
    const char * * argv = new const char *[argc];
    argv[0] = ApplicationName.c_str();
    argv[1] = "--verbose";
    OSAL::Console console;
    CommandLineParser parser(console);
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

} // namespace Test
} // namespace Core