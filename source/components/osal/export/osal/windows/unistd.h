#ifndef _UNISTD_H
#define _UNISTD_H    1

/* This file intended to serve as a drop-in replacement for 
 *  unistd.h on Windows
 *  Please add functionality as neeeded 
 */

#include <sys\types.h>
#include "sys_stat.h"
#include <stdlib.h>
#include <io.h>
#include <osal/windows/getopt.h> /* getopt from: http://www.pwilson.net/sample.html. */
#include "Process.h"
//#include <pthread.h>

#define srandom srand
#define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1       /* execute permission - unsupported in windows*/
#define F_OK    0       /* Test for existence.  */

#define access _access
#define ftruncate _chsize

#define ssize_t int

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

# define F_ULOCK 0	/* Unlock a previously locked region.  */
# define F_LOCK  1	/* Lock a region for exclusive use.  */
# define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
# define F_TEST  3	/* Test a region for other processes locks.  */

typedef int mode_t;

static const mode_t MS_MODE_MASK = 0x0000ffff;           ///< low word

typedef int	sigset_t;

extern int lockf (int __fd, int __cmd, off_t __len);
//extern pid_t fork(void); 
extern int sigemptyset (sigset_t *__set);
extern int sigaddset (sigset_t *__set, int __signo);

#endif /* unistd.h  */
