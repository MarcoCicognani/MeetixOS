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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(PTHREAD_CREATE_DETACHED, int, 0);
CONST_VALUE(PTHREAD_CREATE_JOINABLE, int, 1);

TYPE_ALIAS(pthread_t, struct pthread_t);
TYPE_ALIAS(pthread_attr_t, uint32_t);

int pthread_create(pthread_t*, const pthread_attr_t*, void* (*)(void*), void*);
int pthread_attr_init(pthread_attr_t*);
int pthread_attr_destroy(pthread_attr_t*);
int pthread_set_detachstate(pthread_attr_t*, int);
int pthread_join(pthread_t*, void**);

pthread_t* pthread_self(void);
int        pthread_kill(pthread_t*, int);
void       pthread_exit(void*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
