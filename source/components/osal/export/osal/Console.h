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

namespace OSAL {

#if defined(LINUX) || defined(DARWIN)
    enum class ConsoleColor : int {
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

template<>
struct is_flag<OSAL::ConsoleColor>
{
    static constexpr bool value = true;
};

} // namespace OSAL

OSAL_EXPORT std::ostream & operator << (std::ostream & stream, OSAL::ConsoleColor value);

namespace OSAL {

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
protected:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::ostream & (* BasicIoManip)(std::ostream &);

public:
    static const int InvalidHandle = -1;

    Console(int handle = OSAL::Files::GetFileDescriptor(stdout));
    Console(FILE * file);
    Console(std::ostream & stream);

    std::ostream * GetStream() { return _stream; }

    void SetForegroundColor(ConsoleColor foregroundColor)
    {
        SetTerminalColor(foregroundColor, _currentBackgroundColor);
    }
    void SetBackgroundColor(ConsoleColor backgroundColor)
    {
        SetTerminalColor(_currentForegroundColor, backgroundColor);
    }
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default,
                          ConsoleColor backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor()
    {
        SetTerminalColor();
    }
    bool ShouldUseColor() const;
    bool ForceUseColor() const;
    void ForceUseColor(bool value);

    // Streams a non-pointer _value to this object.
    template <typename T>
    Console & operator << (const T & val)
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

    Console & operator << (_SetForegroundColor color)
    {
        SetForegroundColor(color.color);
        return *this;
    }
    Console & operator << (_SetBackgroundColor color)
    {
        SetBackgroundColor(color.color);
        return *this;
    }

protected:
    Files::FileDescriptor _handle;
    std::ostream * _stream;
    bool _forceUseColor;
    ConsoleColor _currentForegroundColor;
    ConsoleColor _currentBackgroundColor;

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    WORD _defaultColorAttributes;
#endif
};

} // namespace OSAL

OSAL_EXPORT OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColor color);
OSAL_EXPORT OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColor color);

