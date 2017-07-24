#pragma once

#include <string>
#include "xml/XMLDefs.h"

namespace XMLParser
{

enum class XMLError
{
    NoError = 0,
    Error,
    ErrorOpeningFile,
    ErrorParsingElement,
    ErrorFailedToReadElementName,
    ErrorReadingElementValue,
    ErrorReadingAttributes,
    ErrorParsingEmpty,
    ErrorReadingEndTag,
    ErrorParsingUnknown,
    ErrorParsingComment,
    ErrorParsingDeclaration,
    ErrorDocumentEmpty,
    ErrorEmbeddedNull,
    ErrorParsingCDATA,
    ErrorDocumentTopOnly,
    ErrorDuplicateAttribute,

    ErrorStringCount
};

class XMLErrorInfo
{
public:
    XMLErrorInfo()
    {
    }

    XMLErrorInfo(XMLError errorCode, XMLCursor errorLocation);
    void Clear();

    XMLError ErrorCode() const
    {
        return errorCode;
    }

    const OSAL::String & ErrorDescription() const
    {
        return errorDescription;
    }

    int ErrorRow() const
    {
        return errorLocation.Row() + 1;
    }

    int ErrorCol() const
    {
        return errorLocation.Col() + 1;
    }

private:
    static const char * ErrorString[];
    XMLError errorCode{};
    OSAL::String errorDescription;
    XMLCursor errorLocation;
};

class XMLException
    : public OSAL::BaseException
{
public:
    XMLException() : OSAL::BaseException()
    {
    }

    XMLException(const OSAL::String & message)
        : OSAL::BaseException(message)
    {
    }

    virtual OSAL::String BuildMessage() const override
    {
        return {};
    }
};

class XMLInternalException
    : public XMLException
{
protected:
    const char * errorPtr;
    XMLError errorCode;
public:
    XMLInternalException(const char * errorPtr, XMLError errorCode)
        : XMLException(), errorPtr(errorPtr), errorCode(errorCode)
    {
    }

    const char * ErrorPosition() const
    {
        return errorPtr;
    }

    XMLError ErrorCode() const
    {
        return errorCode;
    }
};

class XMLParseException
    : public XMLException
{
protected:
    const char * errorPtr;
    XMLErrorInfo errorInfo;
public:
    XMLParseException(const char * errorPtr, const XMLErrorInfo & errorInfo)
        : XMLException(), errorPtr(errorPtr), errorInfo(errorInfo)
    {
    }

    virtual OSAL::String BuildMessage() const override
    {
        std::ostringstream stream;
        stream << "Error in XML: " << errorInfo.ErrorDescription() << std::endl;
        stream << errorPtr;
        stream << " (" << errorInfo.ErrorRow() << ":" << errorInfo.ErrorCol() << ")";
        return stream.str();
    }

    const XMLErrorInfo ErrorInfo() const
    {
        return errorInfo;
    }
};

class XMLValueParseException
    : public XMLException
{
public:
    XMLValueParseException(const OSAL::String & message)
        : XMLException(message)
    {
    }
};

} // namespace XMLParser

