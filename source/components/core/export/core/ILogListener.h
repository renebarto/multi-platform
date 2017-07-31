#pragma once

#include <string>
#include "core/Logger.h"
#include "core/DateTime.h"

namespace Core
{

class ILogListener
{
public:
    virtual ~ILogListener() {}

    virtual void Log(const DateTime & timestamp,
                     const OSAL::String & domainName,
                     const OSAL::String & componentName,
                     LogLevel logLevel,
                     const OSAL::String & message) = 0;
};

} // namespace Core
