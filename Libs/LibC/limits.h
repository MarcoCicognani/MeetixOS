/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-deprecated-headers"

#include <stdint.h>

CONST_VALUE(PAGE_SIZE, size_t, 4096);

CONST_VALUE(HOST_NAME_MAX, size_t, 64);
CONST_VALUE(PATH_MAX, size_t, 4096);
CONST_VALUE(MAXPATHLEN, size_t, PATH_MAX);
CONST_VALUE(NAME_MAX, size_t, 255);
CONST_VALUE(TTY_NAME_MAX, size_t, 32);
CONST_VALUE(PIPE_BUF, size_t, 4096);
CONST_VALUE(INT_MAX, int32_t, INT32_MAX);
CONST_VALUE(INT_MIN, int32_t, INT32_MIN);
CONST_VALUE(UINT_MAX, uint32_t, UINT32_MAX);
CONST_VALUE(CHAR_BIT, size_t, 8);
CONST_VALUE(SCHAR_MIN, int8_t, (-128));
CONST_VALUE(SCHAR_MAX, int8_t, 127);
CONST_VALUE(UCHAR_MAX, uint8_t, 255);
CONST_VALUE(SHRT_MAX, int16_t, 32767);
CONST_VALUE(SHRT_MIN, int16_t, (-SHRT_MAX - 1));
CONST_VALUE(USHRT_MAX, uint16_t, 65535);
CONST_VALUE(LONG_MAX, long, 2147483647L);
CONST_VALUE(LONG_MIN, long, (-LONG_MAX - 1L));
CONST_VALUE(ULONG_MAX, unsigned long, 4294967295UL);
CONST_VALUE(LONG_LONG_MAX, long long, 9223372036854775807LL);
CONST_VALUE(LONG_LONG_MIN, long long, (-LONG_LONG_MAX - 1LL));
CONST_VALUE(LLONG_MAX, long long, LONG_LONG_MAX);
CONST_VALUE(LLONG_MIN, long long, LONG_LONG_MIN);
CONST_VALUE(ULONG_LONG_MAX, unsigned long long, 18446744073709551615ULL);
CONST_VALUE(ULLONG_MAX, unsigned long long, ULONG_LONG_MAX);
CONST_VALUE(CHAR_MIN, char, SCHAR_MIN);
CONST_VALUE(CHAR_MAX, char, SCHAR_MAX);
CONST_VALUE(CHAR_WIDTH, size_t, 8);
CONST_VALUE(SCHAR_WIDTH, size_t, 8);
CONST_VALUE(UCHAR_WIDTH, size_t, 8);
CONST_VALUE(SHRT_WIDTH, size_t, 16);
CONST_VALUE(USHRT_WIDTH, size_t, 16);
CONST_VALUE(INT_WIDTH, size_t, 32);
CONST_VALUE(UINT_WIDTH, size_t, 32);
CONST_VALUE(LONG_WIDTH, size_t, 32);
CONST_VALUE(ULONG_WIDTH, size_t, 32);
CONST_VALUE(LLONG_WIDTH, size_t, 64);
CONST_VALUE(ULLONG_WIDTH, size_t, 64);
CONST_VALUE(ARG_MAX, size_t, 65536);
CONST_VALUE(PTHREAD_STACK_MIN, size_t, 65536);
CONST_VALUE(SSIZE_MAX, ssize_t, 2147483647);
CONST_VALUE(LINK_MAX, size_t, 4096);
CONST_VALUE(TZNAME_MAX, size_t, 64);

#pragma clang diagnostic pop
