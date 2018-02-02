#pragma once

#include <string>
#include "osal/Path.h"

namespace OSAL {
namespace Test {

extern std::string BuildLogPath(const std::string & componentName, const std::string & fileName);

class Data
{
public:
    static const std::string ProjectName() { return "osal"; }

    static std::string TestDirectory()
    {
        return TEST_DATA_ROOT;
    }

    static std::string TestDirectoryOSAL() { return Path::CombinePath(TestDirectory(), ProjectName()); }
//    static std::string TestLogDirectoryOSAL() { return Path::CombinePath(Path::CurrentDir(), "log"); }
//    static std::string LogFileRef() { return Path::CombinePath(TestDirectoryOSAL(), "log.File.ref.txt"); }
//    static std::string LogStreamRef() { return Path::CombinePath(TestDirectoryOSAL(), "log.Stream.ref.txt"); }

    static std::string FilledDirName() { return "FilledDir"; }
    static std::string FilledDirPath() { return Path::CombinePath(TestDirectoryOSAL(), FilledDirName()); }
//    static std::string TstSubDirName() { return "TstSubDir"; }
//    static std::string TstSubDirPath() { return Path::CombinePath(FilledDirPath(), TstSubDirName()); }
//    static std::string TstSubSubDirName() { return "TstSubSubDir"; }
//    static std::string TstSubSubDirPath() { return Path::CombinePath(TstSubDirPath(), TstSubSubDirName()); }
//    static std::string EmptyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), "EmptyDir"); }
//    static std::string HiddenDirPath() { return Path::CombinePath(FilledDirPath(), "HiddenDir"); }
    static std::string DummyDirName() { return "Dummy"; }
    static std::string DummyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), DummyDirName()); }

    static std::string RegularFileNameNoExtension() { return "a"; }
    static std::string RegularFileExtension() { return ".tst"; }
    static std::string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static std::string RegularFilePath() { return Path::CombinePath(FilledDirPath(), RegularFileName()); }
//    static std::string HiddenFileName() { return ".hidden.txt"; }
//    static std::string HiddenFilePath() { return Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static std::string DummyFileName() { return "dummy.txt"; }
    static std::string DummyFilePath() { return Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace OSAL

