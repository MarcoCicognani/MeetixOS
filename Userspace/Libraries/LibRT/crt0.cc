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
#pragma ide diagnostic ignored "bugprone-reserved-identifier"

/* The runtime core is implemented into the crt0Impl.cc
 * In order to keep this object, once compiled, lightweight as possibile.
 * This code produces a call to an external symbol (__rt_run()) */
[[noreturn]]
auto __rt_run() -> void;

extern "C" {

void _start() {
    __rt_run();
}

} /* extern "C" */

#pragma clang diagnostic pop