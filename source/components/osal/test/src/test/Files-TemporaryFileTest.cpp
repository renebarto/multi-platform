#include <unittest-cpp/UnitTestC++.h>

#include "osal/Exception.h"
#include "osal/Files.h"
#include "osal/TestData.h"

using namespace std;

namespace OSAL {
namespace Test {

TEST_SUITE(osal)
{

class TemporaryFileTest : public UnitTestCpp::TestFixture
{
public:
    void TearDown() override
    {
        Files::Remove(Test::Data::DummyFilePath().c_str());
    }
};

TEST_FIXTURE(TemporaryFileTest, Constructor)
{
    Files::TemporaryFile file;

    EXPECT_NOT_NULL(file.GetStream());
    EXPECT_NE(string {}, file.GetPath());
    EXPECT_EQ(size_t {0}, file.GetPath().find(Path::TempPath()));
}

TEST_FIXTURE(TemporaryFileTest, Close)
{
    Files::TemporaryFile file;

    file.Close();
    EXPECT_NULL(file.GetStream());
    EXPECT_NE(string {}, file.GetPath());
    EXPECT_EQ(size_t {0}, file.GetPath().find(Path::TempPath()));
}

TEST_FIXTURE(TemporaryFileTest, Destructor)
{
    string path;
    {
        Files::TemporaryFile file;
        path = file.GetPath();
    }

    EXPECT_FALSE(Path::FileExists(path));
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
