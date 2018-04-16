#include "regex/Tokenizer.h"

#include "regex/InputReader.h"

using namespace std;
using namespace Regex;

static const set<char> LiteralSet({'!', '"', '#', '$', '%', '&', '\'', ',',
                                   '/', '0', '1', '2', '3', '4', '5', '6',
                                   '7', '8', '9', ':', ';', '<', '=', '>',
                                   '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                   'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
                                   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                                   'X', 'Y', 'Z', '_', '`', 'a', 'b', 'c',
                                   'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                                   'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                                   't', 'u', 'v', 'w', 'x', 'y', 'z', '~'
                                  });

Tokenizer::Tokenizer(InputReader & reader)
    : _tokens()
    , _reader(&reader)
{
}

bool Tokenizer::Tokenize()
{
    Token currentToken;

    while (ReadToken(currentToken))
    {
        _tokens.push_back(currentToken);
//        cout << currentToken << endl;
    }
//    cout << currentToken << endl;
    return _reader->AtEnd();
}

bool Tokenizer::ReadToken(Token & token)
{
    if (_reader->AtEnd())
        return false;
    char ch;
    if (!_reader->PeekChar(ch))
        return false;
    switch (ch)
    {
        case '[':
            token = Token(TokenType::SquareBracketOpen, ch); _reader->Advance(); return true;
        case ']':
            token = Token(TokenType::SquareBracketClose, ch); _reader->Advance(); return true;
        case '{':
            token = Token(TokenType::CurlyBraceOpen, ch); _reader->Advance(); return true;
        case '}':
            token = Token(TokenType::CurlyBraceClose, ch); _reader->Advance(); return true;
        case '(':
            token = Token(TokenType::ParenthesisOpen, ch); _reader->Advance(); return true;
        case ')':
            token = Token(TokenType::ParenthesisClose, ch); _reader->Advance(); return true;
        case '.':
            token = Token(TokenType::Dot, ch); _reader->Advance(); return true;
        case '*':
            token = Token(TokenType::Asterisk, ch); _reader->Advance(); return true;
        case '+':
            token = Token(TokenType::Plus, ch); _reader->Advance(); return true;
        case '?':
            token = Token(TokenType::QuestionMark, ch); _reader->Advance(); return true;
        case '-':
            token = Token(TokenType::Dash, ch); _reader->Advance(); return true;
        case '|':
            token = Token(TokenType::Or, ch); _reader->Advance(); return true;
        case '^':
            token = Token(TokenType::Caret, ch); _reader->Advance(); return true;
        case ' ':
            token = Token(TokenType::Space, ch); _reader->Advance(); return true;
        case '\t':
            token = Token(TokenType::Tab, ch); _reader->Advance(); return true;
        case '\n':
            token = Token(TokenType::NewLine, ch); _reader->Advance(); return true;
        case '\r':
            token = Token(TokenType::Return, ch); _reader->Advance(); return true;
    }
    if (LiteralSet.find(ch) != LiteralSet.end())
    {
        token = Token(TokenType::Literal, ch);
        _reader->Advance();
        return true;
    }
    if (ch == '\\')
    {
        _reader->Advance();
        if (!_reader->GetChar(ch))
            return false;
        switch (ch)
        {
            case 'd':
                token = Token(TokenType::Digit);
                return true;
            case 'D':
                token = Token(TokenType::NotDigit);
                return true;
            case 'w':
                token = Token(TokenType::WordChar);
                return true;
            case 'W':
                token = Token(TokenType::NotWordChar);
                return true;
            case 'a':
                token = Token(TokenType::AlphaChar);
                return true;
            case 'A':
                token = Token(TokenType::NotAlphaChar);
                return true;
            case 's':
                token = Token(TokenType::WhitespaceChar);
                return true;
            case 'S':
                token = Token(TokenType::NotWhitespaceChar);
                return true;
            default:
                token = Token(TokenType::Literal, ch);
                return true;
        }
    }
    return false;
}