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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"

extern "C" {

[[gnu::used]]
void __cxa_atexit(void (*)(void*), void*, void*) {
    /* TODO implement the cxa runtime into the LibAPI */
}

} /* extern "C" */

#pragma clang diagnostic pop