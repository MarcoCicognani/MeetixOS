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

#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define S_IXOTH 01
#define S_IWOTH 02
#define S_IROTH 03
#define S_IRWXO 07
#define S_IXGRP 010
#define S_IWGRP 020
#define S_IRGRP 040
#define S_IRWXG 070
#define S_IXUSR 0100
#define S_IWUSR 0200
#define S_IRUSR 0400
#define S_IRWXU 0700

#define S_ISREG(mode)  1
#define S_ISDIR(mode)  0
#define S_ISCHR(mode)  0
#define S_ISBLK(mode)  0
#define S_ISFIFO(mode) 0

/* ------------------------------------------- C types ------------------------------------------ */

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

/* ------------------------------------ C function prototypes ----------------------------------- */

int    stat(const char*, struct stat*);
int    fstat(int, struct stat*);
int    lstat(const char*, struct stat*);
int    chmod(const char*, mode_t);
int    fchmod(int, mode_t);
mode_t umask(mode_t);
int    mkdir(const char*, mode_t);

#ifdef __cplusplus
}
#endif
