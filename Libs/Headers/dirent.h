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
#include <dir.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define DT_UNKNOWN 0 /* Entry type unknown */
#define DT_REG     1 /* Entry type regular file */
#define DT_DIR     2 /* Entry type directory */
#define DT_FIFO    3 /* Entry type pipe */
#define DT_SOCK    4 /* Entry type socket */
#define DT_CHR     5 /* Entry type character device */
#define DT_BLK     6 /* Entry type block device */
#define DT_LNK     7 /* Entry type symlink */

/* ------------------------------------------- C types ------------------------------------------ */

typedef struct dirent {
    ino_t d_fileno;             /* inode of the entry */
    usize d_reclen;             /* size of the entry */
    usize d_namlen;             /* name length of the entry */
    dev_t d_dev;                /* device identifier */
    u8    d_type;               /* node type (file/directory) */
    char  d_name[FILENAME_MAX]; /* entry name */
} dirent;

/* ------------------------------------ C function prototypes ----------------------------------- */

DIR*    opendir(const char*);
dirent* readdir(DIR*);
int     closedir(DIR*);
void    rewinddir(DIR*);

#ifdef __cplusplus
}
#endif
