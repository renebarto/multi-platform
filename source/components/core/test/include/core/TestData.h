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

    static std::string SubDirName() { return "TstSubDir"; }
    static std::string FilledDirName() { return "FilledDir"; }
    static std::string FilledDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), FilledDirName()); }
    static std::string SymlinkDirNameNoExtension() { return "FilledDir"; }
    static std::string SymlinkDirExtension() { return ".link"; }
    static std::string SymlinkDirName() { return SymlinkDirNameNoExtension() + SymlinkDirExtension(); }
    static std::string SymlinkDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), SymlinkDirName()); }
    static std::string DummyDirName() { return "Dummy"; }
    static std::string DummyDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), DummyDirName()); }
    static std::string NonExistingDirName() { return "Dummy2"; }
    static std::string NonExistingDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), NonExistingDirName()); }
    static std::string HiddenDirName() { return ".hidden"; }
    static std::string HiddenDirPath() { return OSAL::Path::CombinePath(TestDirectoryCore(), HiddenDirName()); }

    static std::string RegularFileNameNoExtension() { return "a"; }
    static std::string RegularFileExtension() { return ".tst"; }
    static std::string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static std::string RegularFilePath() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static std::string RegularFile2NameNoExtension() { return "b"; }
    static std::string RegularFile2Extension() { return ".tst"; }
    static std::string RegularFile2Name() { return RegularFile2NameNoExtension() + RegularFile2Extension(); }
    static std::string RegularFile2Path() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFile2Name()); }
    static std::string RegularFile3NameNoExtension() { return "c"; }
    static std::string RegularFile3Extension() { return ".tst"; }
    static std::string RegularFile3Name() { return RegularFile3NameNoExtension() + RegularFile3Extension(); }
    static std::string RegularFile3Path() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFile3Name()); }
    static std::string RegularFile4NameNoExtension() { return "d"; }
    static std::string RegularFile4Extension() { return ".tst"; }
    static std::string RegularFile4Name() { return RegularFile4NameNoExtension() + RegularFile4Extension(); }
    static std::string RegularFile4Path() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFile4Name()); }
    static std::string RegularFile5NameNoExtension() { return "e"; }
    static std::string RegularFile5Extension() { return ".tst"; }
    static std::string RegularFile5Name() { return RegularFile5NameNoExtension() + RegularFile5Extension(); }
    static std::string RegularFile5Path() { return OSAL::Path::CombinePath(FilledDirPath(), OSAL::Path::CombinePath(SubDirName(), RegularFile5Name())); }
    static std::string RegularFile6NameNoExtension() { return "f"; }
    static std::string RegularFile6Extension() { return ".tst"; }
    static std::string RegularFile6Name() { return RegularFile6NameNoExtension() + RegularFile6Extension(); }
    static std::string RegularFile6Path() { return OSAL::Path::CombinePath(FilledDirPath(), OSAL::Path::CombinePath(SubDirName(), RegularFile6Name())); }
    static std::string RegularFileNonExistingNameNoExtension() { return "z"; }
    static std::string RegularFileNonExistingExtension() { return ".tst"; }
    static std::string RegularFileNonExistingName() { return RegularFileNonExistingNameNoExtension() + RegularFileNonExistingExtension(); }
    static std::string RegularFileNonExistingPath() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFileNonExistingName()); }
    static std::string RegularFileNonExisting2NameNoExtension() { return "y"; }
    static std::string RegularFileNonExisting2Extension() { return ".tst"; }
    static std::string RegularFileNonExisting2Name() { return RegularFileNonExisting2NameNoExtension() + RegularFileNonExisting2Extension(); }
    static std::string RegularFileNonExisting2Path() { return OSAL::Path::CombinePath(FilledDirPath(), RegularFileNonExisting2Name()); }
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

