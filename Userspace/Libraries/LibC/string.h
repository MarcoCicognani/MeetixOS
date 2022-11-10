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
#pragma ide diagnostic ignored "modernize-deprecated-headers"
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void*       memcpy(void*, const void*, size_t);
void*       memmove(void*, const void*, size_t);
void*       memset(void*, int, size_t);
int         memcmp(const void*, const void*, size_t);
void*       memchr(const void*, int, size_t);
const void* memmem(const void*, size_t, const void*, size_t);

char*  strcpy(char*, const char*);
char*  strncpy(char*, const char*, size_t);
char*  strcat(char*, const char*);
char*  strncat(char*, const char*, size_t);
int    strcmp(const char*, const char*);
int    strncmp(const char*, const char*, size_t);
int    strcoll(const char*, const char*);
size_t strxfrm(char*, const char*, size_t);
char*  strchr(const char*, int);
char*  strrchr(const char*, int);
size_t strcspn(const char*, const char*);
size_t strspn(const char*, const char*);
char*  strpbrk(const char*, const char*);
char*  strstr(const char*, const char*);
char*  strtok(char*, const char*);
char*  strtok_r(char*, const char*, char**);
size_t strlen(const char*);
char*  strdup(const char*);
char*  strerror(int);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
