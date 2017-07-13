#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

union SHA512WorkspaceBlock;

class SHA512 : public Digest
{
public:
    SHA512(size_t requestedDigestSize);

    virtual void Initialize() override;

    virtual void Process(const uint8_t *data, size_t len) override;
    virtual void Process(const Core::ByteArray & data) override;
    virtual void Finalize() override;
    virtual size_t GetDigestSize() const override { return _requestedDigestSize; }
    virtual Core::ByteArray GetDigest() const override;

    virtual OSAL::String ToString() const override;

private:
    // Private SHA-1 transformation
    void Transform(uint32_t state[5], const uint8_t buffer[64]);

    uint32_t _state[8];
    uint32_t _count[2];
    uint8_t _buffer[128];
    uint8_t _digest[64];

    uint8_t _workspace[64];
    SHA512WorkspaceBlock * _block; // SHA512 pointer to the byte array above
    size_t _requestedDigestSize;
};

} // namespace Crypto
