#include <core/files/TextMemoryStream.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

static const std::streamsize BufferSize = 4096;

TextMemoryStream::TextMemoryStream()
    : TextStream(_stream)
    , _stream(new MemoryStreamBuf(BufferSize))
{
}

TextMemoryStream::TextMemoryStream(const std::string &value)
    : TextStream(_stream)
    , _stream(new MemoryStreamBuf(BufferSize))
{
    SetContents(value);
}

bool TextMemoryStream::CompareTo(const TextMemoryStream & other) const
{
    return GetContents() == other.GetContents();
}

void TextMemoryStream::Flush()
{
    _stream.flush();
}