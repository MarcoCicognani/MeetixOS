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
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

extern "C" {

int main(int, char const* const*);

} /* extern "C" */

#pragma clang diagnostic pop

auto libc_main() -> void;
auto libc_init() -> void;
auto libc_fini() -> void;
