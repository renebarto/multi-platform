#pragma once

#include "osal/OSAL.h"

namespace OSAL {
namespace Path {

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#define	__S_IREAD	0400	// Read by owner.
#define	__S_IWRITE	0200	// Write by owner.
#define	__S_IEXEC	0100	// Execute by owner.

#define	S_IRUSR	__S_IREAD	// Read by owner.
#define	S_IWUSR	__S_IWRITE	// Write by owner.
#define	S_IXUSR	__S_IEXEC	// Execute by owner.
	// Read, write, and execute by owner.
#define	S_IRWXU	(__S_IREAD|__S_IWRITE|__S_IEXEC)

#define	S_IRGRP	(S_IRUSR >> 3)	// Read by group.
#define	S_IWGRP	(S_IWUSR >> 3)	// Write by group.
#define	S_IXGRP	(S_IXUSR >> 3)	// Execute by group.
	// Read, write, and execute by group.
#define	S_IRWXG	(S_IRWXU >> 3)

#define	S_IROTH	(S_IRGRP >> 3)	// Read by others.
#define	S_IWOTH	(S_IWGRP >> 3)	// Write by others.
#define	S_IXOTH	(S_IXGRP >> 3)	// Execute by others.
	// Read, write, and execute by others.
#define	S_IRWXO	(S_IRWXG >> 3)

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif

} // namespace Path
} // namespace OSAL