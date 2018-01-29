#pragma once

#include <string>
#include "osal/Path.h"

namespace OSAL {
namespace Test {

extern string BuildLogPath(const string & componentName, const string & fileName);

class Data
{
public:
    static const string ProjectName() { return _("osal"); }

    static string TestDirectory()
    {
        return Tostring(TEST_DATA_ROOT);
    }

    static string TestDirectoryOSAL() { return Path::CombinePath(TestDirectory(), ProjectName()); }
    static string TestLogDirectoryOSAL() { return Path::CombinePath(Path::CurrentDir(), _("log")); }
    static string LogFileRef() { return Path::CombinePath(TestDirectoryOSAL(), _("log.File.ref.txt")); }
    static string LogStreamRef() { return Path::CombinePath(TestDirectoryOSAL(), _("log.Stream.ref.txt")); }

    static string FilledDirName() { return _("FilledDir"); }
    static string FilledDirPath() { return Path::CombinePath(TestDirectoryOSAL(), FilledDirName()); }
    static string TstSubDirName() { return _("TstSubDir"); }
    static string TstSubDirPath() { return Path::CombinePath(FilledDirPath(), TstSubDirName()); }
    static string TstSubSubDirName() { return _("TstSubSubDir"); }
    static string TstSubSubDirPath() { return Path::CombinePath(TstSubDirPath(), TstSubSubDirName()); }
    static string EmptyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), _("EmptyDir")); }
    static string HiddenDirPath() { return Path::CombinePath(FilledDirPath(), _("HiddenDir")); }
    static string DummyDirName() { return _("Dummy"); }
    static string DummyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), DummyDirName()); }

    static string RegularFileNameNoExtension() { return _("a"); }
    static string RegularFileExtension() { return _(".tst"); }
    static string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static string RegularFilePath() { return Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static string HiddenFileName() { return _(".hidden.txt"); }
    static string HiddenFilePath() { return Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static string DummyFileName() { return _("dummy.txt"); }
    static string DummyFilePath() { return Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace OSAL

