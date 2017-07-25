#include "osal/mingw/Console.h"

#if defined(WIN_MINGW)

using namespace OSAL;

OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColorType color)
{
	return{ color };
}

OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColorType color)
{
	return{ color };
}

#endif // defined(WIN_MINGW)
