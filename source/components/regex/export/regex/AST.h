#pragma once

#include <memory>
#include "regex/Term.h"

namespace Regex {

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
        Expression,
    };

    Operation GetOperation() const { return _operation; }
    const std::vector<ASTNode::Ptr> & GetNodes() const { return _nodes; }
    const Term & GetTerm() const { return _term; }
    void UpdateTerm(int minCount, int maxCount) { _term.SetMinMaxCount(minCount, maxCount); }

    void Insert(ASTNode::Ptr node)
    {
        _nodes.push_back(node);
    }
    virtual void PrintTo(std::ostream & stream, int indent = 0) const = 0;

protected:
    ASTNode()
        : _operation()
        , _nodes()
        , _term()
    {}
    ASTNode(Operation operation, const Term & term)
        : _operation(operation)
        , _nodes()
        , _term(term)
    {
    }
    ASTNode(Operation operation, ASTNode::Ptr node, const Term & term)
        : _operation(operation)
        , _nodes()
        , _term(term)
    {
        Insert(node);
    }

    Operation _operation;
    std::vector<ASTNode::Ptr> _nodes;
    Term _term;
};

inline void PrintTo(const ASTNode::Operation & operation, std::ostream & stream)
{
    stream << operation;
}

} // namespace Regex

inline std::ostream & operator << (std::ostream & stream, Regex::ASTNode::Operation operation)
{
    switch (operation)
    {
        case Regex::ASTNode::Operation::Null:
            stream << "null"; break;
        case Regex::ASTNode::Operation::Leaf:
            stream << ""; break;
        case Regex::ASTNode::Operation::Or:
            stream << "|"; break;
        case Regex::ASTNode::Operation::Concat:
            stream << "."; break;
        case Regex::ASTNode::Operation::Expression:
            stream << "()"; break;
        default:
            stream << "?"; break;
    }
    return stream;
}

namespace Regex {

class ASTLeaf : public ASTNode
{
public:
    ASTLeaf(const Term & term)
        : ASTNode(Operation::Leaf, term)
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

class ASTOrOperation : public ASTNode
{
public:
    ASTOrOperation()
        : ASTNode(ASTNode::Operation::Or, Term())
    {}

    static ASTNode::Ptr Create()
    {
        return std::make_shared<ASTOrOperation>();
    }

    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << std::string((indent + 1) * 4, ' ') << "Operation " << _operation << std::endl;
        for (auto const & node : _nodes)
        {
            node->PrintTo(stream, indent + 1);
        }
    }
};

class ASTConcatOperation : public ASTNode
{
public:
    ASTConcatOperation()
        : ASTNode(ASTNode::Operation::Concat, Term())
    {}

    static ASTNode::Ptr Create()
    {
        return std::make_shared<ASTConcatOperation>();
    }

    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << std::string((indent + 1) * 4, ' ') << "Operation " << _operation << std::endl;
        for (auto const & node : _nodes)
        {
            node->PrintTo(stream, indent + 1);
        }
    }
};

class ASTExpression : public ASTNode
{
public:
    ASTExpression(Ptr subExpression)
        : ASTNode(Operation::Expression, Term())
    {
        Insert(subExpression);
    }

    static ASTNode::Ptr Create(Ptr subExpression)
    {
        return std::make_shared<ASTExpression>(subExpression);
    }
    void PrintTo(std::ostream & stream, int indent = 0) const override
    {
        stream << std::string((indent + 1) * 4, ' ') << "(" << std::endl;
        for (auto const & node : _nodes)
        {
            node->PrintTo(stream, indent + 1);
        }
        stream << std::string((indent + 1) * 4, ' ') << ")" << std::endl;
    }

private:
    Ptr _subExpression;
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

} // namespace Regex
