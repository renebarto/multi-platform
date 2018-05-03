#pragma once

#include <string>
#include "osal/Path.h"

namespace Core {
namespace Test {

extern std::string BuildLogPath(const std::string & componentName, const std::string & fileName);

class Data
{
public:
    static std::string _applicationName;
    static void ApplicationName(const std::string & applicationName) { _applicationName = applicationName; }
    static std::string ApplicationName();
    static const std::string ProjectName() { return "core"; }

    static std::string TestDirectory()
    {
        return TEST_DATA_ROOT;
    }

    static std::string TestDirectoryOSAL() { return OSAL::Path::CombinePath(TestDirectory(), ProjectName()); }

    static std::string FilledDirName() { return "FilledDir"; }
    static std::string FilledDirPath() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), FilledDirName()); }
    static std::string DummyDirName() { return "Dummy"; }
    static std::string DummyDirPath() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), DummyDirName()); }

    static std::string RegularFileNameNoExtension() { return "a"; }
    static std::string RegularFileExtension() { return ".tst"; }
    static std::string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static std::string RegularFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static std::string DummyFileName() { return "dummy.txt"; }
    static std::string DummyFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace Core

