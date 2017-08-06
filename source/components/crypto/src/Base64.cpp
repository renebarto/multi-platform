#include "crypto/Base64.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <sstream>

using namespace std;
using namespace Crypto;

static const char * charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static constexpr size_t LineSizeChars = 76;
static constexpr size_t GroupSizeBytes = 3;
static constexpr size_t GroupSizeChars = 4;
static constexpr size_t LineSizeBytes = (LineSizeChars + GroupSizeChars - 1) / GroupSizeChars * GroupSizeBytes;

char ByteToChar(uint8_t byte)
{
    return charset[byte & 0x3F];
}

size_t Base64::Encode(const char * dataIn, size_t dataLengthIn, char * dataOut, bool addNewlines)
{
    size_t charGroups = (dataLengthIn / GroupSizeBytes);
    size_t bytesLeftOver = dataLengthIn % GroupSizeBytes;

    if (dataOut == nullptr)
    {
        size_t base64Chars = charGroups * GroupSizeChars;
        if (bytesLeftOver)
            base64Chars += GroupSizeChars;
        if (addNewlines)
            base64Chars += dataLengthIn / LineSizeBytes;
        return base64Chars;
    }
    size_t offsetIn = 0;
    size_t offsetOut = 0;
    size_t numNewLines = 0;
    for (size_t group = 0; group < charGroups; ++group)
    {
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(dataIn[offsetIn] >> 2));
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(((dataIn[offsetIn] & 0x03) << 4) | (dataIn[offsetIn + 1] >> 4)));
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(((dataIn[offsetIn + 1] & 0x0F) << 2) | (dataIn[offsetIn + 2] >> 6)));
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(dataIn[offsetIn + 2] & 0x3F));
        offsetIn += GroupSizeBytes;
        if ((offsetOut - numNewLines) % LineSizeChars == 0)
        {
            dataOut[offsetOut++] = '\n';
            numNewLines++;
        }
    }

    if (bytesLeftOver == 1)
    {
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(dataIn[offsetIn] >> 2));
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>((dataIn[offsetIn] & 0x03) << 4));
        dataOut[offsetOut++] = '=';
        dataOut[offsetOut++] = '=';
    }
    else if (bytesLeftOver == 2)
    {
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(dataIn[offsetIn] >> 2));
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>(((dataIn[offsetIn] & 0x03) << 4) | (dataIn[offsetIn + 1] >> 4)));
        dataOut[offsetOut++] = ByteToChar(static_cast<uint8_t>((dataIn[offsetIn + 1] & 0x0F) << 2));
        dataOut[offsetOut++] = '=';
    }

    return offsetOut;
}

void Base64::Encode(const OSAL::String & dataIn, OSAL::String & dataOut)
{
    std::string dataInNarrow(OSAL::ToNarrowString(dataIn));
    size_t length = Encode(dataInNarrow.c_str(), dataInNarrow.length(), nullptr);
    char * str = new char[length + 1];
    Encode(dataInNarrow.c_str(), dataInNarrow.length(), str);
    str[length] = '\0';
    dataOut = OSAL::ToString(str);
    delete [] str;
}

uint8_t CharToByte(char ch)
{
    uint8_t data = static_cast<uint8_t>(ch);
    if (ch >= 'A' && ch <= 'Z')
        data -= 'A';
    else if (ch >= 'a' && ch <='z')
        data -= ('a' - 26);
    else if (ch >= '0' && ch <='9')
        data -= ('0' - 52);
    else if (ch == '+')
        data = 62;
    else if (ch == '/')
        data = 63;

    return data;
}

size_t Base64::Decode(const char * dataIn, size_t dataLengthIn, char * dataOut)
{
    size_t length = dataLengthIn;
    if (dataIn[length - 1] == '=')
        length--;
    if (dataIn[length - 1] == '=')
        length--;

    size_t charGroups = length / GroupSizeChars;
    size_t charsLeftOver = length % GroupSizeChars;

    if (dataOut == nullptr)
    {
        if (length >= LineSizeChars && dataIn[LineSizeChars] == '\n')   // Verify that newlines where used.
            length -= length / (LineSizeChars + 1);
        charGroups = length / GroupSizeChars;
        charsLeftOver = length % GroupSizeChars;
        size_t numBytes = charGroups * GroupSizeBytes;
        if (charsLeftOver >= 2)
            ++numBytes;
        if (charsLeftOver >= 3)
            ++numBytes;
        return numBytes;
    }
    size_t offsetIn = 0;
    size_t offsetOut = 0;
    for (size_t group = 0; group < charGroups; ++group)
    {
        dataOut[offsetOut++] = static_cast<uint8_t>((CharToByte(dataIn[offsetIn]) << 2) | (CharToByte(dataIn[offsetIn + 1]) >> 4));
        dataOut[offsetOut++] = static_cast<uint8_t>((CharToByte(dataIn[offsetIn + 1]) << 4) | (CharToByte(dataIn[offsetIn + 2]) >> 2));
        dataOut[offsetOut++] = static_cast<uint8_t>((CharToByte(dataIn[offsetIn + 2]) << 6) | (CharToByte(dataIn[offsetIn + 3]) >> 0));
        offsetIn += GroupSizeChars;
        if (dataIn[offsetIn] == '\n')
        {
            ++offsetIn;
        }
    }

    if (charsLeftOver == 2)
    {
        dataOut[offsetOut++] = static_cast<uint8_t>((CharToByte(dataIn[offsetIn]) << 2) | (CharToByte(dataIn[offsetIn + 1]) >> 4));
    }
    else if (charsLeftOver == 3)
    {
        dataOut[offsetOut++] = static_cast<uint8_t>((CharToByte(dataIn[offsetIn]) << 2) | (CharToByte(dataIn[offsetIn + 1]) >> 4));
        dataOut[offsetOut++] = static_cast<uint8_t>((CharToByte(dataIn[offsetIn + 1]) << 4) | (CharToByte(dataIn[offsetIn + 2]) >> 2));
    }

    return offsetOut;
}

void Base64::Decode(const OSAL::String & dataIn, OSAL::String & dataOut)
{
    std::string dataInNarrow(OSAL::ToNarrowString(dataIn));
    size_t length = Decode(dataInNarrow.c_str(), dataInNarrow.length(), nullptr);
    char * str = new char[length + 1];
    Decode(dataInNarrow.c_str(), dataInNarrow.length(), str);
    str[length] = '\0';
    dataOut = OSAL::ToString(str);
    delete [] str;
}


