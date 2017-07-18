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


private:
    static constexpr size_t DigestSize = 20;
    static constexpr size_t BlockSize = 64;
    static constexpr size_t BlockSizeMinusOne = BlockSize - 1;
    static constexpr size_t WordLength = 32;
    using Word = uint32_t;
    static const Word K0;
    static const Word K1;
    static const Word K2;
    static const Word K3;

    Word _state[DigestSize >> 2];
    uint64_t _bitCount;
    uint8_t _buffer[BlockSize];
    uint8_t _digest[DigestSize];

    uint8_t _workspace[BlockSize];
    WorkspaceBlock * _block; // SHA1 pointer to the byte array above

    Word ROTL(Word value, size_t bits);
    Word SHABLK0(WorkspaceBlock * block, size_t i);
    Word SHABLK(WorkspaceBlock * block, size_t i);
    void Round0(WorkspaceBlock * block, Word & v, Word & w, Word & x, Word & y, Word & z, size_t i);
    void Round1(WorkspaceBlock * block, Word & v, Word & w, Word & x, Word & y, Word & z, size_t i);
    void Round2(WorkspaceBlock * block, Word & v, Word & w, Word & x, Word & y, Word & z, size_t i);
    void Round3(WorkspaceBlock * block, Word & v, Word & w, Word & x, Word & y, Word & z, size_t i);
    void Round4(WorkspaceBlock * block, Word & v, Word & w, Word & x, Word & y, Word & z, size_t i);
    void Transform(const uint8_t buffer[BlockSize]);
};

} // namespace Crypto
