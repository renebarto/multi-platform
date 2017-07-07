#pragma once

#include "osal/ConsoleBase.h"
#if defined(WIN_MSVC)
#include "osal/windows/Console.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Console.h"
#elif defined(DARWIN)
#include "osal/darwin/Console.h"
#elif defined(LINUX)
#include "osal/linux/Console.h"
#endif

namespace OSAL {

template<class CharT>
void ConsoleBase<CharT>::SetForegroundColor(ConsoleColorType foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

template<class CharT>
void ConsoleBase<CharT>::SetBackgroundColor(ConsoleColorType backgroundColor)
{
    SetTerminalColor(_currentForegroundColor, backgroundColor);
}

template<class CharT>
void ConsoleBase<CharT>::ResetTerminalColor()
{
    SetTerminalColor();
}

template<class CharT>
ConsoleBase<CharT> &operator<<(ConsoleBase<CharT> &stream, _SetForegroundColor color)
{
    stream.SetForegroundColor(color.color);
    return stream;
}

template<class CharT>
ConsoleBase<CharT> &operator<<(ConsoleBase<CharT> &stream, _SetBackgroundColor color)
{
    stream.SetBackgroundColor(color.color);
    return stream;
}

typedef ConsoleBase<Char> Console;

} // namespace OSAL