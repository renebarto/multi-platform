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
    virtual void Process(const OSAL::ByteArray & data) override;
    virtual void Finalize() override;
    virtual size_t GetDigestSize() const override { return DigestSize; }
    virtual OSAL::ByteArray GetDigest() const override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

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

    void DumpState();
    void DumpState(size_t t, Word a, Word b, Word c, Word d);
    void DumpBlock(WorkspaceBlock * block);

    Word ROTLEFT(Word value, size_t bits);
    Word GetData(const WorkspaceBlock * block, size_t i);

    Word F(Word x, Word y, Word z);
    Word G(Word x, Word y, Word z);
    Word H(Word x, Word y, Word z);
    Word I(Word x, Word y, Word z);
    void FF(Word & a, Word b, Word c, Word d, Word m, size_t s, Word t);
    void GG(Word & a, Word b, Word c, Word d, Word m, size_t s, Word t);
    void HH(Word & a, Word b, Word c, Word d, Word m, size_t s, Word t);
    void II(Word & a, Word b, Word c, Word d, Word m, size_t s, Word t);

    void Transform(const uint8_t buffer[BlockSize]);
};

inline std::ostream & operator << (std::ostream & stream, const MD5 & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Crypto
