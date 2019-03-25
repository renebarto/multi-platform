#include <unittest-cpp/UnitTestC++.h>
#include <unittest-cpp/ConsoleGoogleTestReporter.h>

#include <cstdarg>
#include <fstream>
#include <limits>
#include "osal/osal.h"
#include "osal/Console.h"
#include <core/Trace.h>
#include <unittest-cpp/ConsoleGoogleTestReporter.h>
#include "CommandLineOptionsParser.h"
#include "Message.h"

static const std::string moduleName = "json";

static OSAL::Console console;

#define GTEST_FLAG_PREFIX_ "gtest_"
#define GTEST_FLAG_PREFIX_DASH_ "gtest-"
#define GTEST_FLAG_PREFIX_UPPER_ "GTEST_"
#define GTEST_NAME_ "Google Test"
#define GTEST_PROJECT_URL_ "https://github.com/renebarto/unittest-cpp/"

#define GTEST_FLAG(name) FLAGS_gtest_##name

// Macros for defining flags.
#define GTEST_DEFINE_bool_(name, default_val, doc) \
    bool GTEST_FLAG(name) = (default_val)
#define GTEST_DEFINE_int32_(name, default_val, doc) \
    int32_t GTEST_FLAG(name) = (default_val)
#define GTEST_DEFINE_string_(name, default_val, doc) \
    ::std::string GTEST_FLAG(name) = (default_val)

GTEST_DEFINE_bool_(also_run_disabled_tests, false,
"Run disabled tests too, in addition to the tests normally being run.");

GTEST_DEFINE_bool_(break_on_failure, false,
"True iff a failed assertion should be a debugger break-point.");

GTEST_DEFINE_bool_(catch_exceptions, true,
"True iff " GTEST_NAME_ " should catch exceptions and treat them as test failures.");

GTEST_DEFINE_string_(color, "auto",
"Whether to use colors in the output.  Valid values: yes, no, "
"and auto.  'auto' means to use colors if the output is "
"being sent to a terminal and the TERM environment variable "
"is set to a terminal type that supports colors.");

GTEST_DEFINE_string_(filter, "*",
"A colon-separated list of glob (not regex) patterns "
"for filtering the tests to run, optionally followed by a "
"'-' and a : separated list of negative patterns (tests to "
"exclude).  A test is run if it matches one of the positive "
"patterns and does not match any of the negative patterns.");

GTEST_DEFINE_bool_(list_tests, false,
"List all tests without running them.");

GTEST_DEFINE_string_(output, "",
"A format (currently must be \"xml\"), optionally followed "
"by a colon and an output file name or directory. A directory "
"is indicated by a trailing pathname separator. "
"Examples: \"xml:filename.xml\", \"xml::directoryname/\". "
"If a directory is specified, output files will be created "
"within that directory, with file-names based on the test "
"executable's name and, if necessary, made unique by adding "
"digits.");

GTEST_DEFINE_bool_(print_time, true,
"True iff " GTEST_NAME_
" should display elapsed time in text output.");

GTEST_DEFINE_int32_(random_seed, 0,
"Random number seed to use when shuffling test orders.  Must be in range "
"[1, 99999], or 0 to use a seed based on the current time.");

GTEST_DEFINE_int32_(repeat, 1,
"How many times to repeat each test.  Specify a negative number "
"for repeating forever.  Useful for shaking out flaky tests.");

GTEST_DEFINE_bool_(show_internal_stack_frames, false,
"True iff " GTEST_NAME_ " should include internal stack frames when "
"printing test failure stack traces.");

GTEST_DEFINE_bool_(shuffle, false,
"True iff " GTEST_NAME_
" should randomize tests' order on every run.");

GTEST_DEFINE_string_(stream_result_to, "",
"This flag specifies the host name and the port number on which to stream "
"test results. Example: \"localhost:555\". The flag is effective only on "
"Linux.");

GTEST_DEFINE_bool_(throw_on_failure, false,
"When this flag is specified, a failed assertion will throw an exception "
"if exceptions are enabled or exit the program with a non-zero code "
"otherwise.");

