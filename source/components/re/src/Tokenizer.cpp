#include "re/Tokenizer.h"

#include "re/InputReader.h"

using namespace std;
using namespace RE;

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
            token = Token(TokenType::SquareBracketOpen); return true;
        case ']':
            token = Token(TokenType::SquareBracketClose); return true;
        case '{':
            token = Token(TokenType::CurlyBraceOpen); return true;
        case '}':
            token = Token(TokenType::CurlyBraceClose); return true;
        case '(':
            token = Token(TokenType::ParenthesisOpen); return true;
        case ')':
            token = Token(TokenType::ParenthesisClose); return true;
        case '.':
            token = Token(TokenType::Dot); return true;
        case '*':
            token = Token(TokenType::Asterisk); return true;
        case '+':
            token = Token(TokenType::Plus); return true;
        case '?':
            token = Token(TokenType::QuestionMark); return true;
        case '-':
            token = Token(TokenType::Dash); return true;
        case '|':
            token = Token(TokenType::Or); return true;
        case ' ':
        case '\t':
        {
            token = Token(TokenType::Whitespace);
            while (_reader.PeekChar(ch) && ((ch == ' ') || (ch == '\t')))
                _reader.Advance();
            return true;
        }
        case '\n':
        case '\r':
        {
            token = Token(TokenType::NewLine);
            while (_reader.PeekChar(ch) && ((ch == '\r') || (ch == '\n')))
                _reader.Advance();
            return true;
        }
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