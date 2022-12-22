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

#include <CCLang/Alloc/New.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Lang/Must.hh>

extern "C++" {

auto __rt_heap_plugin_alloc(usize size, bool clean) -> ErrorOr<void*>;
auto __rt_heap_plugin_dealloc(void* ptr, usize size) -> ErrorOr<void>;

auto __rt_heap_plugin_alloc_aligned(usize size, usize alignment, bool clean) -> ErrorOr<void*>;
auto __rt_heap_plugin_dealloc_aligned(void* ptr, usize size, usize alignment) -> ErrorOr<void>;

} /* extern "C++" */

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
    auto error_or_ptr = __rt_heap_plugin_alloc(size, true);
    if ( error_or_ptr.is_value() ) {
        return error_or_ptr.unwrap();
    } else {
        return nullptr;
    }
}

auto operator new(__SIZE_TYPE__ size, align_val_t align, nothrow_t const&) noexcept -> void* {
    auto error_or_ptr = __rt_heap_plugin_alloc_aligned(size, static_cast<UnderlyingType<align_val_t>>(align), true);
    if ( error_or_ptr.is_value() ) {
        return error_or_ptr.unwrap();
    } else {
        return nullptr;
    }
}

auto operator new[](__SIZE_TYPE__ size, nothrow_t const&) noexcept -> void* {
    auto error_or_ptr = __rt_heap_plugin_alloc(size, false);
    if ( error_or_ptr.is_value() ) {
        return error_or_ptr.unwrap();
    } else {
        return nullptr;
    }
}

void operator delete(void* ptr, __SIZE_TYPE__ size) noexcept {
    must$(__rt_heap_plugin_dealloc(ptr, size));
}

void operator delete(void* ptr, __SIZE_TYPE__ size, align_val_t align) noexcept {
    must$(__rt_heap_plugin_dealloc_aligned(ptr, size, static_cast<UnderlyingType<align_val_t>>(align)));
}

void operator delete[](void* ptr, __SIZE_TYPE__ size) noexcept {
    must$(__rt_heap_plugin_dealloc(ptr, size));
}

auto Details::internal_heap_alloc(usize size) -> ErrorOr<void*> {
    return __rt_heap_plugin_alloc(size, true);
}

auto Details::internal_heap_dealloc(void* ptr, usize size) -> void {
    must$(__rt_heap_plugin_dealloc(ptr, size));
}