/// Todo: Add Google Test parsing
namespace internal {


// GTestIsInitialized() returns true iff the user has initialized
// Google Test.  Useful for catching the user mistake of not initializing
// Google Test before calling RUN_ALL_TESTS().
std::vector<std::string> g_argvs;
// g_help_flag is true iff the --help flag or an equivalent form is
// specified on the command line.
bool g_help_flag;

static const char kColorEncodedHelpMessage[] =
    "This program contains tests written using UnitTestC++. You can use the\n"
    "following command line flags to control its behavior:\n"
    "\n"
    "Test Selection:\n"
    "  @G--" GTEST_FLAG_PREFIX_ "list_tests@D\n"
    "      List the names of all tests instead of running them. The name of\n"
    "      TEST(Foo, Bar) is \"Foo.Bar\".\n"
    "  @G--" GTEST_FLAG_PREFIX_ "filter=@YPOSTIVE_PATTERNS"
    "[@G-@YNEGATIVE_PATTERNS]@D\n"
    "      Run only the tests whose name matches one of the positive patterns but\n"
    "      none of the negative patterns. '?' matches any single character; '*'\n"
    "      matches any substring; ':' separates two patterns.\n"
    "  @G--" GTEST_FLAG_PREFIX_ "also_run_disabled_tests@D\n"
    "      Run all disabled tests too.\n"
    "\n"
    "Test Execution:\n"
    "  @G--" GTEST_FLAG_PREFIX_ "repeat=@Y[COUNT]@D\n"
    "      Run the tests repeatedly; use a negative count to repeat forever.\n"
    "  @G--" GTEST_FLAG_PREFIX_ "shuffle@D\n"
    "      Randomize tests' orders on every iteration.\n"
    "  @G--" GTEST_FLAG_PREFIX_ "random_seed=@Y[NUMBER]@D\n"
    "      Random number seed to use for shuffling test orders (between 1 and\n"
    "      99999, or 0 to use a seed based on the current time).\n"
    "\n"
    "Test Output:\n"
    "  @G--" GTEST_FLAG_PREFIX_ "color=@Y(@Gyes@Y|@Gno@Y|@Gauto@Y)@D\n"
    "      Enable/disable colored output. The default is @Gauto@D.\n"
    "  -@G-" GTEST_FLAG_PREFIX_ "print_time=0@D\n"
    "      Don't print the elapsed time of each test.\n"
    "  @G--" GTEST_FLAG_PREFIX_ "output=xml@Y[@G:@YDIRECTORY_PATH@G/@Y|@G:@YFILE_PATH]@D\n"
    "      Generate an XML report in the given directory or with the given file\n"
    "      name. @YFILE_PATH@D defaults to @Gtest_details.xml@D.\n"
    "\n"
    "Assertion Behavior:\n"
    "  @G--" GTEST_FLAG_PREFIX_ "break_on_failure@D\n"
    "      Turn assertion failures into debugger break-points.\n"
    "  @G--" GTEST_FLAG_PREFIX_ "throw_on_failure@D\n"
    "      Turn assertion failures into C++ exceptions.\n"
    "  @G--" GTEST_FLAG_PREFIX_ "catch_exceptions=0@D\n"
    "      Do not report exceptions as test failures. Instead, allow them\n"
    "      to crash the program or throw a pop-up (on Windows).\n"
    "\n"
    "Except for @G--" GTEST_FLAG_PREFIX_ "list_tests@D, you can alternatively set "
    "the corresponding\n"
    "environment variable of a flag (all letters in upper-case). For example, to\n"
    "disable colored text output, you can either specify @G--" GTEST_FLAG_PREFIX_ "color=no@D or set\n"
    "the @G" GTEST_FLAG_PREFIX_UPPER_ "COLOR@D environment variable to @Gno@D.\n"
    "\n"
    "For more information, please read the UnitTestC++ documentation at\n"
    "@G" GTEST_PROJECT_URL_ "@D. If you find a bug in UnitTestC++\n"
    "(not one in your own code or tests), please report it as an issue on GitHub.\n";

const std::vector<std::string>& GetArgvs() {
    return g_argvs;
}

// Helpers for printing colored strings to stdout. Note that on Windows, we
// cannot simply emit special characters and have the terminal change colors.
// This routine must actually emit the characters rather than return a string
// that would be colored when printed, as can be done on Linux.
void ColoredPrintf(OSAL::ConsoleColor color, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    console << fgcolor(color);

    char buffer[16384];
    vsprintf(buffer, fmt, args);
    console << buffer << fgcolor(OSAL::ConsoleColor::Default);
    va_end(args);
}

// Prints a string containing code-encoded text.  The following escape
// sequences can be used in the string to control the text color:
//
//   @@    prints a single '@' character.
//   @R    changes the color to red.
//   @G    changes the color to green.
//   @Y    changes the color to yellow.
//   @D    changes to the default terminal text color.
//
static void PrintColorEncoded(const char* str)
{
    OSAL::ConsoleColor color = OSAL::ConsoleColor::Default;  // The current color.

    // Conceptually, we split the string into segments divided by escape
    // sequences.  Then we print one segment at a time.  At the end of
    // each iteration, the str pointer advances to the beginning of the
    // next segment.
    for (;;)
    {
        const char * p = strchr(str, '@');
        if (p == nullptr)
        {
            ColoredPrintf(color, "%s", str);
            return;
        }

        ColoredPrintf(color, "%s", std::string(str, p).c_str());

        const char ch = p[1];
        str = p + 2;
        if (ch == '@')
        {
            ColoredPrintf(color, "@");
        } else if (ch == 'D')
        {
            color = OSAL::ConsoleColor::Default;
        } else if (ch == 'R')
        {
            color = OSAL::ConsoleColor::Red;
        } else if (ch == 'G')
        {
            color = OSAL::ConsoleColor::Green;
        } else if (ch == 'Y')
        {
            color = OSAL::ConsoleColor::Yellow;
        } else
        {
            --str;
        }
    }
}

template <typename T>
std::string StreamableToString(const T& streamable) {
    return (Message() << streamable).GetString();
}

static bool GTestIsInitialized() { return GetArgvs().size() > 0; }

// If *pstr starts with the given prefix, modifies *pstr to be right
// past the prefix and returns true; otherwise leaves *pstr unchanged
// and returns false.  None of pstr, *pstr, and prefix can be NULL.
bool SkipPrefix(const char* prefix, const char** pstr)
{
    const size_t prefix_len = strlen(prefix);
    if (strncmp(*pstr, prefix, prefix_len) == 0)
    {
        *pstr += prefix_len;
        return true;
    }
    return false;
}

// Determines whether a string has a prefix that Google Test uses for its
// flags, i.e., starts with GTEST_FLAG_PREFIX_ or GTEST_FLAG_PREFIX_DASH_.
// If Google Test detects that a command line flag has its prefix but is not
// recognized, it will print its help message. Flags starting with
// GTEST_INTERNAL_PREFIX_ followed by "internal_" are considered Google Test
// internal flags and do not trigger the help message.
static bool HasGoogleTestFlagPrefix(const char* str) {
    return (SkipPrefix("--", &str) ||
            SkipPrefix("-", &str) ||
            SkipPrefix("/", &str)) &&
           !SkipPrefix("" GTEST_FLAG_PREFIX_ "internal_", &str) &&
    (SkipPrefix(GTEST_FLAG_PREFIX_, &str) ||
     SkipPrefix(GTEST_FLAG_PREFIX_DASH_, &str));
}

// Parses a string as a command line flag.  The string should have
// the format "--flag=value".  When def_optional is true, the "=value"
// part can be omitted.
//
// Returns the value of the flag, or NULL if the parsing failed.
const char* ParseFlagValue(const char* str,
                           const char* flag,
                           bool def_optional) {
    // str and flag must not be NULL.
    if (str == NULL || flag == NULL) return NULL;

    // The flag must start with "--" followed by GTEST_FLAG_PREFIX_.
    const std::string flag_str = std::string("--") + "" GTEST_FLAG_PREFIX_ "" + flag;
    const size_t flag_len = flag_str.length();
    if (strncmp(str, flag_str.c_str(), flag_len) != 0) return NULL;

    // Skips the flag name.
    const char* flag_end = str + flag_len;

    // When def_optional is true, it's OK to not have a "=value" part.
    if (def_optional && (flag_end[0] == '\0')) {
        return flag_end;
    }

    // If def_optional is true and there are more characters after the
    // flag name, or if def_optional is false, there must be a '=' after
    // the flag name.
    if (flag_end[0] != '=') return NULL;

    // Returns the string after "=".
    return flag_end + 1;
}

// Parses a string for a bool flag, in the form of either
// "--flag=value" or "--flag".
//
// In the former case, the value is taken as true as long as it does
// not start with '0', 'f', or 'F'.
//
// In the latter case, the value is taken as true.
//
// On success, stores the value of the flag in *value, and returns
// true.  On failure, returns false without changing *value.
bool ParseBoolFlag(const char* str, const char* flag, bool* value) {
    // Gets the value of the flag as a string.
    const char* const value_str = ParseFlagValue(str, flag, true);

    // Aborts if the parsing failed.
    if (value_str == NULL) return false;

    // Converts the string value to a bool.
    *value = !(*value_str == '0' || *value_str == 'f' || *value_str == 'F');
    return true;
}

// Parses 'str' for a 32-bit signed integer.  If successful, writes
// the result to *value and returns true; otherwise leaves *value
// unchanged and returns false.
bool ParseInt32(const Message& src_text, const char* str, int32_t* value) {
    // Parses the environment variable as a decimal integer.
    char* end = NULL;
    const long long_value = strtol(str, &end, 10);  // NOLINT

    // Has strtol() consumed all characters in the string?
    if (*end != '\0') {
        // No - an invalid character was encountered.
        Message msg;
        msg << "WARNING: " << src_text
            << " is expected to be a 32-bit integer, but actually"
            << " has value \"" << str << "\".\n";
        printf("%s", msg.GetString().c_str());
        fflush(stdout);
        return false;
    }

    // Is the parsed value in the range of an int32_t?
    const int32_t result = static_cast<int32_t>(long_value);
    if (long_value == std::numeric_limits<long>::max() || long_value == std::numeric_limits<long>::min() ||
        // The parsed value overflows as a long.  (strtol() returns
        // LONG_MAX or LONG_MIN when the input overflows.)
        result != long_value
        // The parsed value overflows as an int32_t.
        ) {
        Message msg;
        msg << "WARNING: " << src_text
            << " is expected to be a 32-bit integer, but actually"
            << " has value " << str << ", which overflows.\n";
        printf("%s", msg.GetString().c_str());
        fflush(stdout);
        return false;
    }

    *value = result;
    return true;
}

// Parses a string for an int32_t flag, in the form of
// "--flag=value".
//
// On success, stores the value of the flag in *value, and returns
// true.  On failure, returns false without changing *value.
bool ParseInt32Flag(const char* str, const char* flag, int32_t* value) {
    // Gets the value of the flag as a string.
    const char* const value_str = ParseFlagValue(str, flag, false);

    // Aborts if the parsing failed.
    if (value_str == NULL) return false;

    // Sets *value to the value of the flag.
    return ParseInt32(Message() << "The value of flag --" << flag,
                      value_str, value);
}

// Parses a string for a string flag, in the form of
// "--flag=value".
//
// On success, stores the value of the flag in *value, and returns
// true.  On failure, returns false without changing *value.
bool ParseStringFlag(const char* str, const char* flag, std::string* value) {
    // Gets the value of the flag as a string.
    const char* const value_str = ParseFlagValue(str, flag, false);

    // Aborts if the parsing failed.
    if (value_str == NULL) return false;

    // Sets *value to the value of the flag.
    *value = value_str;
    return true;
}

// Names of the flags (needed for parsing Google Test flags).
static const char kAlsoRunDisabledTestsFlag[] = "also_run_disabled_tests";
static const char kBreakOnFailureFlag[] = "break_on_failure";
static const char kCatchExceptionsFlag[] = "catch_exceptions";
static const char kColorFlag[] = "color";
static const char kFilterFlag[] = "filter";
static const char kListTestsFlag[] = "list_tests";
static const char kOutputFlag[] = "output";
static const char kPrintTimeFlag[] = "print_time";
static const char kRandomSeedFlag[] = "random_seed";
static const char kRepeatFlag[] = "repeat";
static const char kShuffleFlag[] = "shuffle";
static const char kStackTraceDepthFlag[] = "stack_trace_depth";
static const char kStreamResultToFlag[] = "stream_result_to";
static const char kThrowOnFailureFlag[] = "throw_on_failure";
static const char kFlagfileFlag[] = "flagfile";
// A test whose test case name or test name matches this filter is
// disabled and not run.
static const char kDisableTestFilter[] = "DISABLED_*:*/DISABLED_*";
// A test filter that matches everything.
static const char kUniversalFilter[] = "*";
static const char kTypeParamLabel[] = "TypeParam";
static const char kValueParamLabel[] = "GetParam()";

bool ParseGoogleTestFlag(const char* const arg) {
    return ParseBoolFlag(arg, kAlsoRunDisabledTestsFlag, &GTEST_FLAG(also_run_disabled_tests)) ||
           ParseBoolFlag(arg, kBreakOnFailureFlag, &GTEST_FLAG(break_on_failure)) ||
           ParseBoolFlag(arg, kCatchExceptionsFlag, &GTEST_FLAG(catch_exceptions)) ||
           ParseStringFlag(arg, kColorFlag, &GTEST_FLAG(color)) ||
           ParseStringFlag(arg, kFilterFlag, &GTEST_FLAG(filter)) ||
           ParseBoolFlag(arg, kListTestsFlag, &GTEST_FLAG(list_tests)) ||
           ParseStringFlag(arg, kOutputFlag, &GTEST_FLAG(output)) ||
           ParseBoolFlag(arg, kPrintTimeFlag, &GTEST_FLAG(print_time)) ||
           ParseInt32Flag(arg, kRandomSeedFlag, &GTEST_FLAG(random_seed)) ||
           ParseInt32Flag(arg, kRepeatFlag, &GTEST_FLAG(repeat)) ||
           ParseBoolFlag(arg, kShuffleFlag, &GTEST_FLAG(shuffle)) ||
           ParseStringFlag(arg, kStreamResultToFlag, &GTEST_FLAG(stream_result_to)) ||
           ParseBoolFlag(arg, kThrowOnFailureFlag, &GTEST_FLAG(throw_on_failure));
}

// Parses the command line for Google Test flags, without initializing
// other parts of Google Test.  The type parameter CharType can be
// instantiated to either char or wchar_t.
template <typename CharType>
void ParseGoogleTestFlagsOnlyImpl(int* argc, CharType** argv)
{
    for (int i = 1; i < *argc; i++)
    {
        const std::string arg_string = StreamableToString(argv[i]);
        const char* const arg = arg_string.c_str();

        using internal::ParseBoolFlag;
        using internal::ParseInt32Flag;
        using internal::ParseStringFlag;

        bool remove_flag = false;
        if (ParseGoogleTestFlag(arg))
        {
            remove_flag = true;
        } else if (arg_string == "--help" || arg_string == "-h" ||
                   arg_string == "-?" || arg_string == "/?" ||
                   HasGoogleTestFlagPrefix(arg))
        {
            // Both help flag and unrecognized Google Test flags (excluding
            // internal ones) trigger help display.
            g_help_flag = true;
        }

        if (remove_flag)
        {
            // Shift the remainder of the argv list left by one.  Note
            // that argv has (*argc + 1) elements, the last one always being
            // NULL.  The following loop moves the trailing NULL element as
            // well.
            for (int j = i; j != *argc; j++)
            {
                argv[j] = argv[j + 1];
            }

            // Decrements the argument count.
            (*argc)--;

            // We also need to decrement the iterator as we just removed
            // an element.
            i--;
        }
    }

    if (g_help_flag) {
        // We print the help here instead of in RUN_ALL_TESTS(), as the
        // latter may not be called at all if the user is using Google
        // Test with another testing framework.
        PrintColorEncoded(kColorEncodedHelpMessage);
    }
}

// Parses the command line for Google Test flags, without initializing
// other parts of Google Test.
void ParseGoogleTestFlagsOnly(int* argc, char** argv)
{
    ParseGoogleTestFlagsOnlyImpl(argc, argv);
}
void ParseGoogleTestFlagsOnly(int* argc, wchar_t** argv)
{
    ParseGoogleTestFlagsOnlyImpl(argc, argv);
}

template <typename CharType>
void InitGoogleTestImpl(int* argc, CharType** argv) {
    // We don't want to run the initialization code twice.
    if (GTestIsInitialized()) return;

    if (*argc <= 0) return;

    g_argvs.clear();
    for (int i = 0; i != *argc; i++) {
        g_argvs.push_back(StreamableToString(argv[i]));
    }

    ParseGoogleTestFlagsOnly(argc, argv);
}

}  // namespace internal

