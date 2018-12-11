#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <osal/Assert.h>
#include <osal/Console.h>
#include "core/core.h"
#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

namespace Core {

extern CORE_EXPORT void TraceWarning(const std::string & format);
extern CORE_EXPORT void TraceError(const std::string & format);

template <typename T>
T Argument(T value) noexcept
{
    return value;
}

template <typename T>
T const * Argument(std::basic_string<T> const & value) noexcept
{
    return value.c_str();
}

template <typename ... Args>
size_t StringPrint(char * const buffer,
                   size_t const bufferCount,
                   char const * const format,
                   Args const & ... args) noexcept
{
    int const result = snprintf(buffer,
                                bufferCount,
                                format,
                                Argument(args) ...);
    ASSERT(-1 != result);
    return static_cast<size_t>(result);
}
template <typename ... Args>
size_t StringPrint(wchar_t * const buffer,
                   size_t const bufferCount,
                   wchar_t const * const format,
                   Args const & ... args) noexcept
{
    int const result = swprintf(buffer,
                                bufferCount,
                                format,
                                Argument(args) ...);
    ASSERT(-1 != result);
    return static_cast<size_t>(result);
}
template <typename ... Args>
size_t PrintStdout(char const * const format,
                 Args const & ... args) noexcept
{
    int const result = printf(format, args ...);
    ASSERT(-1 != result);
    return static_cast<size_t>(result);
}
template <typename ... Args>
size_t PrintStdout(wchar_t const * const format,
                   Args const & ... args) noexcept
{
    int const result = wprintf(format, args ...);
    ASSERT(-1 != result);
    return static_cast<size_t>(result);
}

// Printing with C# like formatting
// Format:
// {<index><,width<:format>>}
// If index is left out, the logical ordering applies (0, 1, 2, 3...)
// If index is specified, it refers to the parameter with specified index, counting from 0
// If width is left out, the width needed to print is used
// If width is specified, this width will be used (right aligned for positive with left aligned for negative width), unless the width needed is larger
// If format is left out, the standard printing format is used
// If format is specified, the type of argument defines how the format is used:
// For integral values:
//      D<n> for decimal values with n digits (leading zeros if needed, actual width if n is too small)
//      O<n> for octal values with n digits (leading zeros if needed, actual width if n is too small)
//      B<n> for binary values with n digits (leading zeros if needed, actual width if n is too small)
//      X<n> for hexadecimal values with n digits (leading zeros if needed, actual width if n is too small)
// For floating point values:
//      F<n> for fixed representation using n fractional digits
//      E<n> for exponential representation using n fractional digits
//      G<n> for fixed or exponential representation (whichever fits best) (n is ignored)

template<typename T, typename V>
typename std::enable_if<!std::is_integral<V>::value && !std::is_floating_point<V>::value, std::basic_string<T>>::type
Format(V value, int width, const std::basic_string<T> & UNUSED(format))
{
    return Serialize(value, width);
};

template<typename T, typename V>
typename std::enable_if<std::is_integral<V>::value, std::basic_string<T>>::type
Format(V value, int width, const std::basic_string<T> & format)
{
    if (!format.empty())
    {
        switch (format[0])
        {
            case 'D':
                {
                    int digits {};
                    if (format.length() > 1)
                        Deserialize(format.substr(1), digits);
                    return Align(Serialize(value, digits, 10), width);
                }
                break;
            case 'O':
                {
                    int digits {};
                    if (format.length() > 1)
                        Deserialize(format.substr(1), digits);
                    return Align(Serialize(value, digits, 8), width);
                }
                break;
            case 'B':
                {
                    int digits {};
                    if (format.length() > 1)
                        Deserialize(format.substr(1), digits);
                    return Align(Serialize(value, digits, 2), width);
                }
                break;
            case 'X':
                {
                    int digits {};
                    if (format.length() > 1)
                        Deserialize(format.substr(1), digits);
                    return Align(Serialize(value, digits, 16), width);
                }
                break;
            default:
                return Serialize(value, width);
        }
    }
    return Serialize(value, width);
};

template<typename T, typename V>
typename std::enable_if<std::is_floating_point<V>::value, std::basic_string<T>>::type
Format(V value, int width, const std::basic_string<T> & format)
{
    if (!format.empty())
    {
        switch (format[0]) {
            case 'F':
                {
                    std::basic_string<T> subFormat = format.substr(1);
                    auto decimalPointIndex = subFormat.find(T('.'));
                    std::basic_string<T> integralDecimalsSpecifier = subFormat;
                    std::basic_string<T> fractionalDecimalsSpecifier;
                    if (decimalPointIndex != std::basic_string<T>::npos)
                    {
                        integralDecimalsSpecifier = subFormat.substr(0, decimalPointIndex);
                        fractionalDecimalsSpecifier = subFormat.substr(decimalPointIndex + 1);
                    }
                    int integralDecimals {};
                    int fractionalDecimals {};
                    Deserialize(integralDecimalsSpecifier, integralDecimals);
                    Deserialize(fractionalDecimalsSpecifier, fractionalDecimals);
                    return Align(Serialize(value, integralDecimals + fractionalDecimals + 1, fractionalDecimals,
                                           FloatingPointRepresentation::Fixed), width);
                }
                break;
            case 'E':
                {
                    std::basic_string<T> subFormat = format.substr(1);
                    auto decimalPointIndex = subFormat.find(T('.'));
                    std::basic_string<T> integralDecimalsSpecifier = subFormat;
                    std::basic_string<T> fractionalDecimalsSpecifier;
                    if (decimalPointIndex != std::basic_string<T>::npos)
                    {
                        integralDecimalsSpecifier = subFormat.substr(0, decimalPointIndex);
                        fractionalDecimalsSpecifier = subFormat.substr(decimalPointIndex + 1);
                    }
                    int integralDecimals {};
                    int fractionalDecimals {};
                    Deserialize(integralDecimalsSpecifier, integralDecimals);
                    Deserialize(fractionalDecimalsSpecifier, fractionalDecimals);
                    return Align(Serialize(value, integralDecimals + fractionalDecimals + 1, fractionalDecimals,
                                           FloatingPointRepresentation::Exponential), width);
                }
                break;
            case 'G':
                {
                    std::basic_string<T> subFormat = format.substr(1);
                    auto decimalPointIndex = subFormat.find(T('.'));
                    std::basic_string<T> integralDecimalsSpecifier = subFormat;
                    std::basic_string<T> fractionalDecimalsSpecifier;
                    if (decimalPointIndex != std::basic_string<T>::npos)
                    {
                        integralDecimalsSpecifier = subFormat.substr(0, decimalPointIndex);
                        fractionalDecimalsSpecifier = subFormat.substr(decimalPointIndex + 1);
                    }
                    int integralDecimals {};
                    int fractionalDecimals {};
                    Deserialize(integralDecimalsSpecifier, integralDecimals);
                    Deserialize(fractionalDecimalsSpecifier, fractionalDecimals);
                    // We need different behaviour here due to how default output of floating point numbers works
                    return Align(Serialize(value, 0, integralDecimals + fractionalDecimals + 1,
                                           FloatingPointRepresentation::Mixed), width);
                }
                break;
            default:
                return Serialize(value, width);
        }
    }
    return Serialize(value, width);
};

template<typename T>
void Format(std::basic_string<T> & UNUSED(buffer),
            size_t UNUSED(index), size_t UNUSED(parameterIndex),
            int UNUSED(parameterWidth),
            const std::basic_string<T> & UNUSED(parameterFormat))
{
}

template<typename T, typename V, typename ... Args>
void Format(std::basic_string<T> & buffer,
            size_t index, size_t parameterIndex,
            int parameterWidth,
            const std::basic_string<T> & parameterFormat,
            V value, Args ... FArgs)
{
    if (index == parameterIndex)
    {
        buffer += Format<T>(value, parameterWidth, parameterFormat);
    }
    else
        Format(buffer, index, parameterIndex + 1, parameterWidth, parameterFormat, FArgs ...);
}

template <typename T, typename ... Args>
void Format(std::basic_string<T> & buffer,
            T const * const format,
            Args const & ... args)
{
    size_t numParameters = sizeof...(args);
    std::vector<bool> parameterUsed(numParameters);
    const T * currentPosition = format;
    size_t parameterOrdinal {};
    while (*currentPosition != T('\0'))
    {
        const T * parameterStartIndication = OSAL::Strings::strchr(currentPosition, T('{'));
        if (parameterStartIndication != nullptr)
        {
            buffer += std::basic_string<T>(currentPosition, static_cast<size_t>(parameterStartIndication - currentPosition));
            const T * parameterEndIndication = OSAL::Strings::strchr(parameterStartIndication + 1, T('}'));
            if (parameterEndIndication != nullptr)
            {
                std::basic_string<T> parameterID;
                std::basic_string<T> widthSpecifier;
                std::basic_string<T> formatSpecifier;
                parameterID = std::basic_string<T>(parameterStartIndication + 1, static_cast<size_t>(parameterEndIndication - parameterStartIndication - 1));
                const T * parameterWidthDelimiter = OSAL::Strings::strchr(parameterStartIndication + 1, T(','));
                if ((parameterWidthDelimiter != nullptr) && (parameterWidthDelimiter < parameterEndIndication))
                {
                    parameterID = std::basic_string<T>(parameterStartIndication + 1, static_cast<size_t>(parameterWidthDelimiter - parameterStartIndication - 1));
                    widthSpecifier = std::basic_string<T>(parameterWidthDelimiter + 1, static_cast<size_t>(parameterEndIndication - parameterWidthDelimiter - 1));
                    const T * parameterFormatDelimiter = OSAL::Strings::strchr(parameterWidthDelimiter + 1, T(':'));
                    if ((parameterFormatDelimiter != nullptr) && (parameterFormatDelimiter < parameterEndIndication))
                    {
                        widthSpecifier = std::basic_string<T>(parameterWidthDelimiter + 1, static_cast<size_t>(parameterFormatDelimiter - parameterWidthDelimiter - 1));
                        formatSpecifier = std::basic_string<T>(parameterFormatDelimiter + 1, static_cast<size_t>(parameterEndIndication - parameterFormatDelimiter - 1));
                    }
                }
                size_t parameterIndex = parameterOrdinal;
                if (!parameterID.empty() && !Deserialize(parameterID, parameterIndex))
                {
                    std::ostringstream stream;
                    stream << "Invalid index specified: " << parameterID;
                    TraceError(stream.str());
                    return;
                }
                if (parameterIndex < 0 || parameterIndex >= numParameters)
                {
                    std::ostringstream stream;
                    stream << "Invalid index specified: " << parameterID << " should be between 0 and " << numParameters - 1;
                    TraceError(stream.str());
                    return;
                }
                int width {};
                if (!widthSpecifier.empty() && !Deserialize(widthSpecifier, width))
                {
                    std::ostringstream stream;
                    stream << "Invalid width specified: " << widthSpecifier;
                    TraceError(stream.str());
                    return;
                }
                parameterUsed[parameterIndex] = true;
                Format(buffer, parameterIndex, 0, width, formatSpecifier, args ...);

                currentPosition = parameterEndIndication + 1;
            } else
            {
                break;
            }
            ++parameterOrdinal;
        } else
        {
            buffer += std::basic_string<T>(currentPosition);
            break;
        }
    }
    bool haveUnusedElements = false;
    std::ostringstream messageStream;
    for (size_t i = 0; i < numParameters; ++i)
    {
        if (!parameterUsed[i])
        {
            if (!haveUnusedElements)
            {
                messageStream << "Not all parameters have been used. Parameters not used:";
                haveUnusedElements = true;
            }
            messageStream << " " << i;
        }
    }
    if (haveUnusedElements)
        TraceWarning(messageStream.str());
}

template <typename ... Args>
void Print(std::ostream & stream, char const * const format,
           Args const & ... args) noexcept
{
    std::string str;
    Format(str, format, args ...);
    stream << str;
}

template <typename ... Args>
void Print(OSAL::Console & console, char const * const format,
           Args const & ... args) noexcept
{
    std::string str;
    Format(str, format, args ...);
    console << str;
}

inline void Print(std::ostream & stream, char const * const value) noexcept
{
    Print(stream, "{}", value);
}

inline void Print(OSAL::Console & console, char const * const value) noexcept
{
    Print(console, "{}", value);
}

inline void Print(std::ostream & stream, wchar_t const * const value) noexcept
{
    Print(stream, "{}", value);
}

inline void Print(OSAL::Console & console, wchar_t const * const value) noexcept
{
    Print(console, "{}", value);
}

template <typename T>
void Print(std::ostream & stream, std::basic_string<T> const & value) noexcept
{
    Print(stream, value.c_str());
}

template <typename T>
void Print(OSAL::Console & console, std::basic_string<T> const & value) noexcept
{
    Print(console, value.c_str());
}

} // namespace Core