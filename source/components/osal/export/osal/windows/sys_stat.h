/*
 * sys_stat.h - sysy/stat additions for Microsoft Visual Studio
 *
 * Copyright (C) 2018 Rene Barto
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * ``Software''), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL TONI RONKKO BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * Sep 18 2018, Rene Barto
 * First version. Includes all permission definitions used by Linux
 *****************************************************************************/
#pragma once

#include <sys/stat.h>

typedef int mode_t;

/// @Note If STRICT_UGO_PERMISSIONS is not defined, then setting Read for any
///       of User, Group, or Other will set Read for User and setting Write
///       will set Write for User.  Otherwise, Read and Write for Group and
///       Other are ignored.
///
/// @Note For the POSIX modes that do not have a Windows equivalent, the modes
///       defined here use the POSIX values left shifted 16 bits.

static const mode_t S_ISUID = 0x08000000;           ///< does nothing
static const mode_t S_ISGID = 0x04000000;           ///< does nothing
static const mode_t S_ISVTX = 0x02000000;           ///< does nothing
static const mode_t S_IRUSR = mode_t(_S_IREAD);     ///< read by user
static const mode_t S_IWUSR = mode_t(_S_IWRITE);    ///< write by user
static const mode_t S_IXUSR = 0x00400000;           ///< does nothing
#ifndef STRICT_UGO_PERMISSIONS
static const mode_t S_IRGRP = mode_t(_S_IREAD);     ///< read by *USER*
static const mode_t S_IWGRP = mode_t(_S_IWRITE);    ///< write by *USER*
static const mode_t S_IXGRP = 0x00080000;           ///< does nothing
static const mode_t S_IROTH = mode_t(_S_IREAD);     ///< read by *USER*
static const mode_t S_IWOTH = mode_t(_S_IWRITE);    ///< write by *USER*
static const mode_t S_IXOTH = 0x00010000;           ///< does nothing
#else
static const mode_t S_IRGRP = 0x00200000;           ///< does nothing
static const mode_t S_IWGRP = 0x00100000;           ///< does nothing
static const mode_t S_IXGRP = 0x00080000;           ///< does nothing
static const mode_t S_IROTH = 0x00040000;           ///< does nothing
static const mode_t S_IWOTH = 0x00020000;           ///< does nothing
static const mode_t S_IXOTH = 0x00010000;           ///< does nothing
#endif
static const mode_t S_IRWXU = (S_IRUSR | S_IWUSR | S_IXUSR);
static const mode_t S_IRWXG = (S_IRGRP | S_IWGRP | S_IXGRP);
static const mode_t S_IRWXO = (S_IROTH | S_IWOTH | S_IXOTH);
