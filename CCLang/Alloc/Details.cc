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

#include <CCLang/Alloc/Details.hh>

#ifdef IN_KERNEL

#    include <CCLang/Lang/Cxx.hh>
#    include <memory/KernelHeap.hpp>

auto Details::__heap_plug_alloc_mem(usize size) -> ErrorOr<void*> {
    auto start_ptr = KernelHeap::allocate(size.unwrap());
    if ( start_ptr != nullptr ) {
        Cxx::memset(start_ptr, 0, size);
        return start_ptr;
    } else
        return Error::from_code(ErrorCode::NoMemory);
}

auto Details::__heap_plug_dealloc_mem(void* ptr, usize) -> void {
    KernelHeap::free(ptr);
}

#else

#    include <LibRT/Heap.hh>

auto Details::__heap_plug_alloc_mem(usize size) -> ErrorOr<void*> {
    return Heap::alloc(size, Heap::Clean::Yes);
}

auto Details::__heap_plug_dealloc_mem(void* ptr, usize size) -> void {
    Heap::dealloc(ptr, size);
}

#endif