// Initializes Google Test.  This must be called before calling
// RUN_ALL_TESTS().  In particular, it parses a command line for the
// flags that Google Test recognizes.  Whenever a Google Test flag is
// seen, it is removed from argv, and *argc is decremented.
//
// No value is returned.  Instead, the Google Test flag variables are
// updated.
//
// Calling the function for the second time has no user-visible effect.
void InitGoogleTest(int* argc, char** argv)
{
    internal::InitGoogleTestImpl(argc, argv);
}

// This overloaded version can be used in Windows programs compiled in
// UNICODE mode.
void InitGoogleTest(int* argc, wchar_t** argv)
{
    internal::InitGoogleTestImpl(argc, argv);
}

namespace internal {
// Parses the environment variable var as an int32_t. If it is unset,
// returns default_val. If it is not an int32_t, prints an error
// and aborts.
int32_t Int32FromEnvOrDie(const char * var, int32_t default_val)
{
    const char * str_val = OSAL::System::GetEnvironment(var);
    if (str_val == NULL)
    {
        return default_val;
    }

    int32_t result;
    if (!ParseInt32(Message() << "The value of environment variable " << var,
                    str_val, &result))
    {
        exit(EXIT_FAILURE);
    }
    return result;
}

// Returns true iff the wildcard pattern matches the string.  The
// first ':' or '\0' character in pattern marks the end of it.
//
// This recursive algorithm isn't very efficient, but is clear and
// works well enough for matching test names, which are short.
bool PatternMatchesString(const char *pattern, const char *str)
{
    switch (*pattern)
    {
        case '\0':
        case ':':  // Either ':' or '\0' marks the end of the pattern.
            return *str == '\0';
        case '?':  // Matches any single character.
            return *str != '\0' && PatternMatchesString(pattern + 1, str + 1);
        case '*':  // Matches any string (possibly empty) of characters.
            return (*str != '\0' && PatternMatchesString(pattern, str + 1)) ||
                   PatternMatchesString(pattern + 1, str);
        default:  // Non-special character.  Matches itself.
            return *pattern == *str &&
                   PatternMatchesString(pattern + 1, str + 1);
    }
}

// Returns true if "name" matches the ':' separated list of glob-style
// filters in "filter".
static bool MatchesFilter(const std::string& name, const char* filter)
{
    const char *cur_pattern = filter;
    for (;;)
    {
        if (PatternMatchesString(cur_pattern, name.c_str()))
        {
            return true;
        }

        // Finds the next pattern in the filter.
        cur_pattern = strchr(cur_pattern, ':');

        // Returns if no more pattern can be found.
        if (cur_pattern == nullptr)
        {
            return false;
        }

        // Skips the pattern separater (the ':' character).
        cur_pattern++;
    }
}

// Returns true iff the user-specified filter matches the test case
// name and the test name.
static bool FilterMatchesTest(const std::string &test_case_name,
                              const std::string &test_name)
{
    const std::string & fullTestName = test_case_name + "." + test_name.c_str();

    // Split --gtest_filter at '-', if there is one, to separate into
    // positive filter and negative filter portions
    const char* const p = GTEST_FLAG(filter).c_str();
    const char* const dash = strchr(p, '-');
    std::string positive;
    std::string negative;
    if (dash == nullptr)
    {
        positive = GTEST_FLAG(filter).c_str();  // Whole string is a positive filter
        negative = "";
    } else
    {
        positive = std::string(p, dash);   // Everything up to the dash
        negative = std::string(dash + 1);  // Everything after the dash
        if (positive.empty())
        {
            // Treat '-test1' as the same as '*-test1'
            positive = kUniversalFilter;
        }
    }

    // A filter is a colon-separated list of patterns.  It matches a
    // test if any pattern in it matches the test.
    return (MatchesFilter(fullTestName, positive.c_str()) &&
            !MatchesFilter(fullTestName, negative.c_str()));
}

}  // namespace internal

