#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

class SHA512Base : public Digest
{
public:
    struct WorkspaceBlock;

    SHA512Base();

    virtual void Process(const uint8_t * data, size_t len) override;

    virtual void Process(const OSAL::ByteArray & data) override;

    virtual void Finalize() override;

    virtual size_t GetDigestSize() const = 0;

    virtual OSAL::ByteArray GetDigest() const = 0;

    virtual std::ostream & PrintTo(std::ostream & stream) const = 0;

protected:
    static constexpr size_t BlockSize = 128;
    static constexpr size_t BlockSizeMinusOne = BlockSize - 1;
    static constexpr size_t StateSize = 8;
    static constexpr size_t WordLength = 64;

    using Word = uint64_t;
    static const Word K[80];

    uint64_t _bitCountL;
    uint64_t _bitCountH;
    uint8_t _buffer[BlockSize];

    Word _state[StateSize];
    uint8_t _workspace[BlockSize];
    WorkspaceBlock * _block; // SHA256 pointer to the byte array above

    void DumpState();
    void DumpState(size_t t, Word a, Word b, Word c, Word d, Word e, Word f, Word g, Word h);
    void DumpBlock(WorkspaceBlock * block);

    Word ROTLEFT(Word a, size_t b);

    Word ROTRIGHT(Word a, size_t b);

    Word CH(Word x, Word y, Word z);

    Word MAJ(Word x, Word y, Word z);

    Word EP0(Word x);

    Word EP1(Word x);

    Word SIG0(Word x);

    Word SIG1(Word x);

    Word SHABLK0(WorkspaceBlock * block, size_t i);

    Word SHABLK(WorkspaceBlock * block, size_t t);

    void Round0(WorkspaceBlock * block,
                Word & a, Word & b, Word & c, Word & d,
                Word & e, Word & f, Word & g, Word & h,
                size_t t);

    void Round1(WorkspaceBlock * block,
                Word & a, Word & b, Word & c, Word & d,
                Word & e, Word & f, Word & g, Word & h,
                size_t t);

    void Transform(const uint8_t buffer[BlockSize]);
};

class SHA384 : public SHA512Base
{
public:
    SHA384();

    virtual void Initialize() override;

    virtual void Finalize() override;

    virtual size_t GetDigestSize() const override
    { return DigestSize; }

    virtual OSAL::ByteArray GetDigest() const override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    static constexpr size_t DigestSize = 48;
    uint8_t _digest[DigestSize];
};

class SHA512 : public SHA512Base
{
public:
    SHA512();

    virtual void Initialize() override;

    virtual void Finalize() override;

    virtual size_t GetDigestSize() const override
    { return DigestSize; }

    virtual OSAL::ByteArray GetDigest() const override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    static constexpr size_t DigestSize = 64;
    uint8_t _digest[DigestSize];
};

class SHA512_224 : public SHA512Base
{
public:
    SHA512_224();

    virtual void Initialize() override;

    virtual void Finalize() override;

    virtual size_t GetDigestSize() const override
    { return DigestSize; }

    virtual OSAL::ByteArray GetDigest() const override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    static constexpr size_t DigestSize = 28;
    uint8_t _digest[DigestSize];
};

class SHA512_256 : public SHA512Base
{
public:
    SHA512_256();

    virtual void Initialize() override;

    virtual void Finalize() override;

    virtual size_t GetDigestSize() const override
    { return DigestSize; }

    virtual OSAL::ByteArray GetDigest() const override;

    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    static constexpr size_t DigestSize = 32;
    uint8_t _digest[DigestSize];
};

inline std::ostream & operator << (std::ostream & stream, const SHA512Base & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Crypto
