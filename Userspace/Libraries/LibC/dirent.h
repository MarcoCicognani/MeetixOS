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

#include <dir.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(DT_UNKNOWN, uint8_t, 0); /* Entry type unknown */
CONST_VALUE(DT_REG, uint8_t, 1);     /* Entry type regular file */
CONST_VALUE(DT_DIR, uint8_t, 2);     /* Entry type directory */
CONST_VALUE(DT_FIFO, uint8_t, 3);    /* Entry type pipe */
CONST_VALUE(DT_SOCK, uint8_t, 4);    /* Entry type socket */
CONST_VALUE(DT_CHR, uint8_t, 5);     /* Entry type character device */
CONST_VALUE(DT_BLK, uint8_t, 6);     /* Entry type block device */
CONST_VALUE(DT_LNK, uint8_t, 7);     /* Entry type symlink */

struct dirent {
    ino_t   d_fileno;             /* inode of the entry */
    size_t   d_reclen;             /* size of the entry */
    size_t   d_namlen;             /* name length of the entry */
    dev_t   d_dev;                /* device identifier */
    uint8_t d_type;               /* node type (file/directory) */
    char    d_name[FILENAME_MAX]; /* entry name */
};
TYPE_ALIAS(dirent, struct dirent);

DIR*    opendir(const char*);
dirent* readdir(DIR*);
int     closedir(DIR*);
void    rewinddir(DIR*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#pragma clang diagnostic pop
