#pragma once

#include <iostream>

#include "osal/osal.h"
#include "osal/Files.h"
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

class OSAL_EXPORT Console
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::ostream & (* BasicIoManip)(std::ostream &);

public:
    static const int InvalidHandle = -1;

    Console(int handle = OSAL::Files::GetFileDescriptor(stdout));
    Console(FILE * file);
    Console(std::ostream & stream);

    void SetForegroundColor(ConsoleColor foregroundColor);
    void SetBackgroundColor(ConsoleColor backgroundColor);
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default,
                          ConsoleColor backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor();
    bool ShouldUseColor();

    // Streams a non-pointer _value to this object.
    template <typename T>
    inline Console & operator << (const T & val)
    {
        using ::operator <<;
        if (_stream)
            *_stream << val;
        return *this;
    }

    Console & operator << (BasicIoManip val)
    {
        if (_stream)
            *_stream << val;
        return *this;
    }

    Console & operator << (_SetForegroundColor color);
    Console & operator << (_SetBackgroundColor color);

protected:
    Files::FileDescriptor _handle;
    std::ostream * _stream;
    ConsoleColor _currentForegroundColor;
    ConsoleColor _currentBackgroundColor;

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    WORD _defaultColorAttributes;
#endif
};

inline void Console::SetForegroundColor(ConsoleColor foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

inline void Console::SetBackgroundColor(ConsoleColor backgroundColor)
{
    SetTerminalColor(_currentForegroundColor, backgroundColor);
}

inline void Console::ResetTerminalColor()
{
    SetTerminalColor();
}

inline Console & Console::operator<<(_SetForegroundColor color)
{
    SetForegroundColor(color.color);
    return *this;
}

inline Console & Console::operator<<(_SetBackgroundColor color)
{
    SetBackgroundColor(color.color);
    return *this;
}

} // namespace OSAL

OSAL_EXPORT OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColor color);
OSAL_EXPORT OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColor color);

