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

auto Heap::rt_dealloc(void*, usize) -> ErrorOr<void> {
    return Error::from_literal("Not Yet Implemented"sv);
}

auto Heap::rt_alloc_aligned(usize, usize, CleanMem) -> ErrorOr<void*> {
    return Error::from_literal("Not Yet Implemented"sv);
}

auto Heap::rt_dealloc_aligned(void*, usize, usize) -> ErrorOr<void> {
    return Error::from_literal("Not Yet Implemented"sv);
}

/* CCLang.Alloc.New plugin support */

auto __rt_heap_plugin_alloc(usize size, bool clean) -> ErrorOr<void*> {
    return Heap::rt_alloc(size, clean ? Heap::CleanMem::Yes : Heap::CleanMem::No);
}

auto __rt_heap_plugin_dealloc(void* ptr, usize size) -> ErrorOr<void> {
    return Heap::rt_dealloc(ptr, size);
}

auto __rt_heap_plugin_alloc_aligned(usize size, usize alignment, bool clean) -> ErrorOr<void*> {
    return Heap::rt_alloc_aligned(size, alignment, clean ? Heap::CleanMem::Yes : Heap::CleanMem::No);
}

auto __rt_heap_plugin_dealloc_aligned(void* ptr, usize size, usize alignment) -> ErrorOr<void> {
    return Heap::rt_dealloc_aligned(ptr, size, alignment);
}
