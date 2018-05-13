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

    static std::string TestDirectoryCore() { return OSAL::Path::CombinePath(TestDirectory(), ProjectName()); }

    static std::string FilledDirName() { return "FilledDir"; }
    static std::string FilledDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), FilledDirName()); }
    static std::string DummyDirName() { return "Dummy"; }
    static std::string DummyDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), DummyDirName()); }

    static std::string RegularFileNameNoExtension() { return "a"; }
    static std::string RegularFileExtension() { return ".tst"; }
    static std::string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static std::string RegularFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static std::string SymlinkFileNameNoExtension() { return "a"; }
    static std::string SymlinkFileExtension() { return ".link"; }
    static std::string SymlinkFileName() { return SymlinkFileNameNoExtension() + SymlinkFileExtension(); }
    static std::string SymlinkFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), SymlinkFileName()); }
    static std::string HiddenFileNameNoExtension() { return ".hidden"; }
    static std::string HiddenFileExtension() { return ".txt"; }
    static std::string HiddenFileName() { return HiddenFileNameNoExtension() + HiddenFileExtension(); }
    static std::string HiddenFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static std::string DummyFileNameNoExtension() { return "dummy"; }
    static std::string DummyFileExtension() { return ".txt"; }
    static std::string DummyFileName() { return DummyFileNameNoExtension() + DummyFileExtension(); }
    static std::string DummyFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace Core

