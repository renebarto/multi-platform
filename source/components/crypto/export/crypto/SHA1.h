#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

class SHA1 : public Digest
{
public:
    union WorkspaceBlock;

    SHA1();

    virtual void Initialize() override;

    virtual void Process(const uint8_t *data, size_t len) override;
    virtual void Process(const Core::ByteArray & data) override;
    virtual void Finalize() override;
    virtual size_t GetDigestSize() const override { return DigestSize; }
    virtual Core::ByteArray GetDigest() const override;

    virtual OSAL::String ToString() const override;

    static constexpr size_t WordLength = 32;
    using Word = uint32_t;
    static const Word K0;
    static const Word K1;
    static const Word K2;
    static const Word K3;

private:
    static constexpr size_t DigestSize = 20;
    static constexpr size_t BlockSize = 64;
    static constexpr size_t BlockSizeMinusOne = BlockSize - 1;

    Word _state[DigestSize >> 2];
    uint64_t _bitCount;
    uint8_t _buffer[BlockSize];
    uint8_t _digest[DigestSize];

    uint8_t _workspace[BlockSize];
    WorkspaceBlock * _block; // SHA1 pointer to the byte array above
    void Transform(const uint8_t buffer[BlockSize]);
};

} // namespace Crypto
