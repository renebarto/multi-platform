#pragma once

#include <osal/Console.h>
#include <parse-c++/Parser.h>
#include <staruml/Document.h>

class ModelGenerator
{
public:
    ModelGenerator(OSAL::Console & console);

    void GenerateObjectsForAST(const CPPParser::ASTCollection & ast, StarUML::Container::Ptr container);
    void GenerateObjectsForNamespace(const CPPParser::Namespace::Ptr namespaces, StarUML::Container::Ptr container);
    StarUML::Container::Ptr AddPackageForNamespace(CPPParser::Namespace::Ptr, StarUML::Container::Ptr container);
    StarUML::Container::Ptr AddClass(CPPParser::Class::Ptr, StarUML::Container::Ptr container);
    StarUML::Container::Ptr AddStruct(CPPParser::Struct::Ptr, StarUML::Container::Ptr container);
    void Run(const CPPParser::Parser & parser, StarUML::Document & document);

private:
    OSAL::Console & _console;
};