#pragma once

#include <cassert>
#include "parse-c++/IASTVisitor.h"
#include "parse-c++/AST.h"
#include "parse-c++/Typedef.h"
#include "parse-c++/Inheritance.h"
#include "parse-c++/Enum.h"
#include "parse-c++/Function.h"
#include "parse-c++/Variable.h"
#include "parse-c++/Class.h"
#include "parse-c++/Struct.h"
#include "parse-c++/ClassTemplate.h"
#include "parse-c++/Namespace.h"
#include "parse-c++/PreprocessorDirectives.h"

using namespace std;

namespace CPPParser
{

class CodeGenerator : public IASTVisitor
{
public:
    explicit CodeGenerator(std::ostream & stream)
        : _stream(stream)
        , _indent()
    {
    }

    virtual bool Enter(const AST &) override
    {
//        TRACE(__func__, "AST");
        _indent = 0;
        return true;
    }
    virtual bool Leave(const AST &) override
    {
//        TRACE(__func__, "AST");
        return true;
    }

    virtual bool Enter(const ASTCollection &) override
    {
//        TRACE("ASTCollection");
        _indent = 0;
        return true;
    }
    virtual bool Leave(const ASTCollection &) override
    {
//        TRACE("ASTCollection");
        return true;
    }

    virtual bool Enter(const Typedef & element) override
    {
//        TRACE2(__func__, "Typedef", element.Name());
        _stream << Indent(_indent) << "typedef " << element.Type() << " " << element.Name() << ";" << std::endl;
        return true;
    }
    virtual bool Leave(const Typedef & UNUSED(element)) override
    {
//        TRACE2(__func__, "Typedef", element.Name());
        return true;
    }

    virtual bool Enter(const EnumConstant & element) override
    {
//        TRACE2(__func__, "EnumConstant", element.Name());
        _stream << Indent(_indent) << element.Name() << " = " << element.Value() << "," << std::endl;
        return true;
    }
    virtual bool Leave(const EnumConstant & UNUSED(element)) override
    {
//        TRACE2(__func__, "EnumConstant", element.Name());
        return true;
    }

    virtual bool Enter(const Enum & element) override
    {
//        TRACE2(__func__, "Enum", element.Name());
        _stream << Indent(_indent) << "enum "
                << (element.Name().empty() ? "" : element.Name() + " ");
        if (!element.Type().empty())
        {
            _stream << ": " << element.Type() << " ";
        }
        _stream << "{" << std::endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Enum & element) override
    {
//        TRACE2(__func__, "Enum", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // enum "
                << (element.Name().empty() ? "<anonymous>" : element.Name()) << std::endl;
        return true;
    }

    bool EnterFunctionBase(const FunctionBase & element)
    {
        // TODO: Move validations to IsValid()
        bool isObjectMember = (dynamic_pointer_cast<Object>(element.Parent()) != nullptr);
        assert(isObjectMember ||
               (!element.IsVirtual() && !element.IsConst() && !element.IsDefault() && !element.IsDeleted() &&
                !element.IsFinal() && !element.IsOverride() && !element.IsPureVirtual()));
        if (element.IsStatic())
            assert((!element.IsVirtual() && !element.IsDefault() && !element.IsDeleted() &&
                    !element.IsFinal() && !element.IsOverride() && !element.IsPureVirtual()));
        if (element.IsDefault() || element.IsDeleted())
            assert(!element.IsVirtual());
        assert(element.IsVirtual() || (!element.IsPureVirtual()));
        _stream << Indent(_indent);
        if (element.IsInline())
            _stream << "inline ";
        if (element.IsStatic())
            _stream << "static ";
        if (element.IsVirtual())
            _stream << "virtual ";
        if (!element.Type().empty())
            _stream << element.Type() << " ";
        _stream << element.Name() << "(";
        bool firstParameter = true;
        for (auto const & parameter : element.Parameters())
        {
            if (!firstParameter)
            {
                _stream << ", ";
            }
            _stream << parameter.Type() << " " << parameter.Name();
            firstParameter = false;
        }
        _stream << ")";
        if (element.IsConst())
            _stream << " const";
        if (element.IsOverride())
            _stream << " override";
        if (element.IsFinal())
            _stream << " override";
        if (element.IsPureVirtual())
            _stream << " = 0";
        if (element.IsDefault())
            _stream << " = default";
        if (element.IsDeleted())
            _stream << " = delete";
        _stream << ";" << std::endl;
        return true;
    }
    bool LeaveFunctionBase(const FunctionBase & UNUSED(element))
    {
        return true;
    }

