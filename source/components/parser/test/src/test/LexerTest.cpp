#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "parser/Lexer.h"
#include "parser/TestData.h"

using namespace std;

namespace Parser {
namespace Test {

class LexerTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(parser) {

LexerRuleSet Rules({
    LexerRule::CreateStartLiteral("//", LexerState::LineComment, false),
    LexerRule::CreateEndCharSet("\n", LexerState::LineComment, LexerToken::Type::LineComment, true),
    LexerRule::CreateStartLiteral("/*", LexerState::BlockComment, false),
    LexerRule::CreateEndLiteral("*/", LexerState::BlockComment, LexerToken::Type::BlockComment, true),
    LexerRule::CreateContinueCharSet(" \t", LexerState::Any, LexerToken::Type::Whitespace, true),
    LexerRule::CreateSingleChar("\n", LexerToken::Type::NewLine),
    LexerRule::CreateSingleChar("[", LexerToken::Type::SquareBracketOpen),
    LexerRule::CreateSingleChar("]", LexerToken::Type::SquareBracketClose),
    LexerRule::CreateSingleChar("{", LexerToken::Type::CurlyBraceOpen),
    LexerRule::CreateSingleChar("}", LexerToken::Type::CurlyBraceClose),
    LexerRule::CreateSingleChar("(", LexerToken::Type::ParenthesisOpen),
    LexerRule::CreateSingleChar(")", LexerToken::Type::ParenthesisClose),
    LexerRule::CreateSingleChar("*", LexerToken::Type::Asterisk),
    LexerRule::CreateSingleChar(";", LexerToken::Type::Semicolon),
    LexerRule::CreateSingleChar(":", LexerToken::Type::Colon),
    LexerRule::CreateSingleChar("&", LexerToken::Type::Ampersand),
    LexerRule::CreateSingleChar(",", LexerToken::Type::Comma),
    LexerRule::CreateStartCharSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", LexerState::Identifier, true),
    LexerRule::CreateContinueCharSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789",
                                     LexerState::Identifier, LexerToken::Type::Identifier, true),
    LexerRule::CreateStartLiteral("0x", LexerState::HexNumber, true),
    LexerRule::CreateStartLiteral("0X", LexerState::HexNumber, true),
    LexerRule::CreateContinueCharSet("0123456789abcdefABCDEF", LexerState::HexNumber, LexerToken::Type::HexNumber, true),
    LexerRule::CreateStartCharSet("0", LexerState::OctNumber, true),
    LexerRule::CreateContinueCharSet("01234567", LexerState::OctNumber, LexerToken::Type::OctNumber, true),
    LexerRule::CreateStartCharSet("123456789", LexerState::DecNumber, true),
    LexerRule::CreateContinueCharSet("0123456789", LexerState::DecNumber, LexerToken::Type::DecNumber, true),
    LexerRule::CreateStartLiteral("\"", LexerState::String, false),
    LexerRule::CreateEndLiteral("\"", LexerState::String, LexerToken::Type::String, true),
});

TEST_FIXTURE(LexerTest, Empty)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::Empty()));
    ifstream stream(path);
    InputReader reader(path, stream);
    Lexer lexer(reader);

    lexer.SetRules(Rules);

    LexerToken token = lexer.ReadToken();
    EXPECT_EQ("", token.value);
    EXPECT_TRUE(FSA(path, 1, 1, 0) == token.location);
    EXPECT_TRUE(reader.IsEOF());
}

