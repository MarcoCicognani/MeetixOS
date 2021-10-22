/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

/* ------------------------------------------ C defines ----------------------------------------- */

#ifdef __x86_x64__
#    error "architecture not supported"
#elif __i386__
#    define LONG_BIT  32
#    define LONG_MAX  0x7fffffffL
#    define LLONG_MAX 0x7fffffffffffffffLL
#    if '\0' - 1 > 0
#        define CHAR_MIN 0
#        define CHAR_MAX 255
#    else
#        define CHAR_MIN (-128)
#        define CHAR_MAX 127
#    endif
#    define CHAR_BIT           8
#    define SCHAR_MIN          (-128)
#    define SCHAR_MAX          127
#    define UCHAR_MAX          255
#    define SHRT_MIN           (-1 - 0x7fff)
#    define SHRT_MAX           0x7fff
#    define USHRT_MAX          0xffff
#    define INT_MIN            (-1 - 0x7fffffff)
#    define INT_MAX            0x7fffffff
#    define UINT_MAX           0xffffffffU
#    define LONG_MIN           (-LONG_MAX - 1)
#    define ULONG_MAX          (2UL * LONG_MAX + 1)
#    define LLONG_MIN          (-LLONG_MAX - 1)
#    define ULLONG_MAX         (2ULL * LLONG_MAX + 1)
#    define MB_LEN_MAX         4
#    define PIPE_BUF           PIPE_DEFAULT_CAPACITY
#    define FILESIZEBITS       64
#    define NAME_MAX           FILENAME_MAX
#    define SYMLINK_MAX        FILENAME_MAX
#    define PATH_MAX           512
#    define NZERO              20
#    define NGROUPS_MAX        32
#    define ARG_MAX            131072
#    define IOV_MAX            1024
#    define SYMLOOP_MAX        40
#    define WORD_BIT           32
#    define SSIZE_MAX          LONG_MAX
#    define TZNAME_MAX         6
#    define TTY_NAME_MAX       32
#    define HOST_NAME_MAX      255
#    define BC_BASE_MAX        99
#    define BC_DIM_MAX         2048
#    define BC_SCALE_MAX       99
#    define BC_STRING_MAX      1000
#    define CHARCLASS_NAME_MAX 14
#    define COLL_WEIGHTS_MAX   2
#    define EXPR_NEST_MAX      32
#    define LINE_MAX           4096
#    define RE_DUP_MAX         255
#    define NL_ARGMAX          9
#    define NL_LANGMAX         32
#    define NL_MSGMAX          32767
#    define NL_SETMAX          255
#    define NL_TEXTMAX         2048
#    define NL_NMAX            16
#else
#    error "architecture not supported"
#endif
