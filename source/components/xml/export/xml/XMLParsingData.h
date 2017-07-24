#pragma once

#include <cassert>
#include "xml/XMLDefs.h"

namespace XMLParser
{

extern const char UTF_BOM0;
extern const char UTF_BOM1;
extern const char UTF_BOM2;
extern const char UTF_INV1_1;
extern const char UTF_INV1_2;
extern const char UTF_INV2_1;
extern const char UTF_INV2_2;

class XMLParsingData
{
public:
    friend class XMLDocument;

    XMLParsingData(const char * initialString)
        : cursor(), initialString(initialString), currentString(initialString)
    {
    }

    void UpdateCursor(const XMLCursor & cursor);
    void Update(const char * newString, XMLEncoding encoding);

    const std::string InitialString() const
    {
        return initialString;
    }

    const std::string CurrentString() const
    {
        return currentString;
    }

    const XMLCursor & Cursor() const
    {
        return cursor;
    }

protected:
    XMLCursor cursor;
    const char * initialString;
    const char * currentString;
};

} // namespace XMLParser