TEST_FIXTURE(LexerTest, SimpleTest)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::LexerTest()));
    ifstream stream(path);
    InputReader reader(path, stream);
    Lexer lexer(reader);

    lexer.SetRules(Rules);

    LexerToken token = lexer.ReadToken();
    EXPECT_EQ(LexerToken::Type::SquareBracketOpen, token.type);
    EXPECT_EQ("[", token.value);
    EXPECT_TRUE(FSA(path, 1, 1, 0) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::SquareBracketClose, token.type);
    EXPECT_EQ("]", token.value);
    EXPECT_TRUE(FSA(path, 1, 2, 1) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::CurlyBraceOpen, token.type);
    EXPECT_EQ("{", token.value);
    EXPECT_TRUE(FSA(path, 1, 3, 2) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::CurlyBraceClose, token.type);
    EXPECT_EQ("}", token.value);
    EXPECT_TRUE(FSA(path, 1, 4, 3) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Whitespace, token.type);
    EXPECT_EQ("  ", token.value);
    EXPECT_TRUE(FSA(path, 1, 5, 4) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::LineComment, token.type);
    EXPECT_EQ(" Line comment", token.value);
    EXPECT_TRUE(FSA(path, 1, 7, 6) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 1, 22, 21) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::ParenthesisOpen, token.type);
    EXPECT_EQ("(", token.value);
    EXPECT_TRUE(FSA(path, 2, 1, 22) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::ParenthesisClose, token.type);
    EXPECT_EQ(")", token.value);
    EXPECT_TRUE(FSA(path, 2, 2, 23) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Asterisk, token.type);
    EXPECT_EQ("*", token.value);
    EXPECT_TRUE(FSA(path, 2, 3, 24) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Semicolon, token.type);
    EXPECT_EQ(";", token.value);
    EXPECT_TRUE(FSA(path, 2, 4, 25) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Whitespace, token.type);
    EXPECT_EQ(" \t", token.value);
    EXPECT_TRUE(FSA(path, 2, 5, 26) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::BlockComment, token.type);
    EXPECT_EQ(" Block comment ", token.value);
    EXPECT_TRUE(FSA(path, 2, 7, 28) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 2, 26, 47) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Colon, token.type);
    EXPECT_EQ(":", token.value);
    EXPECT_TRUE(FSA(path, 3, 1, 48) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Comma, token.type);
    EXPECT_EQ(",", token.value);
    EXPECT_TRUE(FSA(path, 3, 2, 49) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 3, 3, 50) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Identifier, token.type);
    EXPECT_EQ("abcdefg", token.value);
    EXPECT_TRUE(FSA(path, 4, 1, 51) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 4, 8, 58) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::DecNumber, token.type);
    EXPECT_EQ("12345678", token.value);
    EXPECT_TRUE(FSA(path, 5, 1, 59) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 5, 9, 67) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::OctNumber, token.type);
    EXPECT_EQ("01234567", token.value);
    EXPECT_TRUE(FSA(path, 6, 1, 68) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 6, 9, 76) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::HexNumber, token.type);
    EXPECT_EQ("0x123456789aBcDeF", token.value);
    EXPECT_TRUE(FSA(path, 7, 1, 77) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 7, 18, 94) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::String, token.type);
    EXPECT_EQ("Hello!$#", token.value);
    EXPECT_TRUE(FSA(path, 8, 1, 95) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::NewLine, token.type);
    EXPECT_EQ("\n", token.value);
    EXPECT_TRUE(FSA(path, 8, 11, 105) == token.location);
    EXPECT_TRUE(reader.IsEOF());


    EXPECT_TRUE(FSA(path, 9, 1, 106) == reader.Location());
}

TEST_FIXTURE(LexerTest, UnsupportedCharacters)
{
    string path("Dummy");
    istringstream stream("#%^");
    InputReader reader(path, stream);
    Lexer lexer(reader);

    lexer.SetRules(Rules);

    LexerToken token = lexer.ReadToken();
    EXPECT_EQ(LexerToken::Type::None, token.type);
    EXPECT_EQ("#", token.value);
    EXPECT_TRUE(FSA(path, 1, 1, 0) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::None, token.type);
    EXPECT_EQ("%", token.value);
    EXPECT_TRUE(FSA(path, 1, 2, 1) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::None, token.type);
    EXPECT_EQ("^", token.value);
    EXPECT_TRUE(FSA(path, 1, 3, 2) == token.location);
    EXPECT_TRUE(reader.IsEOF());

    EXPECT_TRUE(FSA(path, 1, 3, 3) == reader.Location());
}

TEST_FIXTURE(LexerTest, ReadAheadToken)
{
    string path("Dummy");
    istringstream stream("123 456 789");
    InputReader reader(path, stream);
    Lexer lexer(reader);

    lexer.SetRules(Rules);

    LexerToken token = lexer.ReadAheadToken();
    EXPECT_EQ(LexerToken::Type::DecNumber, token.type);
    EXPECT_EQ("123", token.value);
    EXPECT_TRUE(FSA(path, 1, 1, 0) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::DecNumber, token.type);
    EXPECT_EQ("123", token.value);
    EXPECT_TRUE(FSA(path, 1, 1, 0) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Whitespace, token.type);
    EXPECT_EQ(" ", token.value);
    EXPECT_TRUE(FSA(path, 1, 4, 3) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadAheadToken();

    EXPECT_EQ(LexerToken::Type::DecNumber, token.type);
    EXPECT_EQ("456", token.value);
    EXPECT_TRUE(FSA(path, 1, 5, 4) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::DecNumber, token.type);
    EXPECT_EQ("456", token.value);
    EXPECT_TRUE(FSA(path, 1, 5, 4) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Whitespace, token.type);
    EXPECT_EQ(" ", token.value);
    EXPECT_TRUE(FSA(path, 1, 8, 7) == token.location);
    EXPECT_FALSE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::DecNumber, token.type);
    EXPECT_EQ("789", token.value);
    EXPECT_TRUE(FSA(path, 1, 9, 8) == token.location);
    EXPECT_TRUE(reader.IsEOF());

    token = lexer.ReadToken();

    EXPECT_EQ(LexerToken::Type::Eof, token.type);
    EXPECT_EQ("", token.value);
    EXPECT_TRUE(FSA(path, 1, 11, 11) == token.location);
    EXPECT_TRUE(reader.IsEOF());

    EXPECT_TRUE(FSA(path, 1, 11, 11) == reader.Location());
}

} // TEST_SUITE(parser)

} // namespace Test
} // namespace Parser
