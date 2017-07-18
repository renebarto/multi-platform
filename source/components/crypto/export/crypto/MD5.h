#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

class MD5 : public Digest
{
public:
    union WorkspaceBlock;

    MD5();

    virtual void Initialize() override;

    virtual void Process(const uint8_t *data, size_t len) override;
    virtual void Process(const Core::ByteArray & data) override;
    virtual void Finalize() override;
    virtual size_t GetDigestSize() const override { return DigestSize; }
    virtual Core::ByteArray GetDigest() const override;

    virtual OSAL::String ToString() const override;

private:
    static constexpr size_t DigestSize = 16;
    static constexpr size_t StateSize = 4;
    static constexpr size_t BlockSize = 64;
    static constexpr size_t BlockSizeMinusOne = BlockSize - 1;
    static constexpr size_t WordLength = 32;
    using Word = uint32_t;
    static const Word K[BlockSize];

    uint8_t _digest[DigestSize];
    uint8_t _buffer[BlockSize];
    uint64_t _bitCount;
    uint32_t _state[StateSize];

    uint8_t _workspace[BlockSize];
    WorkspaceBlock * _block;

    Word ROTL(Word value, size_t bits);
    Word GetData(const WorkspaceBlock * block, size_t i);

    Word F(Word x, Word y, Word z);
    Word G(Word x, Word y, Word z);
    Word H(Word x, Word y, Word z);
    Word I(Word x, Word y, Word z);

    void Transform(const uint8_t buffer[BlockSize]);
};

} // namespace Crypto
