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
#include <LibRT/Heap.hh>
#endif

#include <LibTC/Forward.hh>

#include <LibTC/Alloc/AllocLayout.hh>
#include <LibTC/Core/ErrorOr.hh>
#include <LibTC/Lang/Cxx.hh>
#include <LibTC/Lang/Must.hh>

namespace Details {

#ifdef IN_KERNEL

template<typename T>
auto __heap_plug_alloc_mem(AllocLayout alloc_layout) -> ErrorOr<T*> {
    auto start_ptr = KernelHeap::allocate(alloc_layout.size());
    if ( start_ptr != nullptr ) {
        __builtin_memset(start_ptr, 0, alloc_layout.size());
        return Cxx::bit_cast<T*>(start_ptr);
    } else
        return Error::new_from_code(ENOMEM);
}

template<typename T>
auto __heap_plug_dealloc_mem(AllocLayout, T* object_ptr) {
    KernelHeap::free(Cxx::bit_cast<void*>(object_ptr));
}

#else

template<typename T>
auto __heap_plug_alloc_mem(AllocLayout alloc_layout) -> ErrorOr<T*> {
    return Heap::alloc(alloc_layout, Heap::Clean::Yes).template map<T*>([](auto ptr) -> T* { return Cxx::bit_cast<T*>(ptr); });
}

template<typename T>
auto __heap_plug_dealloc_mem(AllocLayout alloc_layout, T* object_ptr) {
    must$(Heap::dealloc(alloc_layout, object_ptr));
}

#endif

} /* namespace Details */
