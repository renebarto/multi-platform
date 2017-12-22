#include <staruml/Package.h>

#include <core/DefaultLogger.h>
#include <staruml/Class.h>
#include "ModelGenerator.h"

ModelGenerator::ModelGenerator(OSAL::Console & console)
    : _console(console)
{
}

void ModelGenerator::GenerateObjectsForAST(const CPPParser::ASTCollection & ast, StarUML::Container::Ptr container)
{
    for (auto const & aNamespace : ast.Namespaces())
    {
        GenerateObjectsForNamespace(aNamespace, container);
    }
}

void ModelGenerator::GenerateObjectsForNamespace(const CPPParser::Namespace::Ptr ns, StarUML::Container::Ptr container)
{
    auto package = AddPackageForNamespace(ns, container);
    for (auto const & aNamespace : ns->Namespaces())
    {
        GenerateObjectsForNamespace(aNamespace, package);
    }
    for (auto const & aClass : ns->Classes())
    {
        AddClass(aClass, package);
    }
    for (auto const & aStruct : ns->Structs())
    {
        AddStruct(aStruct, package);
    }
}

StarUML::Container::Ptr ModelGenerator::AddPackageForNamespace(CPPParser::Namespace::Ptr ns, StarUML::Container::Ptr container)
{
    _console << ns->QualifiedDescription() << endl;
    auto package = make_shared<StarUML::Package>(container, OSAL::GUID::Generate(), ns->Name());
    container->AddElement(package);
    return package;
}

StarUML::Container::Ptr ModelGenerator::AddClass(CPPParser::Class::Ptr ns, StarUML::Container::Ptr container)
{
    _console << ns->QualifiedDescription() << endl;
    auto aClass = make_shared<StarUML::Class>(container, OSAL::GUID::Generate(), ns->Name());
    container->AddElement(aClass);
    return aClass;
}

StarUML::Container::Ptr ModelGenerator::AddStruct(CPPParser::Struct::Ptr ns, StarUML::Container::Ptr container)
{
    _console << ns->QualifiedDescription() << endl;
    auto aClass = make_shared<StarUML::Class>(container, OSAL::GUID::Generate(), ns->Name());
    container->AddElement(aClass);
    return aClass;
}

void ModelGenerator::Run(const CPPParser::Parser & parser, StarUML::Document & document)
{
    const CPPParser::ASTCollection & ast = parser.GetASTCollection();
    _console << fgcolor(OSAL::ConsoleColor::Green);
    GenerateObjectsForAST(ast, document.GetProject()->GetFirstModel());

    _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
}