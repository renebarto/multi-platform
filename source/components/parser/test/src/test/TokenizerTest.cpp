#include "unittest-cpp/UnitTestC++.h"

#include <fstream>
#include "parser/Tokenizer.h"
#include "parser/TestData.h"

using namespace std;

namespace Parser {
namespace Test {

class TokenizerTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(parser) {

LexerRuleSet LexerRules({
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

static TokenizerRuleSet TokenizerRules
{
    TokenizerRule::CreateCombi(LexerToken::Type::Colon, LexerToken::Type::Colon, TokenType::ScopeResolution),
    TokenizerRule::CreateSimple(LexerToken::Type::None, TokenType::None),
    TokenizerRule::CreateSimple(LexerToken::Type::SquareBracketOpen, TokenType::SquareBracketOpen),
    TokenizerRule::CreateSimple(LexerToken::Type::SquareBracketClose, TokenType::SquareBracketClose),
    TokenizerRule::CreateSimple(LexerToken::Type::CurlyBraceOpen, TokenType::CurlyBraceOpen),
    TokenizerRule::CreateSimple(LexerToken::Type::CurlyBraceClose, TokenType::CurlyBraceClose),
    TokenizerRule::CreateSimple(LexerToken::Type::ParenthesisOpen, TokenType::ParenthesisOpen),
    TokenizerRule::CreateSimple(LexerToken::Type::ParenthesisClose, TokenType::ParenthesisClose),
    TokenizerRule::CreateSimple(LexerToken::Type::Asterisk, TokenType::Asterisk),
    TokenizerRule::CreateSimple(LexerToken::Type::Semicolon, TokenType::Semicolon),
    TokenizerRule::CreateSimple(LexerToken::Type::Colon, TokenType::Colon),
    TokenizerRule::CreateSimple(LexerToken::Type::Ampersand, TokenType::Ampersand),
    TokenizerRule::CreateSimple(LexerToken::Type::Comma, TokenType::Comma),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "namespace", TokenType::Namespace),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "interface", TokenType::Interface),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "id", TokenType::InterfaceID),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "in", TokenType::ParamIn),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "out", TokenType::ParamOut),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "inout", TokenType::ParamInOut),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "size_is", TokenType::ParamSizeIs),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "const", TokenType::QualifierConst),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "void", TokenType::VoidType),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "string", TokenType::StringType),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "sint8", TokenType::SInt8Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "sint16", TokenType::SInt16Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "sint32", TokenType::SInt32Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "sint64", TokenType::SInt64Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "uint8", TokenType::UInt8Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "uint16", TokenType::UInt16Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "uint32", TokenType::UInt32Type),
    TokenizerRule::CreateLookup(LexerToken::Type::Identifier, "uint64", TokenType::UInt64Type),
    // Fallback for unmatched identifiers
    TokenizerRule::CreateSimple(LexerToken::Type::Identifier, TokenType::Identifier),
    TokenizerRule::CreateSimple(LexerToken::Type::DecNumber, TokenType::DecNumber),
    TokenizerRule::CreateSimple(LexerToken::Type::OctNumber, TokenType::OctNumber),
    TokenizerRule::CreateSimple(LexerToken::Type::HexNumber, TokenType::HexNumber),
    TokenizerRule::CreateSimple(LexerToken::Type::String, TokenType::String),
    TokenizerRule::CreateSimple(LexerToken::Type::Whitespace, TokenType::Whitespace),
    TokenizerRule::CreateSimple(LexerToken::Type::NewLine, TokenType::NewLine),
    TokenizerRule::CreateSimple(LexerToken::Type::LineComment, TokenType::LineComment),
    TokenizerRule::CreateSimple(LexerToken::Type::BlockComment, TokenType::Whitespace),
    TokenizerRule::CreateEOF(),
};

TEST_FIXTURE(TokenizerTest, Empty)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::Empty()));
    ifstream stream(path);
    Tokenizer tokenizer(path, stream);
    tokenizer.SetLexerRules(LexerRules);
    tokenizer.SetRules(TokenizerRules);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    EXPECT_EQ(size_t {0}, tokens.size());
}

TEST_FIXTURE(TokenizerTest, SingleNamespace)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::SingleNamespace()));
    ifstream stream(path);
    Tokenizer tokenizer(path, stream);
    tokenizer.SetLexerRules(LexerRules);
    tokenizer.SetRules(TokenizerRules);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {11}, tokens.size());

    size_t index {};

    ASSERT_EQ(TokenType::Namespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ(string("WPEFramework"), tokens[index++].value);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::LineComment, tokens[index].type);
    ASSERT_EQ(string(" namespace WPEFramework"), tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
}

TEST_FIXTURE(TokenizerTest, IPlugin)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::IPlugin()));
    ifstream stream(path);
    Tokenizer tokenizer(path, stream);
    tokenizer.SetLexerRules(LexerRules);
    tokenizer.SetRules(TokenizerRules);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {123}, tokens.size());

    size_t index {};

    ASSERT_EQ(TokenType::Namespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ(string("WPEFramework"), tokens[index++].value);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Namespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ(string("PluginHost"), tokens[index++].value);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000020", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IPlugin", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Initialize", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("PluginHost", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IShell", tokens[index++].value);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("shell", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Deinitialize", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("PluginHost", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IShell", tokens[index++].value);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("shell", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Information", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000021", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IPlugin", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("INotification", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("StateChange", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("PluginHost", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IShell", tokens[index++].value);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("plugin", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::LineComment, tokens[index].type);
    ASSERT_EQ(string(" namespace PluginHost"), tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::LineComment, tokens[index].type);
    ASSERT_EQ(string(" namespace WPEFramework"), tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
}

