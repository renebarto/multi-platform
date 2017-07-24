#pragma once

#include "xml/XMLVisitor.h"

namespace XMLParser
{

namespace Test
{

class XMLTestVisitor : public XMLVisitor
{
public:
    bool visitedByDocumentEnter;
    bool visitedByDocumentExit;
    int visitedByElementEnter;
    int visitedByElementExit;
    bool visitedByComment;
    bool visitedByDeclaration;
    bool visitedByText;
    bool visitedByUnknown;

    XMLTestVisitor()
        : visitedByDocumentEnter(false)
        , visitedByDocumentExit(false)
        , visitedByElementEnter(0)
        , visitedByElementExit(0)
        , visitedByComment(false)
        , visitedByDeclaration(false)
        , visitedByText(false)
        , visitedByUnknown(false)
    {
    }
    virtual ~XMLTestVisitor()
    {
    }

    virtual bool VisitEnter(const XMLDocument &)
    {
        visitedByDocumentEnter = true;
        return true;
    }

    virtual bool VisitExit(const XMLDocument &)
    {
        visitedByDocumentExit = true;
        return true;
    }

    virtual bool VisitEnter(const XMLElement &, const XMLAttribute::List &)
    {
        ++visitedByElementEnter;
        return true;
    }

    virtual bool VisitExit(const XMLElement &)
    {
        ++visitedByElementExit;
        return true;
    }

    virtual bool Visit(const XMLComment &)
    {
        visitedByComment = true;
        return true;
    }

    virtual bool Visit(const XMLDeclaration &)
    {
        visitedByDeclaration = true;
        return true;
    }

    virtual bool Visit(const XMLText &)
    {
        visitedByText = true;
        return true;
    }

    virtual bool Visit(const XMLUnknown &)
    {
        visitedByUnknown = true;
        return true;
    }
};

} // namespace Test

} // namespace XMLParser
