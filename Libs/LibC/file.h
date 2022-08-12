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

#include <LibApi/Api/FileSystem.h>
#include <LibC/stdint.h>
#include <LibC/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------ C defines ----------------------------------------- */

#define FILE_UNGET_PRESERVED_SPACE 4 /* space preserved for ungetc-calls */

#define FILE_FLAG_EOF                    (1 << 26) /* end of file reached */
#define FILE_FLAG_ERROR                  (1 << 27) /* when a file error has occured */
#define FILE_FLAG_BUFFER_SET             (1 << 28) /* whether the buffer was ever set */
#define FILE_FLAG_BUFFER_DIRECTION_READ  (1 << 29) /* last access was read */
#define FILE_FLAG_BUFFER_DIRECTION_WRITE (1 << 30) /* last access was write */
#define FILE_FLAG_BUFFER_OWNER_LIBRARY   (1 << 31) /* buffer was created in setvbuf */

/* ------------------------------------------- C types ------------------------------------------ */

typedef struct FILE {
    FileHandle m_fd;
    bool       m_lock;
    u8*        m_io_buffer;
    usize      m_io_buffer_len;
    u8         m_buffer_mode;
    usize      m_buffered_bytes_write;
    usize      m_buffered_bytes_read;
    usize      m_buffered_bytes_read_offset;
    usize      m_flags;

    isize (*m_impl_read)(void*, usize, struct FILE*);
    isize (*m_impl_write)(const void*, usize, struct FILE*);
    int (*m_impl_seek)(struct FILE*, off_t, int);
    off_t (*m_impl_tell)(struct FILE*);
    int (*m_impl_close)(struct FILE*);
    struct FILE* (*m_impl_reopen)(const char*, const char*, struct FILE*);
    int (*m_impl_fileno)(struct FILE*);
    int (*m_impl_eof)(struct FILE*);
    int (*m_impl_error)(struct FILE*);
    void (*m_impl_clear_err)(struct FILE*);
    void (*m_impl_set_err)(struct FILE*);

    struct FILE* m_prev_stream;
    struct FILE* m_next_stream;
} FILE;

#ifdef __cplusplus
}
#endif
