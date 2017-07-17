#pragma once

#include "crypto/Digest.h"

namespace Crypto
{
    class SHA256Base : public Digest
    {
    public:
        struct WorkspaceBlock;

        SHA256Base();
        virtual ~SHA256Base();

        virtual void Process(const uint8_t *data, size_t len) override;
        virtual void Process(const Core::ByteArray & data) override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const = 0;
        virtual Core::ByteArray GetDigest() const = 0;

        virtual OSAL::String ToString() const = 0;

        static constexpr size_t WordLength = 32;
        using Word = uint32_t;
        static const Word K[64];

        void DumpState();
        static void DumpState(size_t t, Word a, Word b, Word c, Word d, Word e, Word f, Word g, Word h);
        static void DumpBlock(WorkspaceBlock * block);

    protected:
        static constexpr size_t BlockSize = 64;
        static constexpr size_t BlockSizeMinusOne = BlockSize - 1;
        static constexpr size_t StateSize = 8;

        uint64_t _bitCount;
        uint8_t _buffer[BlockSize];
        Word _state[StateSize];

        uint8_t _workspace[BlockSize];
        WorkspaceBlock * _block; // SHA256 pointer to the byte array above

        void Transform(const uint8_t buffer[BlockSize]);
    };

    class SHA224 : public SHA256Base
    {
    public:
        SHA224();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

    private:
        static constexpr size_t DigestSize = 28;

        uint8_t _digest[DigestSize];
    };

    class SHA256 : public SHA256Base
    {
    public:
        SHA256();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

    private:
        static constexpr size_t DigestSize = 32;

        uint8_t _digest[DigestSize];
    };
} // namespace Crypto
