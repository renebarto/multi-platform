#pragma once

#include <iostream>
#include "osal/OSAL.h"
#include "osal/FlagOperators.h"

namespace OSAL
{

class Secret;

} // namespace OSAL

// Ensures that there is at least one operator<< in the global namespace.
// See Message& operator<<(...) below for why.
void OSAL_EXPORT operator <<(const OSAL::Secret&, int);

namespace OSAL
{

#if defined(LINUX) || defined(DARWIN)
enum class ConsoleColor : int
{
    Default = -1,
    Black = 0,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    ColorMask = 0x07,
    Intensity = 0x08,
    Bold = 0x10,
};
#elif defined(WIN_MSVC) || defined(WIN_MINGW)
enum class ConsoleColor : int
{
    Default = -1,
    Black = 0,
    Red = FOREGROUND_RED,
    Green = FOREGROUND_GREEN,
    Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
    Blue = FOREGROUND_BLUE,
    Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
    Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
    White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    ColorMask = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    Intensity = FOREGROUND_INTENSITY,
    Bold = 0,
};
#endif

DEFINE_FLAG_OPERATORS(ConsoleColor, int);

template<class CharT>
OSAL_EXPORT std::basic_ostream<CharT> & operator << (std::basic_ostream<CharT> & stream, ConsoleColor value);

struct OSAL_EXPORT _SetForegroundColor
{
    ConsoleColor color;
};

struct OSAL_EXPORT _SetBackgroundColor
{
    ConsoleColor color;
};

template<class CharT>
class ConsoleBase
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::basic_ostream<CharT> & (* BasicIoManip)(std::basic_ostream<CharT> &);

public:
    static const int InvalidHandle = -1;

    ConsoleBase(int handle = OSAL::Files::fileno(stdout));
    ConsoleBase(std::basic_ostream<CharT> & stream);

    void SetForegroundColor(ConsoleColor foregroundColor);
    void SetBackgroundColor(ConsoleColor backgroundColor);
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default,
                          ConsoleColor backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor();
    bool ShouldUseColor();

    // Streams a non-pointer _value to this object.
    template <typename T>
    inline ConsoleBase & operator << (const T & val)
    {
        using ::operator <<;
        if (_stream)
            *_stream << val;
        return *this;
    }

    ConsoleBase & operator << (BasicIoManip val)
    {
        if (_stream)
            *_stream << val;
        return *this;
    }

    ConsoleBase & operator << (_SetForegroundColor color);
    ConsoleBase & operator << (_SetBackgroundColor color);

protected:
    std::basic_ostream<CharT> * _stream;
    int _handle;
    ConsoleColor _currentForegroundColor;
    ConsoleColor _currentBackgroundColor;
#if defined(WIN_MSVC) || defined(WIN_MINGW)
    WORD _defaultColorAttributes;
#endif

};

template<class CharT>
inline void ConsoleBase<CharT>::SetForegroundColor(ConsoleColor foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

template<class CharT>
inline void ConsoleBase<CharT>::SetBackgroundColor(ConsoleColor backgroundColor)
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

} // namespace OSAL

OSAL_EXPORT OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColor color);
OSAL_EXPORT OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColor color);

