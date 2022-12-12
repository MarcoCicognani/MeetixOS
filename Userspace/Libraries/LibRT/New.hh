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

/* clang-format off */

[[nodiscard]] auto operator new(__SIZE_TYPE__)              -> void* __attribute__((error("MeetixC++ deny the usage of throwable new operators")));
[[nodiscard]] auto operator new(__SIZE_TYPE__, align_val_t) -> void* __attribute__((error("MeetixC++ deny the usage of throwable new operators")));
[[nodiscard]] auto operator new[](__SIZE_TYPE__)            -> void* __attribute__((error("MeetixC++ deny the usage of throwable new operators")));

auto operator delete(void*)              noexcept -> void __attribute__((error("MeetixC++ deny the usage of no known size delete operator")));
auto operator delete(void*, align_val_t) noexcept -> void __attribute__((error("MeetixC++ deny the usage of no known size delete operator")));
auto operator delete[](void*)            noexcept -> void __attribute__((error("MeetixC++ deny the usage of no known size delete operator")));

/* clang-format on */

[[nodiscard]]
auto operator new(__SIZE_TYPE__, nothrow_t const&) noexcept -> void*;
[[nodiscard]]
auto operator new(__SIZE_TYPE__, align_val_t, nothrow_t const&) noexcept -> void*;
[[nodiscard]]
auto operator new[](__SIZE_TYPE__, nothrow_t const&) noexcept -> void*;

void operator delete(void*, __SIZE_TYPE__) noexcept;
void operator delete(void*, __SIZE_TYPE__, align_val_t) noexcept;

void operator delete[](void*, __SIZE_TYPE__) noexcept;