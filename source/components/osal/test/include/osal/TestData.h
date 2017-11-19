#pragma once

#include <string>
#include "osal/Path.h"

namespace OSAL {
namespace Test {

extern String BuildLogPath(const String & componentName, const String & fileName);

class Data
{
public:
    static const String ProjectName() { return _("osal"); }

    static String TestDirectory()
    {
        return ToString(TEST_DATA_ROOT);
    }

    static String TestDirectoryOSAL() { return Path::CombinePath(TestDirectory(), ProjectName()); }
    static String TestLogDirectoryOSAL() { return Path::CombinePath(Path::CurrentDir(), _("log")); }
    static String LogFileRef() { return Path::CombinePath(TestDirectoryOSAL(), _("log.File.ref.txt")); }
    static String LogStreamRef() { return Path::CombinePath(TestDirectoryOSAL(), _("log.Stream.ref.txt")); }

    static String FilledDirName() { return _("FilledDir"); }
    static String FilledDirPath() { return Path::CombinePath(TestDirectoryOSAL(), FilledDirName()); }
    static String TstSubDirName() { return _("TstSubDir"); }
    static String TstSubDirPath() { return Path::CombinePath(FilledDirPath(), TstSubDirName()); }
    static String TstSubSubDirName() { return _("TstSubSubDir"); }
    static String TstSubSubDirPath() { return Path::CombinePath(TstSubDirPath(), TstSubSubDirName()); }
    static String EmptyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), _("EmptyDir")); }
    static String HiddenDirPath() { return Path::CombinePath(FilledDirPath(), _("HiddenDir")); }
    static String DummyDirName() { return _("Dummy"); }
    static String DummyDirPath() { return Path::CombinePath(TestDirectoryOSAL(), DummyDirName()); }

    static String RegularFileNameNoExtension() { return _("a"); }
    static String RegularFileExtension() { return _(".tst"); }
    static String RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static String RegularFilePath() { return Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static String HiddenFileName() { return _(".hidden.txt"); }
    static String HiddenFilePath() { return Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static String DummyFileName() { return _("dummy.txt"); }
    static String DummyFilePath() { return Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test
} // namespace OSAL

