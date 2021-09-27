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

#include <Api/Common.h>
#include <stdint.h>

__BEGIN_C

/* ------------------------------------ C function prototypes ----------------------------------- */

void* memcpy(void*, const void*, usize);
void* memmove(void*, const void*, usize);
void* memset(void*, int, usize);
int   memcmp(const void*, const void*, usize);
void* memchr(const void*, int, usize);

char* strcpy(char*, const char*);
char* strncpy(char*, const char*, usize);
char* strcat(char*, const char*);
char* strncat(char*, const char*, usize);
int   strcmp(const char*, const char*);
int   strncmp(const char*, const char*, usize);
int   strcoll(const char*, const char*);
usize strxfrm(char*, const char*, usize);
char* strchr(const char*, int);
char* strrchr(const char*, int);
usize strcspn(const char*, const char*);
usize strspn(const char*, const char*);
char* strpbrk(const char*, const char*);
char* strstr(const char*, const char*);
char* strtok(char*, const char*);
usize strlen(const char*);
char* strdup(const char*);
char* strerror(int);

__END_C
