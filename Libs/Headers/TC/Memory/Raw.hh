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

#include <TC/Functional/ErrorOr.hh>

namespace TC::Memory::Raw {

#ifdef IN_KERNEL

template<typename T>
ErrorOr<T*> clean_alloc_array(usize object_count);

template<typename T>
ErrorOr<T*> clean_alloc_object(usize custom_size = sizeof(T));

template<typename T>
void free_sized(T* object_start, usize);

#else

template<typename T>
ErrorOr<T*> clean_alloc_array(usize object_count);

template<typename T>
ErrorOr<T*> clean_alloc_object(usize custom_size = sizeof(T));

template<typename T>
void free_sized(T* object_start, usize);

#endif

} /* namespace TC::Memory::Raw */

#include <../LibTC/Memory/Raw.hhi>