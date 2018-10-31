#include "unittest-cpp/UnitTestC++.h"

#include "osal/Pipe.h"
#include "osal/Util.h"

using namespace std;

namespace OSAL {
namespace Test {

class PipeTest : public UnitTestCpp::TestFixture
{
public:
};

TEST_SUITE(osal)
{

TEST_FIXTURE(PipeTest, ConstructDefault)
{
    Pipe pipe;
    EXPECT_EQ(InvalidHandle, pipe.ReadFD());
    EXPECT_EQ(InvalidHandle, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, Construct)
{
    FileHandle fd[2];
#if defined(WIN_MSVC)
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    EXPECT_NE(0, CreatePipe(&fd[0], &fd[1], nullptr, 0));
#elif defined(WIN_MINGW)
    EXPECT_NE(0, CreatePipe(&fd[0], &fd[1], nullptr, 0));
#else
    EXPECT_NE(InvalidHandle, Files::CreatePipe(fd));
#endif
    Pipe pipe(fd[0], fd[1]);
    EXPECT_EQ(fd[0], pipe.ReadFD());
// cppcheck-suppress arrayIndexOutOfBounds
    EXPECT_EQ(fd[1], pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, Create)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(InvalidHandle, pipe.ReadFD());
    EXPECT_NE(InvalidHandle, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, Close)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(InvalidHandle, pipe.ReadFD());
    EXPECT_NE(InvalidHandle, pipe.WriteFD());
    pipe.Close();
    EXPECT_EQ(InvalidHandle, pipe.ReadFD());
    EXPECT_EQ(InvalidHandle, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, CloseRead)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(InvalidHandle, pipe.ReadFD());
    EXPECT_NE(InvalidHandle, pipe.WriteFD());
    pipe.CloseRead();
    EXPECT_EQ(InvalidHandle, pipe.ReadFD());
    EXPECT_NE(InvalidHandle, pipe.WriteFD());
}

TEST_FIXTURE(PipeTest, CloseWrite)
{
    Pipe pipe;
    pipe.Create();
    EXPECT_NE(InvalidHandle, pipe.ReadFD());
    EXPECT_NE(InvalidHandle, pipe.WriteFD());
    pipe.CloseWrite();
    EXPECT_NE(InvalidHandle, pipe.ReadFD());
    EXPECT_EQ(InvalidHandle, pipe.WriteFD());
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
    const ssize_t size = sizeof(reference);
    uint8_t actual[size];
    EXPECT_EQ(size, pipe.Write(reference, size));
    EXPECT_EQ(size, pipe.Read(actual, size));
    Util::Compare(reference, actual, size);
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace OSAL
