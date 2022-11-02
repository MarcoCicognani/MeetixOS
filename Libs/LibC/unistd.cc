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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibApi/Api.h>
#include <LibC/errno.h>
#include <LibC/stdio.h>
#include <LibC/unistd.h>

extern "C" {

char** g_environ = { nullptr };

int chdir(const char*) {
    __NOT_IMPLEMENTED(chdir);
    return -1;
}

ssize_t read(int fd, void* buf, size_t count) {
    FsReadStatus read_status;

    auto const read_bytes = s_read_s(fd, buf, count, &read_status);
    if ( read_status == FS_READ_SUCCESSFUL )
        return static_cast<ssize_t>(read_bytes);
    else if ( read_status == FS_READ_INVALID_FD )
        errno = EBADF;
    else if ( read_status == FS_READ_BUSY )
        errno = EIO;
    else
        errno = EINVAL;
    return -1;
}

ssize_t write(int fd, const void* buf, size_t count) {
    FsWriteStatus write_status;

    auto const written_bytes = s_write_s(fd, buf, count, &write_status);
    if ( write_status == FS_WRITE_SUCCESSFUL )
        return static_cast<ssize_t>(written_bytes);
    else if ( write_status == FS_WRITE_INVALID_FD )
        errno = EBADF;
    else if ( write_status == FS_WRITE_BUSY )
        errno = EIO;
    else
        errno = EINVAL;
    return -1;
}

off_t lseek(int fd, off_t offset, int whence) {
    FsSeekMode mode{};
    if ( whence == SEEK_SET )
        mode = FS_SEEK_SET;
    else if ( whence == SEEK_CUR )
        mode = FS_SEEK_CUR;
    else if ( whence == SEEK_END )
        mode = FS_SEEK_END;

    /* perform the seek operation */
    FsSeekStatus seek_status;

    auto const new_offset = s_seek_s(fd, offset, mode, &seek_status);
    if ( seek_status == FS_SEEK_SUCCESSFUL )
        return new_offset;
    else if ( seek_status == FS_SEEK_INVALID_FD )
        errno = EBADF;
    else
        errno = EIO;
    return -1;
}

long int tell(int fd) {
    return static_cast<long int>(s_tell(fd));
}

int close(int fd) {
    auto const close_status = s_close(fd);
    if ( close_status == FS_CLOSE_SUCCESSFUL )
        return 0;
    else if ( close_status == FS_CLOSE_INVALID_FD )
        errno = EBADF;
    else
        errno = EIO;
    return -1;
}

void* sbrk(intptr_t increment) {
    if ( void* result; s_set_break(increment, &result) )
        return result;

    /* the kernel or the process have finished the memory */
    errno = ENOMEM;
    return (void*)-1;
}

unsigned sleep(unsigned seconds) {
    s_sleep(seconds * 1000);
    return seconds;
}

pid_t getpid() {
    return s_get_tid();
}

pid_t getppid() {
    return s_get_pid();
}

char* getcwd(char* buf, size_t size) {
    auto const work_dir_stat = s_get_working_directory_l(buf, size);
    if ( work_dir_stat == GET_WORKING_DIRECTORY_SUCCESSFUL )
        return buf;
    else if ( work_dir_stat == GET_WORKING_DIRECTORY_SIZE_EXCEEDED )
        errno = ERANGE;
    return nullptr;
}

int isatty(int) {
    __NOT_IMPLEMENTED(isatty);
    return -1;
}

int access(const char*, int) {
    __NOT_IMPLEMENTED(access);
    return -1;
}

int fcntl(int, int, ...) {
    __NOT_IMPLEMENTED(fcntl);
    return -1;
}

int rmdir(const char*) {
    __NOT_IMPLEMENTED(rmdir);
    return -1;
}

int symlink(const char*, const char*) {
    __NOT_IMPLEMENTED(symlink);
    return -1;
}

} /* extern "C" */

#pragma clang diagnostic pop
