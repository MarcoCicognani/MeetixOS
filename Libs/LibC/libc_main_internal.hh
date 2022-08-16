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

#include <LibTC/Functional/ErrorOr.hh>

#ifdef __cplusplus
extern "C" {
#endif

int main(int, char**);

void libc_main();
void libc_init();
void libc_fini();

#ifdef __cplusplus
}
#endif

auto parse_cli_args(i32&, char**&) -> ErrorOr<void>;