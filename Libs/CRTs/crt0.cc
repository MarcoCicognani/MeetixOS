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
#include <LibC/libc_main_internal.hh>

/**
 * @brief User space programs entry point
 */
extern "C" auto _start() -> void {
    libc_main();
    s_exit(-1);
}
