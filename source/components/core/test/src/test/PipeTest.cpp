#include <unit-test-c++/UnitTestC++.h>

#include "core/Pipe.h"
#include "core/Util.h"

using namespace std;

namespace Core {
namespace Test {

TEST_SUITE(core) {

class PipeTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

};

void PipeTest::SetUp()
{
}

void PipeTest::TearDown()
{
}

TEST_FIXTURE(PipeTest, ConstructDefault)
{
    Pipe pipe;
    EXPECT_EQ(-1, pipe.ReadFD());
    EXPECT_EQ(-1, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, Construct)
{
    int fd[2];
    EXPECT_NE(-1, pipe(fd));
    Pipe pipe(fd[0], fd[1]);
    EXPECT_EQ(fd[0], pipe.ReadFD());
// cppcheck-suppress arrayIndexOutOfBounds
    EXPECT_EQ(fd[1], pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, Create)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(-1, pipe.ReadFD());
    EXPECT_NE(-1, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, Close)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(-1, pipe.ReadFD());
    EXPECT_NE(-1, pipe.WriteFD());
    pipe.Close();
    EXPECT_EQ(-1, pipe.ReadFD());
    EXPECT_EQ(-1, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, CloseRead)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(-1, pipe.ReadFD());
    EXPECT_NE(-1, pipe.WriteFD());
    pipe.CloseRead();
    EXPECT_EQ(-1, pipe.ReadFD());
    EXPECT_NE(-1, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, CloseWrite)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(-1, pipe.ReadFD());
    EXPECT_NE(-1, pipe.WriteFD());
    pipe.CloseWrite();
    EXPECT_NE(-1, pipe.ReadFD());
    EXPECT_EQ(-1, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, ReadWrite)
{
    Pipe pipe;
    pipe.Create();
    uint8_t reference[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    };
    ssize_t size = sizeof(reference);
    uint8_t actual[size];
    EXPECT_EQ(size, pipe.Write(reference, size));
    EXPECT_EQ(size, pipe.Read(actual, size));
    Util::Compare(reference, actual, size);
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
