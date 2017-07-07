#include "osal/console-darwin.h"

#if defined(DARWIN)

using namespace OSAL;

OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColorType color)
{
	return{ color };
}

OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColorType color)
{
	return{ color };
}

#endif // defined(DARWIN)
