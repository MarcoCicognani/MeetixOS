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

#ifdef IN_KERNEL
#    include <memory/KernelHeap.hpp>
#else
#    include <malloc.h>
#endif

#include <TC/Assertion.hh>
#include <TC/Functional/ErrorOr.hh>

namespace TC::RawMemory {

#ifdef IN_KERNEL

template<typename T>
Functional::ErrorOr<T*> clean_alloc(usize object_count) {
    auto start_ptr = KernelHeap::allocate(sizeof(T) * object_count);
    if ( start_ptr != nullptr ) {
        __builtin_memset(start_ptr, sizeof(T), object_count);
        return reinterpret_cast<T*>(start_ptr);
    } else
        return ENOMEM;
}

template<typename T>
void free_sized(T* object_start, usize) {
    KernelHeap::free(reinterpret_cast<void*>(object_start));
}

#else

template<typename T>
Functional::ErrorOr<T*> clean_alloc(usize object_count) {
    auto start_ptr = reinterpret_cast<T*>(calloc(sizeof(T), object_count));
    if ( start_ptr != nullptr )
        return start_ptr;
    else
        return ENOMEM;
}

template<typename T>
void free_sized(T* object_start, usize) {
    free(reinterpret_cast<void*>(object_start));
}

#endif

} /* namespace TC::RawMemory */