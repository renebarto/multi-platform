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

    virtual void Process(const Core::ByteArray & dataIn, Core::ByteArray & dataOut) override;

    virtual void Finalize() override;

    virtual OSAL::String ToString() const override;

private:
    static constexpr size_t SubKeySize = 6;
    static constexpr size_t BlockSize = 8;
    static constexpr size_t NumSubKeys = 16;
    using SubKey = uint8_t[SubKeySize];
    using Keyset = SubKey[NumSubKeys];
    using DataBlock = uint8_t[BlockSize];
    void IP(uint32_t state[], const uint8_t dataIn[]);
    void IPInv(const uint32_t state[], uint8_t out[]);
    uint32_t F(uint32_t state, const SubKey & key);

    void ProcessBlock(const DataBlock dataIn, DataBlock dataOut);

    Keyset _keyset;
};

} // namespace Crypto
