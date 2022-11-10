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
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"

#include <sys/stat.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(F_OK, int, 0); /* file existence */
CONST_VALUE(X_OK, int, 1); /* execute or search permission */
CONST_VALUE(W_OK, int, 2); /* write permission */
CONST_VALUE(R_OK, int, 3); /* read permission */

extern char** g_environ;
#define environ g_environ

int chdir(const char*);

ssize_t read(int, void*, size_t);
ssize_t write(int, const void*, size_t);

off_t    lseek(int fd, off_t offset, int);
long int tell(int);

int close(int);

void*    sbrk(intptr_t);
unsigned sleep(unsigned);

pid_t getpid();
pid_t getppid();

char* getcwd(char*, size_t);
int   isatty(int);
int   access(const char*, int);
int   fcntl(int, int, ...);
int   rmdir(const char*);
int   symlink(const char*, const char*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop