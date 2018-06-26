#include <core/files/BinaryMemoryStream.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

static const std::streamsize BufferSize = 4096;

BinaryMemoryStream::BinaryMemoryStream()
    : BinaryStream(_stream)
    , _stream(new BinaryMemoryStreamBuf(BufferSize))
{
}

BinaryMemoryStream::BinaryMemoryStream(const OSAL::ByteArray & value)
    : BinaryStream(_stream)
    , _stream(new BinaryMemoryStreamBuf(BufferSize))
{
    SetContents(value);
}

BinaryMemoryStream::~BinaryMemoryStream()
{
    delete _stream.rdbuf();
}

bool BinaryMemoryStream::CompareTo(const BinaryMemoryStream & other) const
{
    return GetContents() == other.GetContents();
}

void BinaryMemoryStream::Flush()
{
    _stream.flush();
}