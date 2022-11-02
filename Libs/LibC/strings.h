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

int strcasecmp(const char*, const char*);
int strncasecmp(const char*, const char*, size_t);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
