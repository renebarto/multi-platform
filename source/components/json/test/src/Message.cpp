#include "Message.h"

#include <iomanip>
#include <core/serialization/SerializationImpl.h>

// A Unicode code-point can have upto 21 bits, and is encoded in UTF-8
// like this:
//
// Code-point length   Encoding
//   0 -  7 bits       0xxxxxxx
//   8 - 11 bits       110xxxxx 10xxxxxx
//  12 - 16 bits       1110xxxx 10xxxxxx 10xxxxxx
//  17 - 21 bits       11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

// The maximum code-point a one-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint1 = (static_cast<uint32_t>(1) <<  7) - 1;

// The maximum code-point a two-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint2 = (static_cast<uint32_t>(1) << (5 + 6)) - 1;

// The maximum code-point a three-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint3 = (static_cast<uint32_t>(1) << (4 + 2*6)) - 1;

// The maximum code-point a four-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint4 = (static_cast<uint32_t>(1) << (3 + 3*6)) - 1;

// Chops off the n lowest bits from a bit pattern.  Returns the n
// lowest bits.  As a side effect, the original bit pattern will be
// shifted to the right by n bits.
inline uint32_t ChopLowBits(uint32_t* bits, int n)
{
    const uint32_t low_bits = *bits & ((static_cast<uint32_t>(1) << n) - 1);
    *bits >>= n;
    return low_bits;
}

// The following two functions only make sense if the the system
// uses UTF-16 for wide string encoding. All supported systems
// with 16 bit wchar_t (Windows, Cygwin, Symbian OS) do use UTF-16.

// Converts a Unicode code point to a narrow string in UTF-8 encoding.
// code_point parameter is of type uint32_t because wchar_t may not be
// wide enough to contain a code point.
// If the code_point is not a valid Unicode code point
// (i.e. outside of Unicode range U+0 to U+10FFFF) it will be converted
// to "(Invalid Unicode 0xXXXXXXXX)".
std::string CodePointToUtf8(uint32_t code_point)
{
    if (code_point > kMaxCodePoint4)
    {
        return "(Invalid Unicode 0x" + Core::Serialize(code_point) + ")";
    }

    char str[5];  // Big enough for the largest valid code point.
    if (code_point <= kMaxCodePoint1)
    {
        str[1] = '\0';
        str[0] = static_cast<char>(code_point);                          // 0xxxxxxx
    } else if (code_point <= kMaxCodePoint2)
    {
        str[2] = '\0';
        str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[0] = static_cast<char>(0xC0 | code_point);                   // 110xxxxx
    } else if (code_point <= kMaxCodePoint3)
    {
        str[3] = '\0';
        str[2] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[0] = static_cast<char>(0xE0 | code_point);                   // 1110xxxx
    } else
    {  // code_point <= kMaxCodePoint4
        str[4] = '\0';
        str[3] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[2] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[0] = static_cast<char>(0xF0 | code_point);                   // 11110xxx
    }
    return str;
}

// Determines if the arguments constitute UTF-16 surrogate pair
// and thus should be combined into a single Unicode code point
// using CreateCodePointFromUtf16SurrogatePair.
inline bool IsUtf16SurrogatePair(wchar_t first, wchar_t second) {
    return sizeof(wchar_t) == 2 &&
           (first & 0xFC00) == 0xD800 && (second & 0xFC00) == 0xDC00;
}

// Creates a Unicode code point from UTF16 surrogate pair.
inline uint32_t CreateCodePointFromUtf16SurrogatePair(wchar_t first, wchar_t second)
{
    const uint32_t mask = (1 << 10) - 1;
    return (sizeof(wchar_t) == 2)
           ? (((first & mask) << 10) | (second & mask)) + 0x10000
           // This function should not be called when the condition is
           // false, but we provide a sensible default in case it is.
           : static_cast<uint32_t>(first);
}

// Converts the buffer in a stringstream to an std::string, converting NUL
// bytes to "\\0" along the way.
std::string StringStreamToString(::std::stringstream* ss)
{
    const ::std::string& str = ss->str();
    const char* const start = str.c_str();
    const char* const end = start + str.length();

    std::string result;
    result.reserve(2 * (end - start));
    for (const char* ch = start; ch != end; ++ch)
    {
        if (*ch == '\0')
        {
            result += "\\0";  // Replaces NUL with "\\0";
        } else
        {
            result += *ch;
        }
    }

    return result;
}

