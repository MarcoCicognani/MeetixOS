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

#include <LibRT/Heap.hh>

auto Heap::rt_alloc(usize, CleanMem) -> ErrorOr<void*> {
    return Error::from_literal("Not Yet Implemented"sv);
}

auto Heap::rt_realloc(void*, usize) -> ErrorOr<void*> {
    return Error::from_literal("Not Yet Implemented"sv);
}

auto Heap::rt_dealloc(void*, usize) -> void {
}

auto Heap::rt_alloc_aligned(usize, usize, CleanMem) -> ErrorOr<void*> {
    return Error::from_literal("Not Yet Implemented"sv);
}

auto Heap::rt_dealloc_aligned(void*, usize, usize) -> void {
}
