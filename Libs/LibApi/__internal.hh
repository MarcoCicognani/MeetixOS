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

#pragma once

#include <Api.h>

__BEGIN_C

/**
 * @brief Called by the library thread entry-point
 */
void __exit_thread();

/**
 * The address of this function is inserted as the return address for signal & irq handlers.
 * It does nothing but calling the <s_restore_interrupted_state> function.
 */
void __restore_interrupted_state_callback();

/**
 * Common atomic interface used by atomic*ock calls
 *
 * @param atom:         the atom to use
 * @param atom2:        the second atom to use
 * @param set_on_finish:  tell to kernel to set atoms when operations finish
 * @param try_only:      tell to kernel to try atomic wait
 *
 * @security-level APPLICATION
 */
bool __atomic_lock(bool* atom1, bool* atom2, bool set_on_finish, bool try_only);

/**
 * copy source to destination to at least length bytes
 *
 * @param dest:      pointer to memory copy destination
 * @param src:           the source to copy on destination
 * @param len:           the size of the source
 * @return the destination pointer
 */
void* memory_copy(void* dest, const void* src, usize len);

/**
 * returns the length of the provided string
 *
 * @param str:      the string that we measure
 * @return the length of the string
 */
usize string_len(const char* str);

__END_C
