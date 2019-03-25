#pragma once

#include <iostream>
#include <memory>
#include <sstream>

namespace internal {

// A secret type that Google Test users don't know about.  It has no
// definition on purpose.  Therefore it's impossible to create a
// Secret object, which is what we want.
class Secret;

} // namespace internal

// Ensures that there is at least one operator<< in the global namespace.
// See Message& operator<<(...) below for why.
void operator<<(const internal::Secret&, int);

class Message
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for
    // narrow streams.
    typedef std::ostream& (*BasicNarrowIoManip)(std::ostream&);

public:
    // Constructs an empty Message.
    Message();

    // Copy constructor.
    Message(const Message & msg)
        : ss_(new std::stringstream)
    {
        *ss_ << msg.GetString();
    }

    // Constructs a Message from a C-string.
    explicit Message(const char * str)
        : ss_(new std::stringstream)
    {
        *ss_ << str;
    }

    void operator = (const Message &) = delete;

    // Streams a non-pointer value to this object.
    template <typename T>
    inline Message& operator << (const T & val)
    {
        // Some libraries overload << for STL containers.  These
        // overloads are defined in the global namespace instead of ::std.
        //
        // C++'s symbol lookup rule (i.e. Koenig lookup) says that these
        // overloads are visible in either the std namespace or the global
        // namespace, but not other namespaces, including the testing
        // namespace which Google Test's Message class is in.
        //
        // To allow STL containers (and other types that has a << operator
        // defined in the global namespace) to be used in Google Test
        // assertions, testing::Message must access the custom << operator
        // from the global namespace.  With this using declaration,
        // overloads of << defined in the global namespace and those
        // visible via Koenig lookup are both exposed in this function.
        using ::operator <<;
        *ss_ << val;
        return *this;
    }

    // Streams a pointer value to this object.
    //
    // This function is an overload of the previous one.  When you
    // stream a pointer to a Message, this definition will be used as it
    // is more specialized.  (The C++ Standard, section
    // [temp.func.order].)  If you stream a non-pointer, then the
    // previous definition will be used.
    //
    // The reason for this overload is that streaming a NULL pointer to
    // ostream is undefined behavior.  Depending on the compiler, you
    // may get "0", "(nil)", "(null)", or an access violation.  To
    // ensure consistent result across compilers, we always treat NULL
    // as "(null)".
    template <typename T>
    inline Message& operator << (T * const & pointer)
    {
        if (pointer == nullptr)
        {
            *ss_ << "(null)";
        } else
        {
            *ss_ << pointer;
        }
        return *this;
    }

    // Since the basic IO manipulators are overloaded for both narrow
    // and wide streams, we have to provide this specialized definition
    // of operator <<, even though its body is the same as the
    // templatized version above.  Without this definition, streaming
    // endl or other basic IO manipulators to Message will confuse the
    // compiler.
    Message& operator << (BasicNarrowIoManip val)
    {
        *ss_ << val;
        return *this;
    }

    // Instead of 1/0, we want to see true/false for bool values.
    Message& operator << (bool b)
    {
        return *this << (b ? "true" : "false");
    }

    // These two overloads allow streaming a wide C string to a Message
    // using the UTF-8 encoding.
    Message & operator << (const wchar_t * wide_c_str);
    Message & operator << (wchar_t * wide_c_str);
    Message & operator <<(const std::wstring & wstr);

    // Gets the text streamed to this object so far as an std::string.
    // Each '\0' character in the buffer is replaced with "\\0".
    //
    // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
    std::string GetString() const;

private:
    // We'll hold the text streamed to this object here.
    const std::unique_ptr<std::stringstream> ss_;
};

std::ostream & operator << (std::ostream & os, const Message & sb);

