#include "unittest-cpp/UnitTestC++.h"

#include "re/Tokenizer.h"
#include "re/TestData.h"

using namespace std;

namespace RE {
namespace Test {

class TokenizerTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(parser) {

TEST_FIXTURE(TokenizerTest, Empty)
{
    InputReader reader;
    Tokenizer tokenizer(reader);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    EXPECT_EQ(size_t {0}, tokens.size());
}

TEST_FIXTURE(TokenizerTest, Punctuations)
{
    InputReader reader("[]{}()*+?-.| \t\n\r \r\n");
    Tokenizer tokenizer(reader);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {16}, tokens.size());

    size_t index {};

    ASSERT_EQ(Token(TokenType::SquareBracketOpen), tokens[index++]);
    ASSERT_EQ(Token(TokenType::SquareBracketClose), tokens[index++]);
    ASSERT_EQ(Token(TokenType::CurlyBraceOpen), tokens[index++]);
    ASSERT_EQ(Token(TokenType::CurlyBraceClose), tokens[index++]);
    ASSERT_EQ(Token(TokenType::ParenthesisOpen), tokens[index++]);
    ASSERT_EQ(Token(TokenType::ParenthesisClose), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Asterisk), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Plus), tokens[index++]);
    ASSERT_EQ(Token(TokenType::QuestionMark), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Dash), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Dot), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Or), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Whitespace), tokens[index++]);
    ASSERT_EQ(Token(TokenType::NewLine), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Whitespace), tokens[index++]);
    ASSERT_EQ(Token(TokenType::NewLine), tokens[index++]);
}

TEST_FIXTURE(TokenizerTest, LiteralChars)
{
    InputReader reader("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
    Tokenizer tokenizer(reader);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {63}, tokens.size());

    size_t index {};

    ASSERT_EQ(Token(TokenType::Literal, 'a'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'b'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'c'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'd'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'e'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'f'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'g'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'h'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'i'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'j'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'k'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'l'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'm'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'n'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'o'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'p'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'q'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'r'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 's'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 't'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'u'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'v'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'w'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'x'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'y'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'z'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'A'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'B'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'C'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'D'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'E'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'F'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'G'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'H'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'I'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'J'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'K'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'L'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'M'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'N'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'O'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'P'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'Q'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'R'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'S'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'T'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'U'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'V'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'W'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'X'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'Y'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'Z'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '0'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '1'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '2'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '3'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '4'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '5'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '6'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '7'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '8'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '9'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '_'), tokens[index++]);
}

TEST_FIXTURE(TokenizerTest, EscapeChars)
{
    InputReader reader("\\a\\b\\c\\d\\e\\f\\g\\h\\i\\j\\k\\l\\m\\n\\o\\p\\q\\r\\s\\t\\u\\v\\w\\x\\y\\z"
                       "\\A\\B\\C\\D\\E\\F\\G\\H\\I\\J\\K\\L\\M\\N\\O\\P\\Q\\R\\S\\T\\U\\V\\W\\X\\Y\\Z"
                       "\\0\\1\\2\\3\\4\\5\\6\\7\\8\\9\\_\\\\\\/\\[\\]\\{\\}\\(\\)\\.\\*\\+\\?\\-\\|");
    Tokenizer tokenizer(reader);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {77}, tokens.size());

    size_t index {};

    ASSERT_EQ(Token(TokenType::Literal, 'a'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'b'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'c'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Digit), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'e'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'f'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'g'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'h'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'i'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'j'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'k'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'l'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'm'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'n'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'o'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'p'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'q'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'r'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::WhitespaceChar), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 't'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'u'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'v'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::WordChar), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'x'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'y'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'z'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'A'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'B'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'C'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::NotDigit), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'E'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'F'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'G'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'H'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'I'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'J'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'K'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'L'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'M'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'N'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'O'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'P'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'Q'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'R'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::NotWhitespaceChar), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'T'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'U'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'V'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::NotWordChar), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'X'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'Y'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, 'Z'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '0'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '1'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '2'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '3'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '4'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '5'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '6'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '7'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '8'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '9'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '_'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '\\'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '/'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '['), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, ']'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '{'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '}'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '('), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, ')'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '.'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '*'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '+'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '?'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '-'), tokens[index++]);
    ASSERT_EQ(Token(TokenType::Literal, '|'), tokens[index++]);
}

} // TEST_SUITE(parser)

} // namespace Test
} // namespace RE