struct TestInfo
{
    explicit TestInfo(UnitTestCpp::Test * testCase)
        : test(testCase)
        , name(test->_details.testName)
        , isDisabled(false)
        , matchesFilter(false)
        , shouldRun(false)
    {}
    UnitTestCpp::Test * test;
    std::string name;
    bool isDisabled;
    bool matchesFilter;
    bool shouldRun;
};
struct TestFixtureInfo
{
    explicit TestFixtureInfo(UnitTestCpp::TestFixtureInfo * testFixture)
        : fixture(testFixture)
        , name(fixture->Name())
        , shouldRun(false)
        , tests()
    {}
    UnitTestCpp::TestFixtureInfo * fixture;
    std::string name;
    bool shouldRun;
    std::vector<TestInfo> tests;
};
struct TestSuiteInfo
{
    explicit TestSuiteInfo(UnitTestCpp::TestSuiteInfo * testSuite)
        : suite(testSuite)
        , name(suite->Name())
        , shouldRun(false)
        , testFixtures()
    {}
    UnitTestCpp::TestSuiteInfo * suite;
    std::string name;
    bool shouldRun;
    std::vector<TestFixtureInfo> testFixtures;
};
struct TestRunInfo
{
    std::vector<TestSuiteInfo> testSuites;
};

