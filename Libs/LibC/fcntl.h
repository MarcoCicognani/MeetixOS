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

#include <LibApi/Api/FileSystem.h>
#include <LibApi/Api/StdInt.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define O_READ   FILE_FLAG_MODE_READ
#define O_WRITE  FILE_FLAG_MODE_WRITE
#define O_APPEND FILE_FLAG_MODE_APPEND
#define O_CREAT  FILE_FLAG_MODE_CREATE
#define O_TRUNC  FILE_FLAG_MODE_TRUNCATE
#define O_EXCL   FILE_FLAG_MODE_EXCLUSIVE
#define O_RDONLY FILE_FLAG_MODE_READ
#define O_WRONLY FILE_FLAG_MODE_WRITE
#define O_RDWR   (FILE_FLAG_MODE_READ | FILE_FLAG_MODE_WRITE)

#define F_DUPFD    0
#define F_GETFD    0
#define F_SETFD    0
#define F_SETFL    0
#define F_GETLK    0
#define F_SETLK    0
#define F_SETLKW   0
#define F_GETOWN   0
#define F_SETOWN   0
#define FD_CLOEXEC 0

/* ------------------------------------------- C types ------------------------------------------ */

typedef i32 mode_t;

/* ------------------------------------ C function prototypes ----------------------------------- */

int open(const char*, int, ...);

#ifdef __cplusplus
}
#endif
