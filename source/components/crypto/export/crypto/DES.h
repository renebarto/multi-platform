#pragma once

#include "crypto/BlockCipher.h"

namespace Crypto
{

static constexpr size_t DESKeySize = 8;
using DESKey = uint8_t[DESKeySize];

class DES
    : public BlockCipher<DESKey>
{
public:

    DES();

    virtual void Initialize(const DESKey & key, Direction direction) override;

    virtual void Process(const uint8_t * dataIn, uint8_t * dataOut, size_t len) override;

    virtual void Process(const OSAL::ByteArray & dataIn, OSAL::ByteArray & dataOut) override;

    virtual void Finalize() override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

    static constexpr size_t BlockSize = 8;
    using DataBlock = uint8_t[BlockSize];
    void ProcessBlock(const DataBlock dataIn, DataBlock dataOut);

private:
    static constexpr size_t SubKeySize = 6;
    static constexpr size_t NumSubKeys = 16;
    using SubKey = uint8_t[SubKeySize];
    using Keyset = SubKey[NumSubKeys];
    void IP(uint32_t state[], const uint8_t dataIn[]);
    void IPInv(const uint32_t state[], uint8_t out[]);
    uint32_t F(uint32_t state, const SubKey & key);

    Keyset _keyset;
};

static constexpr size_t TripleDESKeySize = 3 * DESKeySize;
using TripleDESKey = uint8_t[TripleDESKeySize];

class TripleDES
    : public BlockCipher<TripleDESKey>
{
public:
    TripleDES();

    virtual void Initialize(const TripleDESKey & key, Direction direction) override;

    virtual void Process(const uint8_t * dataIn, uint8_t * dataOut, size_t len) override;

    virtual void Process(const OSAL::ByteArray & dataIn, OSAL::ByteArray & dataOut) override;

    virtual void Finalize() override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

    static constexpr size_t BlockSize = 8;
    using DataBlock = uint8_t[BlockSize];
    void ProcessBlock(const DataBlock dataIn, DataBlock dataOut);

private:
    DES _des1;
    DES _des2;
    DES _des3;
};

inline std::ostream & operator << (std::ostream & stream, const DES & des)
{
    des.PrintTo(stream);
    return stream;
}

inline std::ostream & operator << (std::ostream & stream, const TripleDES & des)
{
    des.PrintTo(stream);
    return stream;
}

} // namespace Crypto
