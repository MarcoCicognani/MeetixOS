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
#include <LibRT/New.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/Meta.hh>

auto operator new(__SIZE_TYPE__) -> void* {
    verify_not_reached$();
}

auto operator new(__SIZE_TYPE__, align_val_t) -> void* {
    verify_not_reached$();
}

auto operator new[](__SIZE_TYPE__) -> void* {
    verify_not_reached$();
}

void operator delete(void*) noexcept {
    verify_not_reached$();
}

void operator delete(void*, align_val_t) noexcept {
    verify_not_reached$();
}

void operator delete[](void*) noexcept {
    verify_not_reached$();
}

auto operator new(__SIZE_TYPE__ size, nothrow_t const&) noexcept -> void* {
    auto error_or_ptr = Heap::alloc(size, Heap::Clean::No);
   if ( error_or_ptr.is_value() )
       return error_or_ptr.unwrap();
   else
       return nullptr;
}

auto operator new(__SIZE_TYPE__ size, align_val_t align, nothrow_t const&) noexcept -> void* {
   auto error_or_ptr = Heap::alloc_aligned(size, static_cast<UnderlyingType<align_val_t>>(align), Heap::Clean::No);
   if ( error_or_ptr.is_value() )
       return error_or_ptr.unwrap();
   else
       return nullptr;
}

auto operator new[](__SIZE_TYPE__ size, nothrow_t const&) noexcept -> void* {
   auto error_or_ptr  = Heap::alloc(size, Heap::Clean::No);
   if ( error_or_ptr.is_value() )
       return error_or_ptr.unwrap();
   else
       return nullptr;
}

void operator delete(void* ptr, __SIZE_TYPE__ size) noexcept {
    Heap::dealloc(ptr, size);
}

void operator delete(void* ptr, __SIZE_TYPE__ size, align_val_t align) noexcept {
    Heap::dealloc_aligned(ptr, size, static_cast<UnderlyingType<align_val_t>>(align));
}

void operator delete[](void* ptr, __SIZE_TYPE__ size) noexcept {
    Heap::dealloc(ptr, size);
}
