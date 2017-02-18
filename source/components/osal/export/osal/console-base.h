#pragma once

#include <iostream>
#include "osal/osal.h"
#include "osal/flagoperators.h"

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

using ConsoleColorType = Flag<ConsoleColor, int>;
DEFINE_FLAG_OPERATORS(ConsoleColor, int);

template<class CharT>
OSAL_EXPORT std::basic_ostream<CharT> & operator << (std::basic_ostream<CharT> & stream, ConsoleColorType value);

template<class CharT>
class ConsoleBase
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::basic_ostream<CharT> & (* BasicIoManip)(std::basic_ostream<CharT> &);

public:
    static const int InvalidHandle = -1;

    ConsoleBase(int handle = OSAL::Files::FileNo(stdout));
    ConsoleBase(std::basic_ostream<CharT> & stream);

    void SetForegroundColor(ConsoleColorType foregroundColor);
    void SetBackgroundColor(ConsoleColorType backgroundColor);
    void SetTerminalColor(ConsoleColorType foregroundColor = ConsoleColor::Default,
                          ConsoleColorType backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor();
    bool ShouldUseColor();

    // Streams a non-pointer value to this object.
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

protected:
    std::basic_ostream<CharT> * _stream;
    int _handle;
    ConsoleColorType _currentForegroundColor;
    ConsoleColorType _currentBackgroundColor;
#if defined(WIN_MSVC) || defined(WIN_MINGW)
    WORD _defaultColorAttributes;
#endif

};

struct _SetForegroundColor
{
    ConsoleColorType color;
};

struct _SetBackgroundColor
{
    ConsoleColorType color;
};

template<class CharT>
OSAL_EXPORT ConsoleBase<CharT> & operator << (ConsoleBase<CharT> & stream, _SetForegroundColor color);
template<class CharT>
OSAL_EXPORT ConsoleBase<CharT> & operator << (ConsoleBase<CharT> & stream, _SetBackgroundColor color);

} // namespace OSAL

OSAL_EXPORT OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColorType color);
OSAL_EXPORT OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColorType color);