    virtual bool Enter(const Constructor & element) override
    {
//        TRACE2(__func__, "Constructor", element.Name());
        return EnterFunctionBase(element);
    }
    virtual bool Leave(const Constructor & element) override
    {
//        TRACE2(__func__, "Constructor", element.Name());
        return LeaveFunctionBase(element);
    }

    virtual bool Enter(const Destructor & element) override
    {
//        TRACE2(__func__, "Destructor", element.Name());
        return EnterFunctionBase(element);
    }
    virtual bool Leave(const Destructor & element) override
    {
//        TRACE2(__func__, "Destructor", element.Name());
        return LeaveFunctionBase(element);
    }

    virtual bool Enter(const Method & element) override
    {
//        TRACE2(__func__, "Method", element.Name());
        return EnterFunctionBase(element);
    }
    virtual bool Leave(const Method & element) override
    {
//        TRACE2(__func__, "Method", element.Name());
        return LeaveFunctionBase(element);
    }

    virtual bool Enter(const Function & element) override
    {
//        TRACE2(__func__, "Function", element.Name());
        return EnterFunctionBase(element);
    }
    virtual bool Leave(const Function & element) override
    {
//        TRACE2(__func__, "Function", element.Name());
        return LeaveFunctionBase(element);
    }

    virtual bool Enter(const FunctionTemplate & element) override
    {
//        TRACE2(__func__, "FunctionTemplate", element.Name());
        _stream << Indent(_indent);
        _stream << "template<";
        bool firstTemplateParameter = true;
        for (auto const & parameter : element.TemplateParameters())
        {
            if (!firstTemplateParameter)
            {
                _stream << ", ";
            }
            _stream << "class " << parameter;
            firstTemplateParameter = false;
        }

        _stream << "> ";
        if (element.IsInline())
            _stream << "inline ";
        if (element.IsStatic())
            _stream << "static ";
        if (element.IsVirtual())
            _stream << "virtual ";
        if (!element.Type().empty())
            _stream << element.Type() << " ";
        _stream << element.Name() << "(";
        bool firstParameter = true;
        for (auto const & parameter : element.Parameters())
        {
            if (!firstParameter)
            {
                _stream << ", ";
            }
            _stream << parameter.Type() << " " << parameter.Name();
            firstParameter = false;
        }
        _stream << ")";
        if (element.IsConst())
            _stream << " const";
        if (element.IsOverride())
            _stream << " override";
        if (element.IsFinal())
            _stream << " override";
        if (element.IsPureVirtual())
            _stream << " = 0";
        if (element.IsDefault())
            _stream << " = default";
        if (element.IsDeleted())
            _stream << " = delete";
        _stream << ";" << std::endl;
        return true;
    }
    virtual bool Leave(const FunctionTemplate & UNUSED(element)) override
    {
//        TRACE2(__func__, "FunctionTemplate", element.Name());
        return true;
    }

    virtual bool Enter(const Variable & element) override
    {
//        TRACE2(__func__, "Variable", element.Name());
        _stream << Indent(_indent) << element.Type() << " " << element.Name() << ";" << endl;

        return true;
    }
    virtual bool Leave(const Variable & UNUSED(element)) override
    {
//        TRACE2(__func__, "Variable", element.Name());
        return true;
    }

    virtual bool Enter(const DataMember & element) override
    {
//        TRACE2(__func__, "DataMember", element.Name());
        _stream << Indent(_indent) << element.Type() << " " << element.Name() << ";" << endl;
        return true;
    }
    virtual bool Leave(const DataMember & UNUSED(element)) override
    {
//        TRACE2(__func__, "DataMember", element.Name());
        return true;
    }

    void ObjectInheritance(const Object & element)
    {
        if (!element.BaseTypes().empty())
        {
            bool firstBase = true;
            for (auto const & inheritance : element.BaseTypes())
            {
                if (firstBase)
                    _stream << " : ";
                else
                    _stream << ", ";

                if (inheritance->IsVirtual())
                    _stream << "virtual ";
                _stream << inheritance->Access() << " " << inheritance->Name();
                firstBase = false;
            }
        }
    }

