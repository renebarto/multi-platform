#include <unittest-cpp/UnitTestC++.h>

#include "osal/Console.h"
#include "osal/System.h"

using namespace std;

namespace OSAL {
namespace Test {

class ConsoleTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(osal)
{

TEST_FIXTURE(ConsoleTest, Constructor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    EXPECT_FALSE(console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
}

TEST_FIXTURE(ConsoleTest, ConstructorStdOut)
{
    const char * termVar = OSAL::System::GetEnvironment("TERM");
    string term = termVar ? termVar : "";
    bool terminalSupportsColor = (term == "xterm") || (term == "xterm-color") ||
                                 (term == "xterm-256color") || (term == "screen") || 
                                 (term == "screen-256color") || (term == "linux") || (term == "cygwin");
    OSAL::Console console(cout);
    auto handle = OSAL::Files::GetFileDescriptor(stdout);
    terminalSupportsColor &= OSAL::Files::IsTTY(handle);
    EXPECT_EQ(terminalSupportsColor, console.ShouldUseColor());
    EXPECT_FALSE(console.ForceUseColor());
}

TEST_FIXTURE(ConsoleTest, ForceUseColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    EXPECT_TRUE(console.ShouldUseColor());
    EXPECT_TRUE(console.ForceUseColor());
}

TEST_FIXTURE(ConsoleTest, OutputNoColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console << "no color\n"
            << fgcolor(ConsoleColor::Red) << "red\n"
            << fgcolor(ConsoleColor::Green | ConsoleColor::Intensity)
            << bgcolor(ConsoleColor::White | ConsoleColor::Intensity) << "green on black\n"
            << fgcolor(ConsoleColor::Default) << bgcolor(ConsoleColor::Default) << "no color\n";
    string expected("no color\nred\ngreen on black\nno color\n");
    EXPECT_EQ(expected, stream.str());
}

#if defined(WIN_MSVC) || defined(WIN_MINGW)

TEST_FIXTURE(ConsoleTest, OutputColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    console << "no color\n"
            << fgcolor(ConsoleColor::Red) << "red\n"
            << fgcolor(ConsoleColor::Green | ConsoleColor::Intensity)
            << bgcolor(ConsoleColor::White | ConsoleColor::Intensity) << "green on black\n"
            << fgcolor(ConsoleColor::Default) << bgcolor(ConsoleColor::Default) << "no color\n";
    string expected("no color\n\033[0;31;1mred\n\033[0;92;1m\033[0;92;1;107;1mgreen on black\n\033[0;107;1m\033[0mno color\n");
    EXPECT_EQ(expected, stream.str());
}

#elif defined(LINUX) || defined(DARWIN)

TEST_FIXTURE(ConsoleTest, OutputColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    console << "no color\n"
        << fgcolor(ConsoleColor::Red) << "red\n"
        << fgcolor(ConsoleColor::Green | ConsoleColor::Intensity)
        << bgcolor(ConsoleColor::White | ConsoleColor::Intensity) << "green on black\n"
        << fgcolor(ConsoleColor::Default) << bgcolor(ConsoleColor::Default) << "no color\n";
    string expected("no color\n\033[0;31mred\n\033[0;92m\033[0;92;107mgreen on black\n\033[0;107m\033[0mno color\n");
    EXPECT_EQ(expected, stream.str());
}

#endif

TEST_FIXTURE(ConsoleTest, SetForegroundColorNoColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.SetForegroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetForegroundColor(ConsoleColor::Default);
    console << "text";
    string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(ConsoleTest, SetForegroundColorWithColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    console.SetForegroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetForegroundColor(ConsoleColor::Default);
    console << "text";
    string expected("\033[0;34;1mtext\033[0mtext");
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(ConsoleTest, SetBackgroundColorNoColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.SetBackgroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetBackgroundColor(ConsoleColor::Default);
    console << "text";
    string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(ConsoleTest, SetBackgroundColorWithColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    console.SetBackgroundColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.SetBackgroundColor(ConsoleColor::Default);
    console << "text";
    string expected("\033[0;44;1mtext\033[0mtext");
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(ConsoleTest, SetResetTerminalColorNoColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.SetTerminalColor(ConsoleColor::Blue | ConsoleColor::Bold);
    console << "text";
    console.ResetTerminalColor();
    console << "text";
    string expected("texttext");
    EXPECT_EQ(expected, stream.str());
}

#if defined(WIN_MSVC) || defined(WIN_MINGW)

TEST_FIXTURE(ConsoleTest, SetResetTerminalColorWithColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    console.SetTerminalColor(ConsoleColor::Blue | ConsoleColor::Bold, ConsoleColor::Red | ConsoleColor::Intensity);
    console << "text";
    console.ResetTerminalColor();
    console << "text";
    string expected("\033[0;34;1;101;1mtext\033[0mtext");
    EXPECT_EQ(expected, stream.str());
}

#elif defined(LINUX) || defined(DARWIN)

TEST_FIXTURE(ConsoleTest, SetResetTerminalColorWithColor)
{
    ostringstream stream;
    OSAL::Console console(stream);
    console.ForceUseColor(true);
    console.SetTerminalColor(ConsoleColor::Blue | ConsoleColor::Bold, ConsoleColor::Red | ConsoleColor::Intensity);
    console << "text";
    console.ResetTerminalColor();
    console << "text";
    string expected("\033[0;34;1;101mtext\033[0mtext");
    EXPECT_EQ(expected, stream.str());
}

#endif

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
