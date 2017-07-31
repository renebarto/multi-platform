#include "core/ConsoleLogger.h"

using namespace std;
using namespace OSAL;
using namespace Core;

ConsoleLogger::ConsoleLogger(ILogger & logger, Console & console, bool autoRegister/* = true*/)
    : logger(logger)
    , console(console)
{
    if (autoRegister)
        Register();
}

ConsoleLogger::~ConsoleLogger()
{
    UnRegister();
}

ConsoleColor ColorForLogLevel(LogLevel logLevel)
{
    switch (logLevel & LogLevel::Mask)
    {
    case LogLevel::Error:
        return ConsoleColor::Red | ConsoleColor::Intensity;
    case LogLevel::Warning:
        return ConsoleColor::Yellow | ConsoleColor::Intensity;
    case LogLevel::Message:
        return ConsoleColor::Magenta | ConsoleColor::Intensity;
    case LogLevel::Info:
        return ConsoleColor::Green;
    case LogLevel::Debug:
        return ConsoleColor::Cyan;
	case LogLevel::All:
	case LogLevel::Recursion:
	case LogLevel::Fatal:
	case LogLevel::None:
	default:
        break;
    }
    return ConsoleColor::Default;
}

void ConsoleLogger::Register()
{
    logger.AddObserver(this);
}

void ConsoleLogger::UnRegister()
{
    logger.RemoveObserver(this);
}

void ConsoleLogger::Log(const DateTime & timestamp,
                        const OSAL::String & domainName,
                        const OSAL::String & componentName,
                        LogLevel logLevel,
                        const OSAL::String & message)
{
    console << fgcolor(ColorForLogLevel(logLevel))
            << timestamp << _(" - ")
            << (domainName.empty() ? _("Default") : domainName) << _(": ")
            << componentName << _(": ") << message
            << fgcolor(ConsoleColor::Default) << endl;
}

