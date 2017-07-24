#pragma once

#include <string>
#include "osal/Path.h"

namespace OSAL {
namespace Test {

extern String BuildLogPath(const String & componentName, const String & fileName);

class Data
{
public:
    static const String ProjectName() { return "osal"; }
    static const String TestDirName() { return "testdata"; }

    static String TestDirectory()
    {
        static String RootDir = TEST_DATA_ROOT;
        return Path::CombinePath(Path::FullPath(RootDir),TestDirName());
    }

    static String TestDirectoryOSAL() { return Path::CombinePath(TestDirectory(), ProjectName()); }
    static String TestLogDirectoryOSAL() { return Path::CombinePath(Path::CurrentDir(), "log"); }
    static String LogFileRef() { return Path::CombinePath(TestDirectoryOSAL(), "log.File.ref.txt"); }
    static String LogStreamRef() { return Path::CombinePath(TestDirectoryOSAL(), "log.Stream.ref.txt"); }

    static String FilledDirName() { return "FilledDir"; }
    static String FilledDirPath() { return Path::CombinePath(TestDirectoryOSAL(), FilledDirName()); }
    static String TstSubDirName() { return "TstSubDir"; }
    static String TstSubDirPath() { return Path::CombinePath(FilledDirPath(), TstSubDirName()); }
    static String TstSubSubDirName() { return "TstSubSubDir"; }
    static String TstSubSubDirPath() { return Path::CombinePath(TstSubDirPath(), TstSubSubDirName()); }
    static String EmptyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), "EmptyDir"); }
    static String HiddenDirPath() { return Path::CombinePath(FilledDirPath(), "HiddenDir"); }
    static String DummyDirName() { return "Dummy"; }
    static String DummyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), DummyDirName()); }

    static String RegularFileNameNoExtension() { return "a"; }
    static String RegularFileExtension() { return ".tst"; }
    static String RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static String RegularFilePath() { return Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static String HiddenFileName() { return ".hidden.txt"; }
    static String HiddenFilePath() { return Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static String DummyFileName() { return "dummy.txt"; }
    static String DummyFilePath() { return Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace OSAL

