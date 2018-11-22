#include "osal/PlatformDefines.h"

#if defined(WIN_MSVC)

#include "osal/Console.h"
#include "osal/System.h"

namespace OSAL
{

static std::ostream * DetermineStream(Files::FileDescriptor handle)
{
    switch (handle)
    {
    case STDOUT_FILENO:
        return &std::cout;
    case STDERR_FILENO:
        return &std::cerr;
    case STDIN_FILENO:
    default:
        std::cerr << "Invalid handle specified, please specify only stdout or stderr handle"
                  << std::endl;
    }
    return nullptr;
}

static Files::FileDescriptor DetermineHandle(std::ostream * stream)
{
    if (stream == &std::cout)
        return OSAL::Files::GetFileDescriptor(stdout);
    else if (stream == &std::cerr)
        return OSAL::Files::GetFileDescriptor(stderr);
    return -1;
}

static const char * GetAnsiColorCode(ConsoleColor color)
{
    switch (color & ConsoleColor::ColorMask)
    {
    case ConsoleColor::Black:
        return "0";
    case ConsoleColor::Red:
        return "1";
    case ConsoleColor::Green:
        return "2";
    case ConsoleColor::Yellow:
        return "3";
    case ConsoleColor::Blue:
        return "4";
    case ConsoleColor::Magenta:
        return "5";
    case ConsoleColor::Cyan:
        return "6";
    case ConsoleColor::White:
        return "7";
    default:
        return nullptr;
    };
}

Console::Console(int handle)
    : _handle(handle)
    , _stream(DetermineStream(_handle))
    , _forceUseColor()
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

Console::Console(FILE * file)
    : _handle(OSAL::Files::GetFileDescriptor(file))
    , _stream(DetermineStream(_handle))
    , _forceUseColor()
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

Console::Console(std::ostream & stream)
    : _handle(DetermineHandle(&stream))
    , _stream(&stream)
    , _forceUseColor()
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    std::string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
        if ((backgroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
    }
    command += "m";
    if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

bool Console::ShouldUseColor() const
{
    if (ForceUseColor())
        return true;
    if (_handle == InvalidHandle)
        return false;
    if (!OSAL::Files::IsTTY(_handle))
        return false;

    const char * termSetting = OSAL::System::GetEnvironment("TERM");
    if (!termSetting)
        return false;
    std::string term               = termSetting;
    const bool term_supports_color = (term == "xterm") || (term == "xterm-color")
        || (term == "xterm-256color") || (term == "screen") || (term == "screen-256color")
        || (term == "linux") || (term == "cygwin");
    return term_supports_color;
}

bool Console::ForceUseColor() const
{
    return _forceUseColor;
}

void Console::ForceUseColor(bool value)
{
    _forceUseColor = value;
}

} // namespace OSAL

OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColor color)
{
    return {color};
}

OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColor color)
{
    return {color};
}

#endif // defined(WIN_MSVC)
