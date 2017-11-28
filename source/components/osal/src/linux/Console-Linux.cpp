#include "osal/Console.h"

#if defined(LINUX)

using namespace OSAL;

OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColor color)
{
	return{ color };
}

OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColor color)
{
	return{ color };
}

#endif // defined(LINUX)
