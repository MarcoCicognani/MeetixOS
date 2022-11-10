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

#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>

#define DenyUse$(msg) __attribute__((error("Denied the use of " msg)));

/* clang-format off */

[[nodiscard]] auto operator new(usize size) -> void*              DenyUse$("throwable new operator");
[[nodiscard]] auto operator new(usize size, align_val_t) -> void* DenyUse$("throwable new operator");
[[nodiscard]] auto operator new[](usize size) -> void*            DenyUse$("throwable new operator");

void operator delete(void* ptr) noexcept              DenyUse$("no known size delete operator");
void operator delete(void* ptr, align_val_t) noexcept DenyUse$("no known size delete operator");
void operator delete[](void* ptrs) noexcept           DenyUse$("no known size delete operator");

/* clang-format on */

[[nodiscard]]
auto operator new(usize size, nothrow_t const&) noexcept -> void*;

[[nodiscard]]
auto operator new(usize size, align_val_t, nothrow_t const&) noexcept -> void*;

[[nodiscard]]
auto operator new[](usize size, nothrow_t const&) noexcept -> void*;

void operator delete(void* ptr, usize size) noexcept;
void operator delete(void* ptr, usize size, align_val_t) noexcept;

void operator delete[](void* ptr, usize size) noexcept;