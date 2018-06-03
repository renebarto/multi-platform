#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/files/File.h"

namespace Core {

class IBinaryStream
{
public:
    virtual ~IBinaryStream() {}

    // TODO: Define interface
};

} // namespace Core
