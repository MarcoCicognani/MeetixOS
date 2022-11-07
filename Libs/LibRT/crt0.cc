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

[[noreturn]]
auto rt_run() -> void;

extern "C" {

[[noreturn]]
void _start() {
    rt_run();
}

} /* extern "C" */