// Compares the name of each test with the user-specified filter to
// decide whether the test should be run, then records the result in
// each TestCase and TestInfo object.
// Returns the number of tests that should run.
int FilterTests(TestRunInfo & runInfo)
{
    auto & registry = UnitTestCpp::Test::GetTestRegistry();

    // numRunnableTests are the number of tests that will
    // run
    int numRunnableTests = 0;

    runInfo.testSuites.clear();

    UnitTestCpp::TestSuiteInfo * curTestSuite = registry.GetHead();
    while (curTestSuite)
    {
        TestSuiteInfo suiteInfo(curTestSuite);

        UnitTestCpp::TestFixtureInfo * testFixture = curTestSuite->GetHead();
        while (testFixture)
        {
            TestFixtureInfo fixtureInfo(testFixture);

            UnitTestCpp::Test * test = testFixture->GetHead();
            while (test)
            {
                TestInfo testInfo(test);

                // A test is disabled if test case name or test name matches
                // kDisableTestFilter.
                const bool isDisabled =
                    internal::MatchesFilter(fixtureInfo.name, internal::kDisableTestFilter) ||
                        internal::MatchesFilter(testInfo.name, internal::kDisableTestFilter);
                testInfo.isDisabled = isDisabled;

                const bool matchesFilter = internal::FilterMatchesTest(fixtureInfo.name, testInfo.name);
                testInfo.matchesFilter = matchesFilter;

                const bool isRunnable = (GTEST_FLAG(also_run_disabled_tests) || !isDisabled) && matchesFilter;

                numRunnableTests += isRunnable;

                testInfo.shouldRun = isRunnable;
                fixtureInfo.shouldRun = fixtureInfo.shouldRun || isRunnable;
                suiteInfo.shouldRun = suiteInfo.shouldRun || isRunnable;

                fixtureInfo.tests.push_back(testInfo);
                test = test->_next;
            }
            suiteInfo.testFixtures.push_back(fixtureInfo);
            testFixture = testFixture->next;
        }
        runInfo.testSuites.push_back(suiteInfo);
        curTestSuite = curTestSuite->next;
    }

    return numRunnableTests;
}

