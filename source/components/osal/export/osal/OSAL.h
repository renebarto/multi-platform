#pragma once

#include <iomanip>
#include <stdexcept>
#include <sstream>

#include "osal/exports.h"
#if defined(WIN_MSVC)
#include "osal/windows/OSAL.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/OSAL.h"
#elif defined(DARWIN)
#include "osal/darwin/OSAL.h"
#elif defined(LINUX)
#include "osal/linux/OSAL.h"
#endif
#include "osal/Unused.h"
#include "osal/OS.h"
#include "osal/Strings.h"

namespace OSAL {

class BaseException : public std::exception
{
protected:
    String message;
    const std::exception * innerException;
    mutable std::string whatMessage;
public:
    BaseException()
        : message()
          , innerException(nullptr)
    {
    }
    BaseException(const BaseException & innerException)
        : message()
          , innerException(&innerException)
    {
    }
    // Not a copy constructor!!
    BaseException(const std::exception & innerException)
        : message()
          , innerException(&innerException)
    {
    }
    BaseException(const String & message)
        : message(message)
          , innerException(nullptr)
    {
    }
    BaseException(const String & message, const std::exception & innerException)
        : message(message)
          , innerException(&innerException)
    {
    }
    ~BaseException() noexcept
    {
    }
    BaseException & operator = (const BaseException & other)
    {
        message = other.message;
        innerException = other.innerException;
        return *this;
    }
    const char* what() const throw()
    {
        whatMessage = ToNarrowString(FormatMessage());
        return whatMessage.c_str();
    }
    virtual String BuildMessage() const = 0;

    const String & GetMessage() const
    {
        return message;
    }
    void SetMessage(const String & value)
    {
        message = value;
    }
private:
    String FormatMessage() const
    {
        std::basic_ostringstream<Char> stream;
        stream << OS::TypeName(*this);
        String buildMessage = BuildMessage();
        if (!buildMessage.empty())
            stream << " - " << BuildMessage();
        if (!message.empty())
            stream << ": " << message;
        if (innerException)
        {
            stream << std::endl << "-->" << innerException->what();
        }
        return stream.str();
    }
};

class Exception : public BaseException
{
protected:
    char const * functionName;
    char const * fileName;
    int line;
public:
    Exception() = delete;
    Exception(char const * functionName, char const * fileName, int line)
        : BaseException()
          , functionName(functionName)
          , fileName(fileName)
          , line(line)
    {
    }
    Exception(char const * functionName, char const * fileName, int line, const String & message)
        : BaseException(message)
          , functionName(functionName)
          , fileName(fileName)
          , line(line)
    {
    }
    Exception(char const * functionName, char const * fileName, int line, const String & message, const std::exception & innerException)
        : BaseException(message, innerException)
          , functionName(functionName)
          , fileName(fileName)
          , line(line)
    {
    }
    ~Exception()
    {
    }
    virtual String BuildMessage() const override
    {
        std::basic_ostringstream<Char> stream;
        if (functionName != nullptr)
            stream << _("in ") << functionName;
        if (fileName != nullptr)
        {
            stream << _(" [") << fileName;
            if (line > 0)
                stream << ":" << line;
            stream << _("]");
        }
        return stream.str();
    }
};

class SystemError : public Exception
{
protected:
    int errorCode;
public:
    SystemError(char const * functionName, char const * fileName, int line, int errorCode)
        : Exception(functionName, fileName, line)
          , errorCode(errorCode)
    {
    }
    SystemError(char const * functionName, char const * fileName, int line, int errorCode, String message)
        : Exception(functionName, fileName, line, message)
          , errorCode(errorCode)
    {
    }
    ~SystemError()
    {
    }
    int GetErrorCode() const
    {
        return errorCode;
    }
    virtual String BuildMessage() const override
    {
        std::basic_ostringstream<Char> stream;
		stream << Exception::BuildMessage() << _(" errno=") << errorCode
			<< _(" (0x") << std::hex << std::setw(8) << std::setfill(_('0')) << errorCode << _("): \"")
			<< strerror(errorCode) << _("\"");
        return stream.str();
    }
};

class RuntimeError : public Exception
{
public:
    RuntimeError(char const * functionName, char const * fileName, int line, String message)
        : Exception(functionName, fileName, line, message)
    {
    }
    ~RuntimeError()
    {
    }
};

class ArgumentException : public Exception
{
protected:
    String argument;
public:
    ArgumentException(char const * functionName, char const * fileName, int line,
                      String argument)
        : Exception(functionName, fileName, line)
          , argument(argument)
    {
    }
    ArgumentException(char const * functionName, char const * fileName, int line,
                      String argument, String message)
        : Exception(functionName, fileName, line, message)
          , argument(argument)
    {
    }
    ~ArgumentException()
    {
    }
    virtual String BuildMessage() const
    {
        std::basic_ostringstream<Char> stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << _(": argument: ") << argument;
        return stream.str();
    }
};

//class ArgumentOutOfRangeException : public ArgumentException
//{
//public:
//    ArgumentOutOfRangeException(char const * functionName, char const * fileName, int line,
//                                String argument)
//    : ArgumentException(functionName, fileName, line, argument)
//    {
//    }
//    ArgumentOutOfRangeException(char const * functionName, char const * fileName, int line,
//                                String argument, String message)
//    : ArgumentException(functionName, fileName, line, argument, message)
//    {
//    }
//    ~ArgumentOutOfRangeException()
//    {
//    }
//    virtual String BuildMessage() const
//    {
//        std::ostringstream stream;
//        stream << Exception::BuildMessage();
//        if (!argument.empty())
//            stream << ": argument out of range: " << argument;
//        return stream.str();
//    }
//};

inline void ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode)
{
    if (errorCode != 0)
    {
        throw SystemError(functionName, fileName, line, errorCode);
    }
}

} // namespace OSAL

#include "osal/OS.h"
#include "osal/Files.h"
#include "osal/Path.h"
#include "osal/Strings.h"
#include "osal/System.h"
#include "osal/Time.h"
