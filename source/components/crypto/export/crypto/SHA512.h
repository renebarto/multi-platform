#pragma once

#include "crypto/Digest.h"

namespace Crypto
{
    class SHA512Base : public Digest
    {
    public:
        union WorkspaceBlock;

        SHA512Base();

        virtual void Process(const uint8_t *data, size_t len) override;
        virtual void Process(const Core::ByteArray & data) override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const = 0;
        virtual Core::ByteArray GetDigest() const = 0;

        virtual OSAL::String ToString() const = 0;

        static constexpr size_t WordLength = 64;
        using Word = uint64_t;

    protected:
        static constexpr size_t BlockSize = 128;
        static constexpr size_t BlockSizeMinusOne = BlockSize - 1;
        static const Word K[80];

        uint64_t _bitCount;
        uint8_t _buffer[BlockSize];

        uint8_t _workspace[BlockSize];
        WorkspaceBlock * _block; // SHA256 pointer to the byte array above

        virtual void Transform(const uint8_t buffer[BlockSize]) = 0;
    };

    class SHA384 : public SHA512Base
    {
    public:
        SHA384();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

    private:
        static constexpr size_t DigestSize = 48;

        Word _state[DigestSize >> 2];
        uint8_t _digest[DigestSize];

        void Transform(const uint8_t buffer[BlockSize]);
    };

    class SHA512 : public SHA512Base
    {
    public:
        SHA512();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

    private:
        static constexpr size_t DigestSize = 64;

        Word _state[DigestSize >> 2];
        uint8_t _digest[DigestSize];

        void Transform(const uint8_t buffer[BlockSize]);
    };

    class SHA512_224 : public SHA512Base
    {
    public:
        SHA512_224();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

    private:
        static constexpr size_t DigestSize = 28;

        Word _state[DigestSize >> 2];
        uint8_t _digest[DigestSize];

        void Transform(const uint8_t buffer[BlockSize]);
    };

    class SHA512_256 : public SHA512Base
    {
    public:
        SHA512_256();

        virtual void Initialize() override;
        virtual void Finalize() override;

        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

    private:
        static constexpr size_t DigestSize = 32;

        Word _state[DigestSize >> 2];
        uint8_t _digest[DigestSize];

        void Transform(const uint8_t buffer[BlockSize]);
    };

} // namespace Crypto
