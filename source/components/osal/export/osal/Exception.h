#pragma once

#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "osal/exports.h"
#include "osal/System.h"
#include "osal/Unused.h"

namespace OSAL
{

class BaseException : public std::exception
{
protected:
    std::string message;
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
    BaseException(const std::string & message)
        : message(message)
        , innerException(nullptr)
    {
    }
    BaseException(const std::string & message, const std::exception & innerException)
        : message(message)
        , innerException(&innerException)
    {
    }
    ~BaseException() noexcept
    {
    }
    BaseException & operator=(const BaseException & other)
    {
        message = other.message;
        innerException = other.innerException;
        return *this;
    }
    const char * what() const throw()
    {
        whatMessage = FormatMessage();
        return whatMessage.c_str();
    }
    virtual std::string BuildMessage() const = 0;

    const std::string & GetMessage() const
    {
        return message;
    }
    void SetMessage(const std::string & value)
    {
        message = value;
    }

private:
    std::string FormatMessage() const
    {
        std::ostringstream stream;
        stream << System::TypeName(*this);
        std::string buildMessage = BuildMessage();
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
    Exception(char const * functionName, char const * fileName, int line, const std::string & message)
        : BaseException(message)
        , functionName(functionName)
        , fileName(fileName)
        , line(line)
    {
    }
    Exception(char const * functionName,
        char const * fileName,
        int line,
        const std::string & message,
        const std::exception & innerException)
        : BaseException(message, innerException)
        , functionName(functionName)
        , fileName(fileName)
        , line(line)
    {
    }
    ~Exception()
    {
    }
    virtual std::string BuildMessage() const override
    {
        std::ostringstream stream;
        if (functionName != nullptr)
            stream << "in " << functionName;
        if (fileName != nullptr)
        {
            stream << " [" << fileName;
            if (line > 0)
                stream << ":" << line;
            stream << "]";
        }
        return stream.str();
    }
};

class RuntimeError : public Exception
{
public:
    RuntimeError(char const * functionName, char const * fileName, int line, std::string message)
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
    std::string argument;

public:
    ArgumentException(char const * functionName, char const * fileName, int line, std::string argument)
        : Exception(functionName, fileName, line)
        , argument(argument)
    {
    }
    ArgumentException(char const * functionName,
        char const * fileName,
        int line,
        std::string argument,
        std::string message)
        : Exception(functionName, fileName, line, message)
        , argument(argument)
    {
    }
    ~ArgumentException()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument: " << argument;
        return stream.str();
    }
};

class ArgumentNullException : public ArgumentException
{
public:
    ArgumentNullException(char const * functionName, char const * fileName, int line, std::string argument)
        : ArgumentException(functionName, fileName, line, argument)
    {
    }
    ~ArgumentNullException() throw()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument null: " << argument;
        return stream.str();
    }
};

class ArgumentOutOfRangeException : public ArgumentException
{
public:
    ArgumentOutOfRangeException(char const * functionName, char const * fileName, int line,
        std::string argument)
        : ArgumentException(functionName, fileName, line, argument)
    {
    }
    ArgumentOutOfRangeException(char const * functionName, char const * fileName, int line,
        std::string argument, std::string message)
        : ArgumentException(functionName, fileName, line, argument, message)
    {
    }
    ~ArgumentOutOfRangeException() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument out of range: " << argument;
        return stream.str();
    }
};

} // namespace OSAL

#if defined(WIN_MSVC)
#include "osal/windows/Exception.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Exception.h"
#elif defined(DARWIN)
#include "osal/darwin/Exception.h"
#elif defined(LINUX)
#include "osal/linux/Exception.h"
#endif
