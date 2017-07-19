#pragma once

#include <crypto/BlockCipher.h>

namespace Crypto
{

class Base64
{
public:
    static size_t Encode(const char * dataIn, size_t dataLengthIn, char * dataOut, bool addNewLines = true);

    static void Encode(const OSAL::String & dataIn, OSAL::String & dataOut);

    static size_t Decode(const char * dataIn, size_t dataLengthIn, char * dataOut);

    static void Decode(const OSAL::String & dataIn, OSAL::String & dataOut);
};

} // namespace Crypto
