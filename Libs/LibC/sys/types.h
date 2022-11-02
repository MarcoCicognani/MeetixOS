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

#include <Api/FileSystem.h>
#include <Api/Kernel.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

TYPE_ALIAS(pid_t, Pid);
TYPE_ALIAS(off_t, int64_t);
TYPE_ALIAS(ino_t, FsVirtID);
TYPE_ALIAS(dev_t, FsVirtID);
TYPE_ALIAS(nlink_t, int);
TYPE_ALIAS(uid_t, int);
TYPE_ALIAS(gid_t, int);
TYPE_ALIAS(blksize_t, int);
TYPE_ALIAS(blkcnt_t, int);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
