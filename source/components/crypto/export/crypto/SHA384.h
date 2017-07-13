#pragma once

#include "crypto/Digest.h"

namespace Crypto
{

union SHA384WorkspaceBlock;

class SHA384 : public Digest
{
public:
    SHA384();

    virtual void Initialize() override;

    virtual void Process(const uint8_t *data, size_t len) override;
    virtual void Process(const Core::ByteArray & data) override;
    virtual void Finalize() override;
    virtual Core::ByteArray GetDigest() const override;

    virtual OSAL::String ToString() const override;

private:
    // Private SHA-1 transformation
    void Transform(uint32_t state[5], const uint8_t buffer[64]);

    uint32_t _state[8];
    uint32_t _count[2];
    uint8_t _buffer[128];
    uint8_t _digest[48];

    uint8_t _workspace[64];
    SHA384WorkspaceBlock * _block; // SHA384 pointer to the byte array above
};

} // namespace Crypto
