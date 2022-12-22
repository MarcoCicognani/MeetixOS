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

auto operator new(__SIZE_TYPE__ size, nothrow_t const&) noexcept -> void*;
void operator delete(void* ptr, __SIZE_TYPE__ size) noexcept;

auto operator new(__SIZE_TYPE__ size, align_val_t, nothrow_t const&) noexcept -> void*;
void operator delete(void* ptr, __SIZE_TYPE__ size, align_val_t) noexcept;

auto operator new[](__SIZE_TYPE__ size, nothrow_t const&) noexcept -> void*;
void operator delete[](void* ptr, __SIZE_TYPE__ size) noexcept;

/* NOTE - The following operators are denied from the usage since they use the exceptions */

auto operator new(__SIZE_TYPE__) -> void* __attribute__((error("MeetixC++ deny the usage of throwable new operators")));
auto operator delete(void*) noexcept -> void __attribute__((error("MeetixC++ deny the usage of no known size delete operator")));

auto operator new(__SIZE_TYPE__, align_val_t) -> void* __attribute__((error("MeetixC++ deny the usage of throwable new operators")));
auto operator delete(void*, align_val_t) noexcept -> void __attribute__((error("MeetixC++ deny the usage of no known size delete operator")));

auto operator new[](__SIZE_TYPE__) -> void* __attribute__((error("MeetixC++ deny the usage of throwable new operators")));
auto operator delete[](void*) noexcept -> void __attribute__((error("MeetixC++ deny the usage of no known size delete operator")));