// Prints the names of the tests matching the user-specified filter flag.
void ListTestsMatchingFilter(TestRunInfo & runInfo)
{
    for (auto const & suite : runInfo.testSuites)
    {
        for (auto const & fixture : suite.testFixtures)
        {
            bool printedTestFixtureName = false;
            for (auto const & test : fixture.tests)
            {
                if (test.matchesFilter)
                {
                    if (!printedTestFixtureName)
                    {
                        printedTestFixtureName = true;
                        cout << fixture.name << "." << endl;
                    }
                    cout << "  " << test.name << endl;
                }
            }
        }
    }

    cout << flush;
}

struct InSelection
{
    InSelection()
        : _runInfo()
    {
        auto & registry = UnitTestCpp::Test::GetTestRegistry();

        _runInfo.testSuites.clear();

        UnitTestCpp::TestSuiteInfo * curTestSuite = registry.GetHead();
        while (curTestSuite)
        {
            TestSuiteInfo suiteInfo(curTestSuite);

            UnitTestCpp::TestFixtureInfo * testFixture = curTestSuite->GetHead();
            while (testFixture)
            {
                TestFixtureInfo fixtureInfo(testFixture);

                UnitTestCpp::Test * test = testFixture->GetHead();
                while (test)
                {
                    TestInfo testInfo(test);

                    // A test is disabled if test case name or test name matches
                    // kDisableTestFilter.
                    const bool isDisabled =
                        internal::MatchesFilter(fixtureInfo.name, internal::kDisableTestFilter) ||
                            internal::MatchesFilter(testInfo.name, internal::kDisableTestFilter);
                    testInfo.isDisabled = isDisabled;

                    const bool matchesFilter = internal::FilterMatchesTest(fixtureInfo.name, testInfo.name);
                    testInfo.matchesFilter = matchesFilter;

                    const bool isRunnable = (GTEST_FLAG(also_run_disabled_tests) || !isDisabled) && matchesFilter;

                    testInfo.shouldRun = isRunnable;
                    fixtureInfo.shouldRun = fixtureInfo.shouldRun || isRunnable;
                    suiteInfo.shouldRun = suiteInfo.shouldRun || isRunnable;

                    fixtureInfo.tests.push_back(testInfo);
                    test = test->_next;
                }
                suiteInfo.testFixtures.push_back(fixtureInfo);
                testFixture = testFixture->next;
            }
            _runInfo.testSuites.push_back(suiteInfo);
            curTestSuite = curTestSuite->next;
        }
    }
    bool operator () (const UnitTestCpp::TestInfo * const test) const
    {
        for (auto const & suite : _runInfo.testSuites)
        {
            if (suite.shouldRun)
            {
                for (auto const & fixture : suite.testFixtures)
                {
                    if (suite.shouldRun)
                    {
                        for (auto const & item : fixture.tests)
                        {
                            if (item.name == test->_details.testName)
                                return item.shouldRun;
                        }
                    }
                }
            }
        }
        return false;
    }
    bool operator () (const UnitTestCpp::TestFixtureInfo * const fixture) const
    {
        for (auto const & suite : _runInfo.testSuites)
        {
            if (suite.shouldRun)
            {
                for (auto const & item : suite.testFixtures)
                {
                    if (item.fixture == fixture)
                        return item.shouldRun;
                }
            }
        }
        return false;
    }
    bool operator () (const UnitTestCpp::TestSuiteInfo * const suite) const
    {
        for (auto const & item : _runInfo.testSuites)
        {
            if (item.suite == suite)
                return item.shouldRun;
        }
        return false;
    }

private:
    TestRunInfo _runInfo;
};

