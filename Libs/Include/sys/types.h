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

#include <Api.h>
#include <Api/Common.h>

__BEGIN_C

/* ------------------------------------------- C types ------------------------------------------ */

typedef Pid     pid_t;
typedef int64_t off_t;
typedef int64_t ssize_t;

typedef FsVirtID ino_t;
typedef FsVirtID dev_t;

typedef int nlink_t;
typedef int uid_t;
typedef int gid_t;
typedef int blksize_t;
typedef int blkcnt_t;

__END_C
