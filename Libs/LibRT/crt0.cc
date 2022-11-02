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

namespace RT {

[[noreturn]]
auto run() -> void;

} /* namespace RT */

extern "C" {

[[noreturn]]
void _start() {
    RT::run();
}

} /* extern "C" */
