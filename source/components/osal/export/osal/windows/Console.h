#pragma once

//#include <unistd.h>
#include "osal/ConsoleBase.h"

namespace OSAL {

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

inline std::basic_ostream<Char> * DetermineStream(int handle)
{
#if defined(UNICODE) || defined(_UNICODE)
    switch (handle)
    {
        case STDOUT_FILENO:
            return &std::wcout;
            break;
        case STDERR_FILENO:
            return &std::wcerr;
            break;
        case STDIN_FILENO:
        default:
            std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
#else
	switch (handle)
	{
	case STDOUT_FILENO:
		return &std::cout;
		break;
	case STDERR_FILENO:
		return &std::cerr;
		break;
	case STDIN_FILENO:
	default:
		std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
	}
#endif
	return nullptr;
}

inline int DetermineHandle(std::ostream * stream)
{
    if (stream == &std::cout)
        return OSAL::Files::fileno(stdout);
    else if (stream == &std::cerr)
        return OSAL::Files::fileno(stderr);
    return -1;
}

inline const wchar_t * GetAnsiColorCode(ConsoleColorType color)
{
	switch (color & ConsoleColor::ColorMask)
	{
	case ConsoleColor::Black:
		return _("0");
	case ConsoleColor::Red:
		return _("1");
	case ConsoleColor::Green:
		return _("2");
	case ConsoleColor::Yellow:
		return _("3");
	case ConsoleColor::Blue:
		return _("4");
	case ConsoleColor::Magenta:
		return _("5");
	case ConsoleColor::Cyan:
		return _("6");
	case ConsoleColor::White:
		return _("7");
	case ConsoleColor::Default:
	case ConsoleColor::Intensity:
	default:
		return nullptr;
	};
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(std::basic_ostream<CharT> & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
void ConsoleBase<CharT>::SetTerminalColor(ConsoleColorType foregroundColor, ConsoleColorType backgroundColor)
{
    if (!ShouldUseColor())
        return;
#if defined(UNICODE) || defined(_UNICODE)
	std::wstring command = L"\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += L";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += L";9";
        else
            command += L";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += L";10";
        else
            command += L";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += L"m";
#else
	std::string command = "\033[0";
	if (foregroundColor != ConsoleColor::Default)
	{
		if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
		{
			command += ";1";
		}
		if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
			command += ";9";
		else
			command += ";3";
		command += GetAnsiColorCode(foregroundColor);
	}
	if (backgroundColor != ConsoleColor::Default)
	{
		if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
			command += ";10";
		else
			command += ";4";
		command += GetAnsiColorCode(backgroundColor);
	}
	command += "m";
#endif
	if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

template<class CharT>
bool ConsoleBase<CharT>::ShouldUseColor()
{
    if (_handle == InvalidHandle)
        return false;
    if (!OSAL::Files::isatty(_handle))
        return false;

    const char * termSetting = OSAL::System::getenv("TERM");
    if (!termSetting)
        return false;
    std::string term = termSetting;
    const bool term_supports_color =
    (term == "xterm") ||
    (term == "xterm-color") ||
    (term == "xterm-256color") ||
    (term == "screen") ||
    (term == "screen-256color") ||
    (term == "linux") ||
    (term == "cygwin");
    return term_supports_color;
}

} // namespace OSAL
