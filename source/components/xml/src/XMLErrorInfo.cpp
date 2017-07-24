#include "xml/XMLErrorInfo.h"

namespace XMLParser
{

XMLErrorInfo::XMLErrorInfo(XMLError errorCode, XMLCursor errorLocation)
    : errorCode(errorCode)
    , errorDescription(ErrorString[int(errorCode)])
    , errorLocation(errorLocation)
{
}

void XMLErrorInfo::Clear()
{
    this->errorCode        = XMLError::NoError;
    this->errorDescription = "";
    this->errorLocation.Clear();
}

const char * XMLErrorInfo::ErrorString[int(XMLError::ErrorStringCount)] = {
    "No error",
    "Error",
    "Failed to open file.",
    "Error parsing Element.",
    "Failed to read Element name.",
    "Error reading Element value.",
    "Error reading Attributes.",
    "Error: empty tag.",
    "Error reading end tag.",
    "Error parsing Unknown.",
    "Error parsing Comment.",
    "Error parsing Declaration.",
    "Error document empty.",
    "Error null (0) or unexpected EOF found in input stream.",
    "Error parsing CDATA.",
    "Error when XMLDocument added to document, because "
    "XMLDocument can only be at the root.",
    "Error: duplicate attribute.",
};

} // namespace XMLParser
