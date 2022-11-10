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

#include <ST/Core/ErrorOr.hh>
#include <ST/Lang/IntTypes.hh>

namespace Heap {

enum class Clean : bool {
    Yes,
    No
};

auto alloc(usize size, Clean) -> ErrorOr<void*>;
auto realloc(void* ptr, usize new_size) -> ErrorOr<void*>;
auto dealloc(void* ptr, usize size) -> void;

auto alloc_aligned(usize size, usize alignment, Clean) -> ErrorOr<void*>;
auto dealloc_aligned(void* ptr, usize size, usize alignment) -> void;

} /* namespace Heap */