TEST_FIXTURE(TokenizerTest, IChannel)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::IChannel()));
    ifstream stream(path);
    Tokenizer tokenizer(path, stream);
    tokenizer.SetLexerRules(LexerRules);
    tokenizer.SetRules(TokenizerRules);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {121}, tokens.size());

    size_t index {};

    ASSERT_EQ(TokenType::Namespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ(string("WPEFramework"), tokens[index++].value);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Namespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ(string("PluginHost"), tokens[index++].value);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000025", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IChannel", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Inbound", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ID", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("data", tokens[index++].value);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("length", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Outbound", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ID", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamOut, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("data", tokens[index++].value);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("length", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::LineComment, tokens[index].type);
    ASSERT_EQ(string(" namespace PluginHost"), tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::LineComment, tokens[index].type);
    ASSERT_EQ(string(" namespace WPEFramework"), tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
}

TEST_FIXTURE(TokenizerTest, IOCDM)
{
    string path(OSAL::Path::CombinePath(Data::TestDirectoryParser(), Data::IOCDM()));
    ifstream stream(path);
    Tokenizer tokenizer(path, stream);
    tokenizer.SetLexerRules(LexerRules);
    tokenizer.SetRules(TokenizerRules);

    ASSERT_TRUE(tokenizer.Tokenize());

    auto & tokens = tokenizer.GetTokens();
    ASSERT_EQ(size_t {481}, tokens.size());

    size_t index {};

    // namespace OCDM {
    ASSERT_EQ(TokenType::Namespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ(string("OCDM"), tokens[index++].value);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // [id(0x00000012)]
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000012", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // interface ISession
    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ISession", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    //sint32 Load();
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Load", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // void Update([in] const uint8* keyMessage, [in] const uint16 keyLength);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Update", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keyMessage", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keyLength", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // sint32 Remove();
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Remove", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // sint32 Close();
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("Close", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // [id(0x00000013)]
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000013", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // interface ISession::ICallback
    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ISession", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ICallback", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // void OnKeyMessage(
    //     [in] const uint8* keyMessage,
    //     [in] const uint16 keyLength,
    //     [in] const string URL);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("OnKeyMessage", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keyMessage", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keyLength", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("URL", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // void OnKeyReady();
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("OnKeyReady", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // void OnKeyError(
    //     [in] const sint16 error,
    //     [in] const sint32 sysError,
    //     [in] const string errorMessage);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("OnKeyError", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("error", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("sysError", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("errorMessage", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // void OnKeyStatusUpdate(const std::string keyMessage);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::VoidType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("OnKeyStatusUpdate", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);

    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keyMessage", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // [id(0x00000011)]
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000011", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // interface ISystem
    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ISystem", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // sint32 CreateSession(
    //     [in] const sint32 licenseType,
    //     [in] const string initDataType,
    //     [in] const uint8* initData,
    //     [in] const uint16 initDataLength,
    //     [in] const uint8* CDMData,
    //     [in] const uint16 CDMDataLength,
    //     [in] ISession::ICallback* callback,
    //     [out] string & sessionId,
    //     [out] string & bufferId,
    //     [out] ISession* & session);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("CreateSession", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("licenseType", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("initDataType", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("initData", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("initDataLength", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("CDMData", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("CDMDataLength", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ISession", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ICallback", tokens[index++].value);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("callback", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamOut, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Ampersand, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("sessionId", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamOut, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Ampersand, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("bufferId", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamOut, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ISession", tokens[index++].value);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Ampersand, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("session", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // sint32 SetServerCertificate(
    //     [in] const uint8* serverCertificate,
    //     [in] const uint16 serverCertificateLength);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("SetServerCertificate", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt8Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("serverCertificate", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::UInt16Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("serverCertificateLength", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // [id(0x00000010)]
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::InterfaceID, tokens[index++].type);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::HexNumber, tokens[index].type);
    ASSERT_EQ("0x00000010", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // interface IAccessorOCDM
    ASSERT_EQ(TokenType::Interface, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IAccessorOCDM", tokens[index++].value);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // sint32 IsTypeSupported(
    //     [in] const string keySystem,
    //     [in] const string mimeType) const;
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SInt32Type, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("IsTypeSupported", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keySystem", tokens[index++].value);
    ASSERT_EQ(TokenType::Comma, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("mimeType", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // OCDM::ISystem* KeySystem (
    //     [in] const string keySystem);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("OCDM", tokens[index++].value);
    ASSERT_EQ(TokenType::ScopeResolution, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("ISystem", tokens[index++].value);
    ASSERT_EQ(TokenType::Asterisk, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("KeySystem", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("keySystem", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    // string BufferId(
    //     [in] const string sessionId) const;
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("BufferId", tokens[index++].value);
    ASSERT_EQ(TokenType::ParenthesisOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketOpen, tokens[index++].type);
    ASSERT_EQ(TokenType::ParamIn, tokens[index++].type);
    ASSERT_EQ(TokenType::SquareBracketClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::StringType, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::Identifier, tokens[index].type);
    ASSERT_EQ("sessionId", tokens[index++].value);

    ASSERT_EQ(TokenType::ParenthesisClose, tokens[index++].type);
    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);
    ASSERT_EQ(TokenType::QualifierConst, tokens[index++].type);
    ASSERT_EQ(TokenType::Semicolon, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);
    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);

    ASSERT_EQ(TokenType::CurlyBraceClose, tokens[index++].type);

    ASSERT_EQ(TokenType::Whitespace, tokens[index++].type);

    ASSERT_EQ(TokenType::LineComment, tokens[index].type);
    ASSERT_EQ(string(" namespace OCDM"), tokens[index++].value);

    ASSERT_EQ(TokenType::NewLine, tokens[index++].type);
}

} // TEST_SUITE(parser)

} // namespace Test
} // namespace Parser
