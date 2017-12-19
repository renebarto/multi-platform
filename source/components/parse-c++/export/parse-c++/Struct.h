#pragma once

#include <clang-c/Index.h>
#include "parse-c++/Utility.h"
#include "parse-c++/Object.h"

using namespace Utility;

namespace CPPParser
{

class Struct : public Object
{
public:
    using Ptr = std::shared_ptr<Struct>;
    using List = std::vector<Ptr>;

    Struct() = delete;
    explicit Struct(Element::WeakPtr parent, SourceLocation sourceLocation, std::string name, AccessSpecifier accessSpecifier)
        : Object(std::move(parent), std::move(sourceLocation), std::move(name), accessSpecifier, AccessSpecifier::Public)
    {}

    virtual std::string QualifiedDescription() const override { return "struct " + QualifiedName(); }
    virtual std::string QualifiedName() const override
    {
        std::string result;
        if (Parent() != nullptr)
        {
            result = Parent()->QualifiedName() + "::";
        }
        return result + Name();
    }
    virtual bool TraverseBegin(IASTVisitor & visitor) const override
    {
        return visitor.Enter(*this);
    }
    virtual bool TraverseEnd(IASTVisitor & visitor) const override
    {
        return visitor.Leave(*this);
    }
    virtual bool Visit(IASTVisitor & visitor) const override
    {
        bool ok = true;
        if (!visitor.Enter(*this))
            ok = false;
        if (!VisitChildren(visitor))
            ok = false;
        if (!visitor.Leave(*this))
            ok = false;
        return ok;
    }
};

} // namespace CPPParser
