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
#    include <LibC/malloc.h> /* TODO use import LibRT.Heap */
#endif

#include <LibTC/Forward.hh>

#include <LibTC/Core/ErrorOr.hh>
#include <LibTC/Lang/Cxx.hh>

namespace Details {

#ifdef IN_KERNEL

template<typename T>
constexpr auto heap_alloc_clean_array(usize object_count) -> ErrorOr<T*> {
    auto start_ptr = KernelHeap::allocate(sizeof(T) * object_count);
    if ( start_ptr != nullptr ) {
        __builtin_memset(start_ptr, 0, object_count * sizeof(T));
        return Cxx::bit_cast<T*>(start_ptr);
    } else
        return Error::construct_from_code(ENOMEM);
}

template<typename T>
constexpr auto heap_alloc_clean_object(usize custom_size) -> ErrorOr<T*> {
    VERIFY_GREATER_EQUAL(custom_size, sizeof(T));

    auto object_ptr = KernelHeap::allocate(custom_size);
    if ( object_ptr != nullptr ) {
        __builtin_memset(object_ptr, 0, custom_size);
        return Cxx::bit_cast<T*>(object_ptr);
    } else
        return Error::construct_from_code(ENOMEM);
}

template<typename T>
constexpr auto heap_free_with_size(T* object_start, usize) -> void {
    KernelHeap::free(Cxx::bit_cast<void*>(object_start));
}

#else

template<typename T>
constexpr auto heap_alloc_clean_array(usize object_count) -> ErrorOr<T*> {
    // return try$(heap_alloc<T>(AllocLayout::construct_for_array_of<T>(object_count), ZeroMemory::Yes));

    auto start_ptr = Cxx::bit_cast<T*>(calloc(sizeof(T), object_count));
    if ( start_ptr != nullptr )
        return start_ptr;
    else
        return Error::construct_from_code(ENOMEM);
}

template<typename T>
constexpr auto heap_alloc_clean_object(usize custom_size) -> ErrorOr<T*> {
    // return try$(heap_alloc<T>(AllocLayout::construct_for_type<T>(), ZeroMemory::Yes));

    auto object_ptr = Cxx::bit_cast<T*>(calloc(custom_size, 1));
    if ( object_ptr != nullptr )
        return object_ptr;
    else
        return Error::construct_from_code(ENOMEM);
}

template<typename T>
constexpr auto heap_free_with_size(T* object_start, usize) -> void {
    free(Cxx::bit_cast<void*>(object_start));
}

#endif

} /* namespace Details */
