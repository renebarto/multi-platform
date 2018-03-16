#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "parser/InputReader.h"
#include "parser/TestData.h"

using namespace std;

namespace Parser {
namespace Test {

class InputReaderTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(parser) {

TEST_FIXTURE(InputReaderTest, Empty)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectory(), Data::Empty()));
    ifstream stream(path);
    InputReader reader(path, stream);

    EXPECT_EQ('\0', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 1, 0) == reader.Location());
    EXPECT_TRUE(reader.IsEOF());
}

TEST_FIXTURE(InputReaderTest, ReadChar)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::SingleNamespace()));
    ifstream stream(path);
    InputReader reader(path, stream);

    EXPECT_EQ('n', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 2, 1) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_EQ('m', reader.ReadChar());
    EXPECT_EQ('e', reader.ReadChar());
    EXPECT_EQ('s', reader.ReadChar());
    EXPECT_EQ('p', reader.ReadChar());
    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_EQ('c', reader.ReadChar());
    EXPECT_EQ('e', reader.ReadChar());
    EXPECT_EQ(' ', reader.ReadChar());
    EXPECT_EQ('W', reader.ReadChar());
    EXPECT_EQ('P', reader.ReadChar());
    EXPECT_EQ('E', reader.ReadChar());
    EXPECT_EQ('F', reader.ReadChar());
    EXPECT_EQ('r', reader.ReadChar());
    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_EQ('m', reader.ReadChar());
    EXPECT_EQ('e', reader.ReadChar());
    EXPECT_EQ('w', reader.ReadChar());
    EXPECT_EQ('o', reader.ReadChar());
    EXPECT_EQ('r', reader.ReadChar());
    EXPECT_EQ('k', reader.ReadChar());
    EXPECT_EQ(' ', reader.ReadChar());
    EXPECT_EQ('{', reader.ReadChar());
    EXPECT_EQ('\n', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 2, 1, 25) == reader.Location());
    EXPECT_EQ('\n', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 3, 1, 26) == reader.Location());
    EXPECT_EQ('}', reader.ReadChar());
    EXPECT_EQ(' ', reader.ReadChar());
    EXPECT_EQ('/', reader.ReadChar());
    EXPECT_EQ('/', reader.ReadChar());
    EXPECT_EQ(' ', reader.ReadChar());
    EXPECT_EQ('n', reader.ReadChar());
    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_EQ('m', reader.ReadChar());
    EXPECT_EQ('e', reader.ReadChar());
    EXPECT_EQ('s', reader.ReadChar());
    EXPECT_EQ('p', reader.ReadChar());
    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_EQ('c', reader.ReadChar());
    EXPECT_EQ('e', reader.ReadChar());
    EXPECT_EQ(' ', reader.ReadChar());
    EXPECT_EQ('W', reader.ReadChar());
    EXPECT_EQ('P', reader.ReadChar());
    EXPECT_EQ('E', reader.ReadChar());
    EXPECT_EQ('F', reader.ReadChar());
    EXPECT_EQ('r', reader.ReadChar());
    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_EQ('m', reader.ReadChar());
    EXPECT_EQ('e', reader.ReadChar());
    EXPECT_EQ('w', reader.ReadChar());
    EXPECT_EQ('o', reader.ReadChar());
    EXPECT_EQ('r', reader.ReadChar());
    EXPECT_EQ('k', reader.ReadChar());
    EXPECT_EQ('\n', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 4, 1, 54) == reader.Location());
    EXPECT_TRUE(reader.IsEOF());
    EXPECT_EQ('\0', reader.ReadChar());
    EXPECT_TRUE(reader.IsEOF());
}

TEST_FIXTURE(InputReaderTest, ReadChars)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::SingleNamespace()));
    ifstream stream(path);
    InputReader reader(path, stream);

    EXPECT_EQ("namespace", reader.ReadChars(9));
    EXPECT_TRUE(SourceLocation(path, 1, 10, 9) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ(" ", reader.ReadChars(1));
    EXPECT_TRUE(SourceLocation(path, 1, 11, 10) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ("WPEFramework", reader.ReadChars(12));
    EXPECT_TRUE(SourceLocation(path, 1, 23, 22) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ(" {\n", reader.ReadChars(3));
    EXPECT_TRUE(SourceLocation(path, 2, 1, 25) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ("\n", reader.ReadChars(1));
    EXPECT_TRUE(SourceLocation(path, 3, 1, 26) == reader.Location());
    EXPECT_EQ("} // namespace WPEFramework\n", reader.ReadChars(28));
    EXPECT_TRUE(SourceLocation(path, 4, 1, 54) == reader.Location());
    EXPECT_TRUE(reader.IsEOF());
    EXPECT_EQ("", reader.ReadChars(1));
    EXPECT_TRUE(reader.IsEOF());
}

TEST_FIXTURE(InputReaderTest, ReadAheadChar)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::SingleNamespace()));
    ifstream stream(path);
    InputReader reader(path, stream);

    char peek = reader.ReadAheadChar();
    EXPECT_EQ('n', peek);
    EXPECT_TRUE(SourceLocation(path, 1, 1, 0) == reader.Location());
    EXPECT_EQ('n', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 2, 1) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 3, 2) == reader.Location());
    peek = reader.ReadAheadChar();
    EXPECT_EQ('m', peek);
    EXPECT_TRUE(SourceLocation(path, 1, 3, 2) == reader.Location());
    EXPECT_EQ('m', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 4, 3) == reader.Location());
    while (reader.ReadChar() != '\0')
        ;
    EXPECT_TRUE(reader.IsEOF());
    peek = reader.ReadAheadChar();
    EXPECT_EQ('\0', peek);
    EXPECT_TRUE(reader.IsEOF());
}

TEST_FIXTURE(InputReaderTest, ReadAheadChars)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::SingleNamespace()));
    ifstream stream(path);
    InputReader reader(path, stream);

    string peek = reader.ReadAheadChars(9);
    EXPECT_EQ("namespace", peek);
    EXPECT_TRUE(SourceLocation(path, 1, 1, 0) == reader.Location());
    EXPECT_EQ('n', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 2, 1) == reader.Location());
    EXPECT_FALSE(reader.IsEOF());

    EXPECT_EQ('a', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 3, 2) == reader.Location());
    peek = reader.ReadAheadChars(20);
    EXPECT_EQ("mespace WPEFramework", peek);
    EXPECT_TRUE(SourceLocation(path, 1, 3, 2) == reader.Location());
    EXPECT_EQ('m', reader.ReadChar());
    EXPECT_TRUE(SourceLocation(path, 1, 4, 3) == reader.Location());
    while (reader.ReadChar() != '\0')
        ;
    EXPECT_TRUE(reader.IsEOF());
    peek = reader.ReadAheadChars(2);
    EXPECT_EQ("", peek);
    EXPECT_TRUE(reader.IsEOF());
}

} // TEST_SUITE(parser)

} // namespace Test
} // namespace Parser
