#pragma once

#include "osal/Strings.h"
#include "osal/ByteArray.h"
#include "crypto/exports.h"

namespace Crypto
{

class Digest
{
public:
    virtual ~Digest() {}

    virtual void Initialize() = 0;

    virtual void Process(const uint8_t *data, size_t len) = 0;
    virtual void Process(const OSAL::ByteArray & data) = 0;
    virtual void Finalize() = 0;
    virtual size_t GetDigestSize() const = 0;
    virtual OSAL::ByteArray GetDigest() const = 0;

    virtual OSAL::String ToString() const = 0;
};

inline void PrintTo(const Digest & value, std::basic_ostream<OSAL::Char> & stream)
{
    stream << value.ToString();
}

} // namespace Crypto
