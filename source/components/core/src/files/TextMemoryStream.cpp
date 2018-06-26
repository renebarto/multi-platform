#include <core/files/TextMemoryStream.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

static const std::streamsize BufferSize = 4096;

TextMemoryStream::TextMemoryStream()
    : TextStream(_stream)
    , _stream(new TextMemoryStreamBuf(BufferSize))
{
}

TextMemoryStream::TextMemoryStream(const std::string & value)
    : TextStream(_stream)
    , _stream(new TextMemoryStreamBuf(BufferSize))
{
    SetContents(value);
}

TextMemoryStream::~TextMemoryStream()
{
    delete _stream.rdbuf();
}

bool TextMemoryStream::CompareTo(const TextMemoryStream & other) const
{
    return GetContents() == other.GetContents();
}

void TextMemoryStream::Flush()
{
    _stream.flush();
}