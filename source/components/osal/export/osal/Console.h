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
inline void ConsoleBase<CharT>::SetForegroundColor(ConsoleColorType foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

template<class CharT>
inline void ConsoleBase<CharT>::SetBackgroundColor(ConsoleColorType backgroundColor)
{
    SetTerminalColor(_currentForegroundColor, backgroundColor);
}

template<class CharT>
inline void ConsoleBase<CharT>::ResetTerminalColor()
{
    SetTerminalColor();
}

template<class CharT>
inline ConsoleBase<CharT> & ConsoleBase<CharT>::operator<<(_SetForegroundColor color)
{
    SetForegroundColor(color.color);
    return *this;
}

template<class CharT>
inline ConsoleBase<CharT> & ConsoleBase<CharT>::operator<<(_SetBackgroundColor color)
{
    SetBackgroundColor(color.color);
    return *this;
}

typedef ConsoleBase<Char> Console;

} // namespace OSAL