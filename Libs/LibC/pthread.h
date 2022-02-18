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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define PTHREAD_CREATE_DETACHED 0
#define PTHREAD_CREATE_JOINABLE 1

/* ------------------------------------------- C types ------------------------------------------ */

struct pthread_t;
typedef struct pthread_t pthread_t;
typedef u32              pthread_attr_t;

/* ------------------------------------ C function prototypes ----------------------------------- */

int        pthread_create(pthread_t*, const pthread_attr_t*, void* (*)(void*), void*);
int        pthread_attr_init(pthread_attr_t*);
int        pthread_attr_destroy(pthread_attr_t*);
int        pthread_set_detachstate(pthread_attr_t*, int);
int        pthread_join(pthread_t*, void**);
pthread_t* pthread_self(void);
int        pthread_kill(pthread_t*, int);
void       pthread_exit(void*);

#ifdef __cplusplus
}
#endif
