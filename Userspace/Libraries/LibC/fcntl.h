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
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <Api/FileSystem.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(O_READ, int, FILE_FLAG_MODE_READ);
CONST_VALUE(O_WRITE, int, FILE_FLAG_MODE_WRITE);
CONST_VALUE(O_APPEND, int, FILE_FLAG_MODE_APPEND);
CONST_VALUE(O_CREAT, int, FILE_FLAG_MODE_CREATE);
CONST_VALUE(O_TRUNC, int, FILE_FLAG_MODE_TRUNCATE);
CONST_VALUE(O_EXCL, int, FILE_FLAG_MODE_EXCLUSIVE);
CONST_VALUE(O_RDONLY, int, FILE_FLAG_MODE_READ);
CONST_VALUE(O_WRONLY, int, FILE_FLAG_MODE_WRITE);
CONST_VALUE(O_RDWR, int, FILE_FLAG_MODE_READ | FILE_FLAG_MODE_WRITE);

CONST_VALUE(F_DUPFD, int, 0);
CONST_VALUE(F_GETFD, int, 0);
CONST_VALUE(F_SETFD, int, 0);
CONST_VALUE(F_SETFL, int, 0);
CONST_VALUE(F_GETLK, int, 0);
CONST_VALUE(F_SETLK, int, 0);
CONST_VALUE(F_SETLKW, int, 0);
CONST_VALUE(F_GETOWN, int, 0);
CONST_VALUE(F_SETOWN, int, 0);
CONST_VALUE(FD_CLOEXEC, int, 0);

TYPE_ALIAS(mode_t, int32_t);

int open(const char*, int, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
