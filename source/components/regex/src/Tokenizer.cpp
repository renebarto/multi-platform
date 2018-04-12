#include "regex/Tokenizer.h"

#include "regex/InputReader.h"

using namespace std;
using namespace Regex;

Tokenizer::Tokenizer(InputReader & reader)
    : _tokens()
    , _reader(reader)
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
    return true;
}

bool Tokenizer::ReadToken(Token & token)
{
    if (_reader.AtEnd())
        return false;
    char ch;
    if (!_reader.GetChar(ch))
        return false;
    switch (ch)
    {
        case '[':
            token = Token(TokenType::SquareBracketOpen, ch); return true;
        case ']':
            token = Token(TokenType::SquareBracketClose, ch); return true;
        case '{':
            token = Token(TokenType::CurlyBraceOpen, ch); return true;
        case '}':
            token = Token(TokenType::CurlyBraceClose, ch); return true;
        case '(':
            token = Token(TokenType::ParenthesisOpen, ch); return true;
        case ')':
            token = Token(TokenType::ParenthesisClose, ch); return true;
        case '.':
            token = Token(TokenType::Dot, ch); return true;
        case '*':
            token = Token(TokenType::Asterisk, ch); return true;
        case '+':
            token = Token(TokenType::Plus, ch); return true;
        case '?':
            token = Token(TokenType::QuestionMark, ch); return true;
        case '-':
            token = Token(TokenType::Dash, ch); return true;
        case '|':
            token = Token(TokenType::Or, ch); return true;
        case '^':
            token = Token(TokenType::Caret, ch); return true;
        case ' ':
            token = Token(TokenType::Space, ch); return true;
        case '\t':
            token = Token(TokenType::Tab, ch); return true;
        case '\n':
            token = Token(TokenType::NewLine, ch); return true;
        case '\r':
            token = Token(TokenType::Return, ch); return true;
    }
    if (isalnum(ch) || (ch == '_'))
    {
        token = Token(TokenType::Literal, ch);
        return true;
    }
    if (ch == '\\')
    {
        if (!_reader.GetChar(ch))
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