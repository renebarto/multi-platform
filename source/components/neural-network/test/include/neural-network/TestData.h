#pragma once

#include <string>
#include "osal/Path.h"

namespace Parser {
namespace Test {

extern std::string BuildLogPath(const std::string & componentName, const std::string & fileName);

class Data
{
public:
    static std::string _applicationName;
    static void ApplicationName(const std::string & applicationName) { _applicationName = applicationName; }
    static std::string ApplicationName();
    static const std::string ProjectName() { return "neural-network"; }

    static std::string TestDirectory()
    {
        return TEST_DATA_ROOT;
    }

    static std::string TestDirectoryParser() { return OSAL::Path::CombinePath(TestDirectory(), ProjectName()); }

    static std::string Empty() { return "empty.idl"; }
    static std::string LexerTest() { return "lexertest.idl"; }
    static std::string SingleNamespace() { return "single_namespace.idl"; }
    static std::string NestedNamespace() { return "nested_namespace.idl"; }
    static std::string NamespaceWithInterface() { return "namespace_with_interface.idl"; }
    static std::string NamespaceWithNestedInterface() { return "namespace_with_nested_interface.idl"; }
    static std::string IPlugin() { return "IPlugin.idl"; }
    static std::string IChannel() { return "IChannel.idl"; }
    static std::string IOCDM() { return "IOCDM.idl"; }
    static std::string InterfaceWithSimpleMethods() { return "interface_with_simple_methods.idl"; }
};

} // namespace Test
} // namespace Parser

