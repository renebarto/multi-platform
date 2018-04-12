#pragma once

#include <string>

namespace Regex {

class InputReader
{
public:
    InputReader()
        : _input()
        , _index()
    {}
    InputReader(const std::string & input)
        : _input(input)
        , _index()
    {}

    void SetInput(const std::string & input)
    {
        _input = input;
        _index = {};
    }
    void Advance()
    {
        if (!AtEnd())
            ++_index;
    }
    bool PeekChar(char & ch)
    {
        if (AtEnd())
            return false;
        ch = _input[_index];
        return true;
    }
    bool GetChar(char & ch)
    {
        if (AtEnd())
            return false;
        ch = _input[_index];
        Advance();
        return true;
    }

    bool LookAheadChar(char & ch) const
    {
        if (AtEnd(_index + 1))
            return false;
        ch = _input[_index + 1];
        return true;
    }

    bool AtEnd() const
    {
        return AtEnd(_index);
    }

    bool AtEnd(size_t index) const
    {
        return (index >= _input.length());
    }

    bool Have(char ch) const
    {
        return !AtEnd() && (_input[_index] == ch);
    }

    bool Expect(char ch)
    {
        if (AtEnd())
            return false;
        if (!Have(ch))
            return false;
        Advance();
        return true;
    }

private:
    std::string _input;
    size_t _index;
};

} // namespace Regex