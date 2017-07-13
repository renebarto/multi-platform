#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

    class SHA256 : public Digest
    {
    public:
        union WorkspaceBlock;

        SHA256();

        virtual void Initialize() override;

        virtual void Process(const uint8_t *data, size_t len) override;
        virtual void Process(const Core::ByteArray & data) override;
        virtual void Finalize() override;
        virtual size_t GetDigestSize() const override { return DigestSize; }
        virtual Core::ByteArray GetDigest() const override;

        virtual OSAL::String ToString() const override;

        static constexpr size_t WordLength = 32;
        using Word = uint32_t;

    private:
        static constexpr size_t DigestSize = 32;
        static constexpr size_t BlockSize = 64;
        static constexpr size_t BlockSizeMinusOne = BlockSize - 1;
        static const Word K[64];
            // Private SHA-1 transformation
        void Transform(uint32_t state[DigestSize >> 2], const uint8_t buffer[BlockSize]);

        Word _state[DigestSize >> 2];
        uint64_t _bitCount;
        uint8_t _buffer[BlockSize];
        uint8_t _digest[DigestSize];

        uint8_t _workspace[BlockSize];
        WorkspaceBlock * _block; // SHA256 pointer to the byte array above
    };

} // namespace Crypto
