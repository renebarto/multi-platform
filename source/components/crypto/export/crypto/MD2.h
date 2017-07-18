#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

class MD2 : public Digest
{
public:
    union WorkspaceBlock;

    MD2();

    virtual void Initialize() override;

    virtual void Process(const uint8_t *data, size_t len) override;
    virtual void Process(const Core::ByteArray & data) override;
    virtual void Finalize() override;
    virtual size_t GetDigestSize() const override { return DigestSize; }
    virtual Core::ByteArray GetDigest() const override;

    virtual OSAL::String ToString() const override;

private:
    static constexpr size_t DigestSize = 16;
    static constexpr size_t StateSize = 48;
    static constexpr size_t BlockSize = 16;
    static constexpr size_t BlockSizeMinusOne = BlockSize - 1;

    uint8_t _digest[DigestSize];
    uint8_t _buffer[BlockSize];
    uint64_t _byteCount;
    uint8_t _state[StateSize];
    uint8_t _checksum[BlockSize];

    void DumpState();
    void DumpBlock(const uint8_t buffer[BlockSize]);
    void Transform(const uint8_t buffer[BlockSize]);
};

} // namespace Crypto
