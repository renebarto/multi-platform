#pragma once

#include <string>
#include "osal/Path.h"

namespace OSAL {
namespace Test {

extern std::string BuildLogPath(const std::string & componentName, const std::string & fileName);

class Data
{
public:
    static std::string _applicationName;
    static void ApplicationName(const std::string & applicationName) { _applicationName = applicationName; }
    static std::string ApplicationName();
    static const std::string ProjectName() { return "osal"; }

    static std::string TestDirectory()
    {
        return TEST_DATA_ROOT;
    }

    static std::string TestDirectoryOSAL() { return OSAL::Path::CombinePath(TestDirectory(), ProjectName()); }
//    static std::string TestLogDirectoryOSAL() { return OSAL::Path::CombinePath(OSAL::Path::CurrentDir(), "log"); }
//    static std::string LogFileRef() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), "log.File.ref.txt"); }
//    static std::string LogStreamRef() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), "log.Stream.ref.txt"); }

    static std::string FilledDirName() { return "FilledDir"; }
    static std::string FilledDirPath() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), FilledDirName()); }
//    static std::string TstSubDirName() { return "TstSubDir"; }
//    static std::string TstSubDirPath() { return OSAL::Path::CombinePath(FilledDirPath(), TstSubDirName()); }
//    static std::string TstSubSubDirName() { return "TstSubSubDir"; }
//    static std::string TstSubSubDirPath() { return OSAL::Path::CombinePath(TstSubDirPath(), TstSubSubDirName()); }
//    static std::string EmptyDirPath() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), "EmptyDir"); }
//    static std::string HiddenDirPath() { return OSAL::Path::CombinePath(FilledDirPath(), "HiddenDir"); }
    static std::string DummyDirName() { return "Dummy"; }
    static std::string DummyDirPath() { return OSAL::Path::CombinePath(TestDirectoryOSAL(), DummyDirName()); }

    static std::string RegularFileNameNoExtension() { return "a"; }
    static std::string RegularFileExtension() { return ".tst"; }
    static std::string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static std::string RegularFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFileName()); }
//    static std::string HiddenFileName() { return ".hidden.txt"; }
//    static std::string HiddenFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static std::string DummyFileName() { return "dummy.txt"; }
    static std::string DummyFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace OSAL