// Converts a wide string to a narrow string in UTF-8 encoding.
// The wide string is assumed to have the following encoding:
//   UTF-16 if sizeof(wchar_t) == 2 (on Windows, Cygwin, Symbian OS)
//   UTF-32 if sizeof(wchar_t) == 4 (on Linux)
// Parameter str points to a null-terminated wide string.
// Parameter num_chars may additionally limit the number
// of wchar_t characters processed. -1 is used when the entire string
// should be processed.
// If the string contains code points that are not valid Unicode code points
// (i.e. outside of Unicode range U+0 to U+10FFFF) they will be output
// as '(Invalid Unicode 0xXXXXXXXX)'. If the string is in UTF16 encoding
// and contains invalid UTF-16 surrogate pairs, values in those pairs
// will be encoded as individual Unicode characters from Basic Normal Plane.
std::string WideStringToUtf8(const wchar_t * str, int num_chars)
{
    if (num_chars == -1)
        num_chars = static_cast<int>(wcslen(str));

    std::stringstream stream;
    for (int i = 0; i < num_chars; ++i)
    {
        uint32_t unicode_code_point;

        if (str[i] == L'\0')
        {
            break;
        } else if (i + 1 < num_chars && IsUtf16SurrogatePair(str[i], str[i + 1]))
        {
            unicode_code_point = CreateCodePointFromUtf16SurrogatePair(str[i],
                                                                       str[i + 1]);
            i++;
        } else
        {
            unicode_code_point = static_cast<uint32_t>(str[i]);
        }

        stream << CodePointToUtf8(unicode_code_point);
    }
    return StringStreamToString(&stream);
}

// Converts a wide C string to an std::string using the UTF-8 encoding.
// NULL will be converted to "(null)".
std::string ShowWideCString(const wchar_t * wide_c_str) {
    if (wide_c_str == NULL)  return "(null)";

    return WideStringToUtf8(wide_c_str, -1);
}

// Converts an array of wide chars to a narrow string using the UTF-8
// encoding, and streams the result to the given Message object.
static void StreamWideCharsToMessage(const wchar_t * wstr, size_t length,
                                     Message * msg)
{
    for (size_t i = 0; i != length; )
    {
        if (wstr[i] != L'\0')
        {
            *msg << WideStringToUtf8(wstr + i, static_cast<int>(length - i));
            while (i != length && wstr[i] != L'\0')
                i++;
        } else
        {
            *msg << '\0';
            i++;
        }
    }
}

// Constructs an empty Message.
// We allocate the stringstream separately because otherwise each use of
// ASSERT/EXPECT in a procedure adds over 200 bytes to the procedure's
// stack frame leading to huge stack frames in some cases; gcc does not reuse
// the stack space.
Message::Message()
    : ss_(new std::stringstream)
{
    // By default, we want there to be enough precision when printing
    // a double to a Message.
    *ss_ << std::setprecision(std::numeric_limits<double>::digits10 + 2);
}

// These two overloads allow streaming a wide C string to a Message
// using the UTF-8 encoding.
Message & Message::operator << (const wchar_t * wide_c_str)
{
    return *this << ShowWideCString(wide_c_str);
}
Message & Message::operator << (wchar_t * wide_c_str)
{
    return *this << ShowWideCString(wide_c_str);
}

// Converts the given wide string to a narrow string using the UTF-8
// encoding, and streams the result to this Message object.
Message & Message::operator <<(const std::wstring & wstr)
{
  StreamWideCharsToMessage(wstr.c_str(), wstr.length(), this);
  return *this;
}

// Gets the text streamed to this object so far as an std::string.
// Each '\0' character in the buffer is replaced with "\\0".
std::string Message::GetString() const {
    return StringStreamToString(ss_.get());
}


// Streams a Message to an ostream.
std::ostream & operator <<(std::ostream & os, const Message & sb)
{
    return os << sb.GetString();
}