int main(int argc, char * argv[])
{
//    console << fgcolor(OSAL::ConsoleColor::Magenta | OSAL::ConsoleColor::Intensity);
//    console << "Running tests for: " << moduleName << std::endl;
//    console << fgcolor(OSAL::ConsoleColor::Default);
    Core::InitializeTrace(console.GetStream());

    InitGoogleTest(&argc, argv);
    CommandLineOptionsParser parser(console);
    std::string applicationName = argv[0];

    InSelection selection;

    // Lists the tests and exits if the --gtest_list_tests flag was specified.
    if (GTEST_FLAG(list_tests))
    {
        UnitTestCpp::ConsoleGoogleTestReporter reporter;
        ListSelectedTests(reporter, selection);
//        UnitTestCpp::ListSelectedTests(selection);
        return 0;
    }


    if (!parser.Parse(argc, argv))
    {
        exit(1);
    }

    console << "Application : " << applicationName << std::endl;
    console << "XML output  : " << parser.xmlOutput << std::endl;
    console << "Suite       : " << parser.testSuiteName << std::endl;
    console << "Fixture     : " << parser.testFixtureName << std::endl;
    console << "Test        : " << parser.testName << std::endl;
    console << "GTest filter: " << parser.testFilter << std::endl;
    console << "GTest color : " << parser.testColor << std::endl;
    console << "Test data   : " << TEST_DATA_ROOT << std::endl;

    int result = 0;

    if (!parser.xmlOutput.empty())
    {
        std::basic_ofstream<char> outputFile;

        outputFile.open(parser.xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        return RunSelectedTests(reporter, selection);
    }
    else
    {
        UnitTestCpp::ConsoleGoogleTestReporter reporter;

        return UnitTestCpp::RunSelectedTests(reporter, selection);
    }

    return result;
}
