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

auto Heap::alloc(usize, Clean) -> ErrorOr<void*> {
    return Error::new_from_literal("Not Yet Implemented"sv);
}

auto Heap::realloc(void*, usize) -> ErrorOr<void*> {
    return Error::new_from_literal("Not Yet Implemented"sv);
}

auto Heap::dealloc(void*, usize) -> void {
}

auto Heap::alloc_aligned(usize, usize, Clean) -> ErrorOr<void*> {
    return Error::new_from_literal("Not Yet Implemented"sv);
}

auto Heap::dealloc_aligned(void*, usize, usize) -> void {
}
