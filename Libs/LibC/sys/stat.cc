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

#include <LibApi/Api.h>
#include <LibC/sys/stat.h>

extern "C" {

int stat(const char*, struct stat*) {
    __NOT_IMPLEMENTED(stat);
    return -1;
}

int fstat(int, struct stat*) {
    __NOT_IMPLEMENTED(fstat);
    return -1;
}

int lstat(const char*, struct stat*) {
    __NOT_IMPLEMENTED(lstat);
    return -1;
}

int chmod(const char*, mode_t) {
    __NOT_IMPLEMENTED(chmod);
    return -1;
}

int fchmod(int, mode_t) {
    __NOT_IMPLEMENTED(fchmod);
    return -1;
}

mode_t umask(mode_t) {
    __NOT_IMPLEMENTED(umask);
    return -1;
}

int mkdir(const char*, mode_t) {
    __NOT_IMPLEMENTED(mkdir);
    return -1;
}
}