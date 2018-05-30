#pragma once

#include <string>
#include "osal/Path.h"

namespace Database {
namespace Test {

extern std::string BuildLogPath(const std::string & componentName, const std::string & fileName);

class Data
{
public:
    static std::string _applicationName;
    static void ApplicationName(const std::string & applicationName) { _applicationName = applicationName; }
    static std::string ApplicationName();
    static const std::string ProjectName() { return "database"; }

    static std::string TestDirectory()
    {
        return TEST_DATA_ROOT;
    }

    static std::string TestDirectoryDatabase() { return OSAL::Path::CombinePath(TestDirectory(), ProjectName()); }

    static const std::string DatabaseDummy() { return "dummy"; }
    static const std::string DatabaseImportSimpleRef() { return OSAL::Path::CombinePath(TestDirectoryDatabase(), "simple.sql"); }
    static const std::string DatabaseImportSimple2Ref() { return OSAL::Path::CombinePath(TestDirectoryDatabase(), "simple2Ref.sql"); }
    static const std::string DatabaseImportSimple2TableRef() { return OSAL::Path::CombinePath(TestDirectoryDatabase(), "simple2TableRef.sql"); }
    static const std::string DatabaseImportSimple2() { return OSAL::Path::CombinePath(TestDirectoryDatabase(), "simple2.sql"); }
    static const std::string DatabaseSimple() { return "simple"; }
    static const std::string DatabaseTmp() { return "tmp"; }
    static const std::string TableDummy() { return "dummy"; }
    static const std::string TableSimple() { return "table1"; }
    static const std::string TableTop40() { return "top 40"; }
    static const std::string ViewDummy() { return "dummy"; }
    static const std::string ViewTop40() { return "count by artist"; }
    static const std::string HtmlSource() { return OSAL::Path::CombinePath(TestDirectoryDatabase(), "wk01.htm"); }
    static const std::string HtmlTableSpec() { return "Nederlandse Top 401"; }
    static const std::string Location() { return "localhost"; }
    static const std::string Userid() { return "root"; }
    static const std::string Password() { return "R@qazwsx2"; }
};

} // namespace Test
} // namespace Database

