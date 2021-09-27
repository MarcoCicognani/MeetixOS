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

__BEGIN_C

/* ------------------------------------------ C defines ----------------------------------------- */

#define RTLD_LAZY 0
#define RTLD_NOW  1

/* ------------------------------------ C function prototypes ----------------------------------- */

void* dlopen(const char*, int);
int   dlclose(void*);
void* dlsym(void*, const char*);
char* dlerror(void);

__END_C