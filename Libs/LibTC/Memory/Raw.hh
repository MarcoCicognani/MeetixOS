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

#ifdef IN_KERNEL
#    include <memory/KernelHeap.hpp>
#else
#    include <malloc.h>
#endif

#include <LibTC/BitCast.hh>
#include <LibTC/Functional/ErrorOr.hh>

namespace TC::Memory::Raw {

#ifdef IN_KERNEL

template<typename T>
constexpr ErrorOr<T*> clean_alloc_array(usize object_count) {
    auto start_ptr = KernelHeap::allocate(sizeof(T) * object_count);
    if ( start_ptr != nullptr ) {
        __builtin_memset(start_ptr, 0, object_count * sizeof(T));
        return bit_cast<T*>(start_ptr);
    } else
        return Error{ ENOMEM };
}

template<typename T>
constexpr ErrorOr<T*> clean_alloc_object(usize custom_size) {
    VERIFY_GREATER_EQUAL(custom_size, sizeof(T));

    auto object_ptr = KernelHeap::allocate(custom_size);
    if ( object_ptr != nullptr ) {
        __builtin_memset(object_ptr, 0, custom_size);
        return bit_cast<T*>(object_ptr);
    } else
        return Error{ ENOMEM };
}

template<typename T>
constexpr void free_sized(T* object_start, usize) {
    KernelHeap::free(bit_cast<void*>(object_start));
}

#else

template<typename T>
constexpr ErrorOr<T*> clean_alloc_array(usize object_count) {
    auto start_ptr = bit_cast<T*>(calloc(sizeof(T), object_count));
    if ( start_ptr != nullptr )
        return start_ptr;
    else
        return Error{ ENOMEM };
}

template<typename T>
constexpr ErrorOr<T*> clean_alloc_object(usize custom_size) {
    VERIFY_GREATER_EQUAL(custom_size, sizeof(T));

    auto object_ptr = bit_cast<T*>(calloc(custom_size, 1));
    if ( object_ptr != nullptr )
        return object_ptr;
    else
        return Error{ ENOMEM };
}

template<typename T>
constexpr void free_sized(T* object_start, usize) {
    free(bit_cast<void*>(object_start));
}

#endif

} /* namespace TC::Memory::Raw */