    virtual bool Enter(const Class & element) override
    {
//        TRACE2(__func__, "Class", element.Name());
        _stream << Indent(_indent) << "class " << element.Name();
        ObjectInheritance(element);
        _stream << " {" << std::endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Class & element) override
    {
//        TRACE2(__func__, "Class", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // class " << element.Name() << std::endl;
        return true;
    }

    virtual bool Enter(const Struct & element) override
    {
//        TRACE2(__func__, "Struct", element.Name());
        _stream << Indent(_indent) << "struct " << element.Name();
        ObjectInheritance(element);
        _stream << " {" << std::endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Struct & element) override
    {
//        TRACE2(__func__, "Struct", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // struct " << element.Name() << std::endl;
        return true;
    }

    virtual bool Enter(const ClassTemplate & element) override
    {
//        TRACE2(__func__, "ClassTemplate", element.Name());
        _stream << Indent(_indent);
        _stream << "template<";
        bool firstTemplateParameter = true;
        for (auto const & parameter : element.TemplateParameters())
        {
            if (!firstTemplateParameter)
            {
                _stream << ", ";
            }
            _stream << "class " << parameter;
            firstTemplateParameter = false;
        }

        _stream << "> class " << element.Name();;
        ObjectInheritance(element);
        _stream << " {" << std::endl;
        ++_indent;
        return true;
    }

    virtual bool Leave(const ClassTemplate & element) override
    {
//        TRACE2(__func__, "ClassTemplate", element.Name());
        --_indent;
        _stream << Indent(_indent) << "}; // class " << element.Name() << std::endl;
        return true;
    }

    virtual bool Enter(const Namespace & element) override
    {
//        TRACE2(__func__, "Namespace", element.Name());
        _stream << Indent(_indent) << "namespace " << (element.Name().empty() ? "" : element.Name() + " ") << "{" << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const Namespace & element) override
    {
//        TRACE2(__func__, "Namespace", element.Name());
        --_indent;
        _stream << Indent(_indent) << "} // namespace " << (element.Name().empty() ? "<anonymous>" : element.Name()) << endl;
        return true;
    }

    virtual bool Enter(const IncludeDirective & element) override
    {
//        TRACE(__func__, "Include");
        _stream << Indent(_indent) << "#include "
                << (element.IncludeType() == IncludeSpecifier::Local ? '"' : '<')
                << element.Name()
                << (element.IncludeType() == IncludeSpecifier::Local ? '"' : '>')
                << endl;
        return true;
    }
    virtual bool Leave(const IncludeDirective & UNUSED(element)) override
    {
//        TRACE(__func__, "Include");
        return true;
    }

    virtual bool Enter(const IfdefDirective & element) override
    {
//        TRACE(__func__, "Ifdef");
        _stream << Indent(_indent) << "Ifdef "
                << element.Name()
                << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const IfdefDirective & UNUSED(element)) override
    {
//        TRACE(__func__, "Ifdef");
        --_indent;
        _stream << Indent(_indent) << "Endif"
                << endl;
        return true;
    }

    virtual bool Enter(const IfDirective & element) override
    {
//        TRACE(__func__, "If");
        _stream << Indent(_indent) << "If "
                << element.Name()
                << endl;
        ++_indent;
        return true;
    }
    virtual bool Leave(const IfDirective & UNUSED(element)) override
    {
//        TRACE(__func__, "If");
        --_indent;
        _stream << Indent(_indent) << "Endif"
                << endl;
        return true;
    }

    virtual bool Enter(const DefineDirective & element) override
    {
//        TRACE(__func__, "Define");
        _stream << Indent(_indent) << "Define "
                << element.Name()
                << endl;
        return true;
    }
    virtual bool Leave(const DefineDirective & UNUSED(element)) override
    {
//        TRACE(__func__, "Define");
        return true;
    }

    virtual bool Enter(const UndefDirective & element) override
    {
//        TRACE(__func__, "Undef");
        _stream << Indent(_indent) << "Undef "
                << element.Name()
                << endl;
        return true;
    }
    virtual bool Leave(const UndefDirective & UNUSED(element)) override
    {
//        TRACE(__func__, "Undef");
        return true;
    }

public:
    std::ostream & _stream;
    int _indent;
};

} // namespace CPPParser
