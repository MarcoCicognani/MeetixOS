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

#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(S_IXOTH, mode_t, 0001);
CONST_VALUE(S_IWOTH, mode_t, 0002);
CONST_VALUE(S_IROTH, mode_t, 0003);
CONST_VALUE(S_IRWXO, mode_t, 0007);
CONST_VALUE(S_IXGRP, mode_t, 0010);
CONST_VALUE(S_IWGRP, mode_t, 0020);
CONST_VALUE(S_IRGRP, mode_t, 0040);
CONST_VALUE(S_IRWXG, mode_t, 0070);
CONST_VALUE(S_IXUSR, mode_t, 0100);
CONST_VALUE(S_IWUSR, mode_t, 0200);
CONST_VALUE(S_IRUSR, mode_t, 0400);
CONST_VALUE(S_IRWXU, mode_t, 0700);

#define S_ISREG(mode)  1
#define S_ISDIR(mode)  0
#define S_ISCHR(mode)  0
#define S_ISBLK(mode)  0
#define S_ISFIFO(mode) 0

struct stat {
    dev_t     st_dev;
    ino_t     st_ino;
    mode_t    st_mode;
    nlink_t   st_nlink;
    uid_t     st_uid;
    gid_t     st_gid;
    off_t     st_size;
    time_t    st_atime;
    time_t    st_mtime;
    time_t    st_ctime;
    blksize_t st_blksize;
    blkcnt_t  st_blocks;
};

int    stat(const char*, struct stat*);
int    fstat(int, struct stat*);
int    lstat(const char*, struct stat*);
int    chmod(const char*, mode_t);
int    fchmod(int, mode_t);
mode_t umask(mode_t);
int    mkdir(const char*, mode_t);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
