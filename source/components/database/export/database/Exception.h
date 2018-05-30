#pragma once

#include "osal/Exception.h"
#include "database/exports.h"

namespace Database
{

class DATABASE_EXPORT ConnectException : public OSAL::Exception
{
public:
    ConnectException(const char * functionName, const char * fileName, int line, const std::string & message)
        : OSAL::Exception(functionName, fileName, line, message)
    {
    }
};

class DATABASE_EXPORT FieldIndexException : public OSAL::Exception
{
protected:
    std::string location;
    size_t index;
public:
    FieldIndexException(const char * functionName, const char * fileName, int line,
                        const std::string & location, size_t index)
        : OSAL::Exception(functionName, fileName, line)
        , location(location)
        , index(index)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << OSAL::Exception::BuildMessage();
        if (!location.empty())
            stream << " @" << location;
        stream << ": Invalid field index: " << index;
        return stream.str();
    }
};

class DATABASE_EXPORT RowIndexException : public OSAL::Exception
{
protected:
    std::string location;
    size_t index;
public:
    RowIndexException(const char * functionName, const char * fileName, int line,
                      const std::string & location, size_t index)
        : OSAL::Exception(functionName, fileName, line)
        , location(location)
        , index(index)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << OSAL::Exception::BuildMessage();
        if (!location.empty())
            stream << " @" << location;
        stream << ": Invalid row index: " << index;
        return stream.str();
    }
};

class DATABASE_EXPORT QueryNumRowsException : public OSAL::Exception
{
protected:
    std::string location;
    size_t expected;
    size_t actual;
public:
    QueryNumRowsException(const char * functionName, const char * fileName, int line,
                          const std::string & location, size_t expected, size_t actual)
        : OSAL::Exception(functionName, fileName, line)
        , location(location)
        , expected(expected)
        , actual(actual)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << OSAL::Exception::BuildMessage();
        if (!location.empty())
            stream << " @" << location;
        stream << ": Invalid number of rows returned: expect " << expected
               << " actual " + actual;
        return stream.str();
    }
};

} // DBSupport
