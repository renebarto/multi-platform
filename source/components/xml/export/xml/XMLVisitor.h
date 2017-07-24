#pragma once

#include "xml/XMLAttribute.h"
#include "xml/XMLBase.h"

/**
	Implements the interface to the "Visitor pattern" (see the Accept() method.)
	If you call the Accept() method, it requires being passed a XMLVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, Element)
	you will get called with a VisitEnter/VisitExit pair. Nodes that are always leaves
	are simply called with Visit().

	If you return 'true' from a Visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its siblings</b> will be Visited.

	All flavors of Visit methods have a default implementation that returns 'true' (continue
	visiting). You need to only override methods that are interesting to you.

	Generally Accept() is called on the XMLDocument, although all nodes support Visiting.

	You should never change the document from a callback.
*/

namespace XMLParser
{

class XMLDocument;

class XMLElement;

class XMLComment;

class XMLDeclaration;

class XMLText;

class XMLUnknown;

class XMLVisitor
{
public:
    virtual ~XMLVisitor()
    {
    }

    virtual bool VisitEnter(const XMLDocument &)
    {
        return true;
    }

    virtual bool VisitExit(const XMLDocument &)
    {
        return true;
    }

    virtual bool VisitEnter(const XMLElement &, const XMLAttribute::List &)
    {
        return true;
    }

    virtual bool VisitExit(const XMLElement &)
    {
        return true;
    }

    virtual bool Visit(const XMLComment &)
    {
        return true;
    }

    virtual bool Visit(const XMLDeclaration &)
    {
        return true;
    }

    virtual bool Visit(const XMLText &)
    {
        return true;
    }

    virtual bool Visit(const XMLUnknown &)
    {
        return true;
    }
};

} // namespace XMLParser
