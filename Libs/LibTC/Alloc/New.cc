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

#include <LibTC/Alloc/Details.hh>
#include <LibTC/Alloc/New.hh>
#include <LibTC/Core/Assertions.hh>

auto operator new(usize) -> void* {
    VERIFY_NOT_REACHED();
}

auto operator new(usize, align_val_t) -> void* {
    VERIFY_NOT_REACHED();
}

auto operator new[](usize) -> void* {
    VERIFY_NOT_REACHED();
}

void operator delete(void*) noexcept {
    VERIFY_NOT_REACHED();
}

void operator delete(void*, align_val_t) noexcept {
    VERIFY_NOT_REACHED();
}

void operator delete[](void*) noexcept {
    VERIFY_NOT_REACHED();
}

auto operator new(usize size, nothrow_t const&) noexcept -> void* {
    auto const alloc_layout = AllocLayout::new_from_raw_parts(size, alignof(void*));
    auto const error_or_ptr = Details::__heap_plug_alloc_mem<void>(alloc_layout);
    if ( error_or_ptr.is_value() )
        return error_or_ptr.value();
    else
        return nullptr;
}

auto operator new(usize size, align_val_t align, nothrow_t const&) noexcept -> void* {
    auto const alloc_layout = AllocLayout::new_from_raw_parts(size, UnderlyingType<align_val_t>(align));
    auto const error_or_ptr = Details::__heap_plug_alloc_mem<void>(alloc_layout);
    if ( error_or_ptr.is_value() )
        return error_or_ptr.value();
    else
        return nullptr;
}

auto operator new[](usize size, nothrow_t const&) noexcept -> void* {
    auto const alloc_layout = AllocLayout::new_from_raw_parts(size, alignof(void*));
    auto const error_or_ptr = Details::__heap_plug_alloc_mem<void>(alloc_layout);
    if ( error_or_ptr.is_value() )
        return error_or_ptr.value();
    else
        return nullptr;
}

void operator delete(void* ptr, usize size) noexcept {
    auto const alloc_layout = AllocLayout::new_from_raw_parts(size, alignof(void*));
    Details::__heap_plug_dealloc_mem(alloc_layout, ptr);
}

void operator delete(void* ptr, usize size, align_val_t align) noexcept {
    auto const alloc_layout = AllocLayout::new_from_raw_parts(size, UnderlyingType<align_val_t>(align));
    Details::__heap_plug_dealloc_mem(alloc_layout, ptr);
}

void operator delete[](void* ptr, usize size) noexcept {
    auto const alloc_layout = AllocLayout::new_from_raw_parts(size, alignof(void*));
    Details::__heap_plug_dealloc_mem(alloc_layout, ptr);
}
