#include "parse-c++/AST.h"

#include <fstream>
#include <iomanip>
#include <clang-c/Index.h>
#include "parse-c++/Utility.h"
#include "parse-c++/Container.h"
#include "parse-c++/TreeInfo.h"
#include "parse-c++/Namespace.h"
#include "parse-c++/CodeGenerator.h"

using namespace std;
using namespace Utility;

namespace CPPParser
{

AST::AST()
    : Container(WeakPtr(), SourceLocation(), "AST", AccessSpecifier::Invalid)
    , _stack()
    , _tokenLookupMap()
{
}

bool AST::TraverseBegin(IASTVisitor & visitor) const
{
    return visitor.Enter(*this);
}

bool AST::TraverseEnd(IASTVisitor & visitor) const
{
    return visitor.Leave(*this);
}

bool AST::Visit(IASTVisitor & visitor) const
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

void AST::Show(std::ostream & stream, int UNUSED(indent)) const
{
    TreeInfo treeInfo(stream);
    Visit(treeInfo);
}

void AST::GenerateCode(std::ostream & stream, int UNUSED(indent)) const
{
    CodeGenerator codeGenerator(stream);
    Visit(codeGenerator);
}

Declaration::Ptr AST::Find(CXCursor token) const
{
    auto it = _tokenLookupMap.find(token);
    Declaration::Ptr object = nullptr;
    if (it != _tokenLookupMap.end())
        object = it->second;
    return object;
}

void AST::AddToMap(CXCursor token, Declaration::Ptr object)
{
    _tokenLookupMap.insert({token, object});
}

Declaration::Ptr AST::AddNamespace(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Declaration::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    ssize_t parentPosition = _stack.Find(parentToken);
    if (parentPosition > 0)
    {
        // Our parent is in the stack and not on top
        CXCursor currentParentsChild = _stack.At(static_cast<size_t>(parentPosition - 1));
        if ((token.kind == currentParentsChild.kind) &&
            (name == ConvertString(clang_getCursorSpelling(currentParentsChild))))
        {
            // The top of the stack (after correction) is the same kind and has the same name, so this must be the same token
            object = Find(currentParentsChild);
        }
    }
    else if ((parentPosition < 0) && (_stack.Count() > 0))
    {
        // Our parent is not on the stack, we may be in the global namespace
        CXCursor currentTopOfStack = _stack.At(static_cast<size_t>(0));
        if ((token.kind == currentTopOfStack.kind) &&
            (name == ConvertString(clang_getCursorSpelling(currentTopOfStack))))
        {
            // The top of the stack (after correction) is the same kind and has the same name, so this must be the same token
            object = Find(currentTopOfStack);
        }
    }
    UpdateStack(token, parentToken);
    if (object == nullptr)
    {
        object = make_shared<Namespace>(parent, SourceLocation(token), name);
        Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
        if (parentContainer != nullptr)
        {
            parentContainer->Add(object);
        }
        else
        {
            Add(object);
        }
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddClass(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Class::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    object = make_shared<Class>(parent, SourceLocation(token), name, accessSpecifier);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (parentContainer != nullptr)
    {
        parentContainer->Add(object);
    } else
    {
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddStruct(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Struct::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    object = make_shared<Struct>(parent, SourceLocation(token), name, accessSpecifier);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (parentContainer != nullptr)
    {
        parentContainer->Add(object);
    } else
    {
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddConstructor(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Constructor>(parent, SourceLocation(token), name, accessSpecifier, parameters, flags);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        cerr << "Parent is not an object" << endl;
        return nullptr;
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddDestructor(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));

    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Destructor>(parent, SourceLocation(token), name, accessSpecifier, flags);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        cerr << "Parent is not an object" << endl;
        return nullptr;
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddMethod(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Method>(parent, SourceLocation(token), name, accessSpecifier, type, parameters, flags);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular function in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddDataMember(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getCursorType(token)));

    auto object = make_shared<DataMember>(parent, SourceLocation(token), name, accessSpecifier, type);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(object->Parent());
    if (parentObject != nullptr)
    {
        parentObject->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddEnum(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string underlyingType;
    CXType type = clang_getEnumDeclIntegerType(token);
    if (type.kind != CXType_UInt)
    {
        CXString strType = clang_getTypeSpelling(type);
        underlyingType = ConvertString(strType);
    }

    auto object = make_shared<Enum>(parent, SourceLocation(token), name, accessSpecifier, underlyingType);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (parentContainer != nullptr)
    {
        parentContainer->Add(object);
    }
    else
    {
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

void AST::AddEnumValue(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    Enum::Ptr parentEnum = dynamic_pointer_cast<Enum>(parent);
    if (parentEnum != nullptr)
    {
        parentEnum->AddValue(token);
    }
    else
    {
        cerr << "Parent is not an enum?" << endl;
        return;
    }
}

Declaration::Ptr AST::AddTypedef(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getTypedefDeclUnderlyingType(token)));

    auto object = make_shared<Typedef>(parent, SourceLocation(token), name, accessSpecifier, type);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddVariable(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    std::string type = ConvertString(clang_getTypeSpelling(clang_getCursorType(token)));

    auto object = make_shared<Variable>(parent, SourceLocation(token), name, accessSpecifier, type);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddFunction(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<Function>(parent, SourceLocation(token), name, type, parameters, flags);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

void AST::AddBaseClass(CXCursor token, CXCursor parentToken, Declaration::Ptr baseType)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    bool isVirtual = (clang_isVirtualBase(token) != 0);

    auto inheritance = make_shared<Inheritance>(parent, SourceLocation(token), name, accessSpecifier, baseType, isVirtual);
    Object::Ptr parentObject = dynamic_pointer_cast<Object>(parent);
    if (parentObject != nullptr)
    {
        parentObject->AddBase(inheritance);
    }
    else
    {
        if (parent == nullptr)
        {
            cerr << "Type is null. Type not supported yet?" << endl;
        }
        else
        {
            cerr << "Type is not an object" <<  parent->Name() << endl;
        }
        return;
    }
}

Declaration::Ptr AST::AddFunctionTemplate(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    CXType functionType = clang_getCursorType(token);
    CXType resultType = clang_getResultType(functionType);
    CXString resultTypeStr = clang_getTypeSpelling(resultType);
    std::string type = ConvertString(resultTypeStr);

    int numArguments = clang_Cursor_getNumArguments(token);
    ParameterList parameters;
    for (int i = 0; i < numArguments; ++i)
    {
        CXCursor parameterToken = clang_Cursor_getArgument(token, i);
        CXString parameterNameStr = clang_getCursorSpelling(parameterToken);
        CXType parameterTypeDecl = clang_getArgType(functionType, i);
        CXString parameterTypeStr = clang_getTypeSpelling(parameterTypeDecl);
        std::string parameterName = ConvertString(parameterNameStr);
        std::string parameterType = ConvertString(parameterTypeStr);

        parameters.emplace_back(parameterName, parameterType);
    }
    FunctionFlags flags {};
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isConst(token) != 0) ? FunctionFlags::Const : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isVirtual(token) != 0) ? FunctionFlags::Virtual : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isPureVirtual(token) != 0) ? (FunctionFlags::PureVirtual | FunctionFlags::Virtual) : 0));
    flags = static_cast<FunctionFlags>(flags | ((clang_CXXMethod_isStatic(token) != 0) ? FunctionFlags::Static : 0));

    auto object = make_shared<FunctionTemplate>(parent, SourceLocation(token), name, type, parameters, flags);
    Namespace::Ptr parentNamespace = dynamic_pointer_cast<Namespace>(object->Parent());
    if (parentNamespace != nullptr)
    {
        parentNamespace->Add(object);
    }
    else
    {
        // Regular type in global namespace
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

Declaration::Ptr AST::AddClassTemplate(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    ClassTemplate::Ptr object;
    std::string name = ConvertString(clang_getCursorSpelling(token));
    AccessSpecifier accessSpecifier = ConvertAccessSpecifier(clang_getCXXAccessSpecifier(token));
    object = make_shared<ClassTemplate>(parent, SourceLocation(token), name, accessSpecifier);
    Container::Ptr parentContainer = dynamic_pointer_cast<Container>(object->Parent());
    if (parentContainer != nullptr)
    {
        parentContainer->Add(object);
    } else
    {
        Add(object);
    }
    AddToMap(token, object);
    return object;
}

void AST::AddTemplateTypeParameter(CXCursor token, CXCursor parentToken)
{
    Declaration::Ptr parent = Find(parentToken);
    std::string name = ConvertString(clang_getCursorSpelling(token));
    FunctionTemplate::Ptr functionTemplate = dynamic_pointer_cast<FunctionTemplate>(parent);
    if (functionTemplate != nullptr)
    {
        functionTemplate->AddTemplateParameter(name);
        return;
    }
    ClassTemplate::Ptr classTemplate = dynamic_pointer_cast<ClassTemplate>(parent);
    if (classTemplate != nullptr)
    {
        classTemplate->AddTemplateParameter(name);
        return;
    }
    cerr << "Panic! No function or class template" << endl;
}

void AST::ShowInfo()
{
    cout << "AST stack contents:" << endl;
    for (size_t index = 0; index < _stack.Count(); ++index)
    {
        CXCursor element = _stack.At(_stack.Count() - index - 1);
        CXCursorKind kind = clang_getCursorKind(element);
        CXString strKind = clang_getCursorKindSpelling(kind);
        CXType type = clang_getCursorType(element);
        CXString strType = clang_getTypeSpelling(type);
        CXType underlyingType = clang_getTypedefDeclUnderlyingType(element);
        CXString strUnderlyingType = clang_getTypeSpelling(underlyingType);
        CXString strName = clang_getCursorSpelling(element);
        if (kind < CXCursorKind::CXCursor_FirstInvalid)
        {
            switch (type.kind)
            {
                case CXTypeKind::CXType_Invalid:
                    std::cout << setw(3) << index << " " << strKind << ": " << strName << std::endl;
                    break;
                case CXTypeKind::CXType_Typedef:
                    std::cout << setw(3) << index << " " << strKind << ": " << strName << ": " << strUnderlyingType << std::endl;
                    break;
                default:
                    std::cout << setw(3) << index << " " << strKind << ": " << strName << ": " << strType << std::endl;
                    break;
            }
        }
        else
        {
            std::cout << setw(3) << index << " " << strKind << ": " << strType << ": " << std::endl;
        }
    }
    cout << "AST token map:" << endl;
    for (auto element : _tokenLookupMap)
    {
        std::string token = ConvertString(clang_getCursorSpelling(element.first));
        cout << token << " @ " << SourceLocation(element.first) << endl;
//        cout << token << " : ";
//        TreeInfo treeInfo(cout);
//        element.second->TraverseBegin(treeInfo);
//        element.second->TraverseEnd(treeInfo);
    }
}

void AST::UpdateStack(CXCursor token, CXCursor parentToken)
{
    ssize_t index = _stack.Find(parentToken);
    if (index > 0)
    {
        // Make sure parent cursor is at top of stack, remove any others
        _stack.RemoveTopElements(index);
    }
    else if (index < 0)
    {
        // Parent is not on stack, clear stack completely
        // Make sure parent cursor is at top of stack, remove any others
        _stack.RemoveTopElements(_stack.Count());
    }
    _stack.Push(token);
}

} // namespace CPPParser
