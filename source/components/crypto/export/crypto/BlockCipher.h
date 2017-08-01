#pragma once

#include "osal/Strings.h"
#include "core/ByteArray.h"
#include "crypto/exports.h"

namespace Crypto
{

enum Direction
{
    Encrypt,
    Decrypt,
};

template<class Key>
class BlockCipher
{
public:
    virtual ~BlockCipher() {}

    virtual void Initialize(const Key & key, Direction direction) = 0;

    virtual void Process(const uint8_t * dataIn, uint8_t * dataOut, size_t len) = 0;
    virtual void Process(const Core::ByteArray & dataIn, Core::ByteArray & dataOut) = 0;
    virtual void Finalize() = 0;

    virtual OSAL::String ToString() const = 0;

};

template<class Key>
inline void PrintTo(const BlockCipher<Key> & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Crypto
