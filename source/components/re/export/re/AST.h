#pragma once

#include <memory>
#include "re/Term.h"

namespace RE {

class ASTNode
{
public:
    using Ptr = std::shared_ptr<ASTNode>;
    enum Operation
    {
        Null,
        Leaf,
        Or,
        Concat,
    };

    Operation GetOperation() const { return _operation; }
    ASTNode::Ptr GetLeftNode() const { return _leftNode; }
    ASTNode::Ptr GetRightNode() const { return _rightNode; }
    const Term & GetTerm() const { return _term; }
    virtual void PrintTo(std::ostream & stream, int indent = 0) const = 0;

protected:
    ASTNode()
        : _operation()
        , _leftNode()
        , _rightNode()
        , _term()
    {}
    ASTNode(Operation operation, ASTNode::Ptr leftNode, ASTNode::Ptr rightNode, const Term & term)
        : _operation(operation)
        , _leftNode(leftNode)
        , _rightNode(rightNode)
        , _term(term)
    {}

    Operation _operation;
    ASTNode::Ptr _leftNode;
    ASTNode::Ptr _rightNode;
    Term _term;
};

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, RE::ASTNode::Operation operation)
{
    switch (operation)
    {
        case RE::ASTNode::Operation::Null:
            stream << "null"; break;
        case RE::ASTNode::Operation::Leaf:
            stream << ""; break;
        case RE::ASTNode::Operation::Or:
            stream << "|"; break;
        case RE::ASTNode::Operation::Concat:
            stream << "."; break;
        default:
            stream << "?"; break;
    }
    return stream;
}

namespace RE {

class ASTLeafNull : public ASTNode
{
public:
    ASTLeafNull()
        : ASTNode(Operation::Null, ASTNode::Ptr(), ASTNode::Ptr(), Term())
    {}

    static ASTNode::Ptr Create()
    {
        return std::make_shared<ASTLeafNull>();
    }
    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << std::string((indent + 1) * 4, ' ') << "empty" << std::endl;
    }
};

class ASTLeaf : public ASTNode
{
public:
    ASTLeaf(const Term & term)
        : ASTNode(Operation::Leaf, ASTNode::Ptr(), ASTNode::Ptr(), term)
    {}

    static ASTNode::Ptr Create(const Term & term)
    {
        return std::make_shared<ASTLeaf>(term);
    }
    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << std::string((indent + 1) * 4, ' ');
        _term.PrintTo(stream);
        stream << std::endl;
    }
};

class ASTUnaryOperation : public ASTNode
{
public:
    ASTUnaryOperation()
    {}

    static ASTNode::Ptr Create()
    {
        return std::make_shared<ASTUnaryOperation>();
    }

protected:
    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << _operation << "(";
        if (_leftNode != nullptr)
            _leftNode->PrintTo(stream, indent + 1);
        else
            stream << "null";
        stream << ")" << std::endl;
    }
};

class ASTBinaryOperation : public ASTNode
{
public:
    ASTBinaryOperation(Operation operation, ASTNode::Ptr leftNode, ASTNode::Ptr rightNode)
        : ASTNode(operation, leftNode, rightNode, Term())
    {}

    static ASTNode::Ptr Create(Operation operation, ASTNode::Ptr leftNode, ASTNode::Ptr rightNode)
    {
        return std::make_shared<ASTBinaryOperation>(operation, leftNode, rightNode);
    }

protected:
    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << std::string((indent + 1) * 4, ' ') << "operation: " << _operation << std::endl;
        if (_leftNode != nullptr)
            _leftNode->PrintTo(stream, indent + 1);
        else
            stream << "null";
        if (_rightNode != nullptr)
            _rightNode->PrintTo(stream, indent + 1);
        else
            stream << "null";
    }
};

class AST
{
public:
    AST()
        : _tree()
    {}

    bool IsEmpty() const { return Root() == nullptr; }
    ASTNode::Ptr Root() const
    {
        return _tree;
    }
    void Root(ASTNode::Ptr root)
    {
        _tree = root;
    }

    void PrintTo(std::ostream & stream, int indent = 0) const
    {
        if (_tree != nullptr)
            _tree->PrintTo(stream, indent);
        else
            stream << "Empty tree" << std::endl;
    }

protected:
    ASTNode::Ptr _tree;
};

} // namespace RE
