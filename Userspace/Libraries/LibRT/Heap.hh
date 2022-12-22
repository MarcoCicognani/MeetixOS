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

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/IntTypes.hh>

namespace Heap {

enum class CleanMem : bool {
    Yes,
    No
};

auto rt_alloc(usize size, CleanMem clean_mem) -> ErrorOr<void*>;
auto rt_realloc(void* ptr, usize new_size) -> ErrorOr<void*>;
auto rt_dealloc(void* ptr, usize size) -> void;

auto rt_alloc_aligned(usize size, usize alignment, CleanMem clean_mem) -> ErrorOr<void*>;
auto rt_dealloc_aligned(void* ptr, usize size, usize alignment) -> void;

} /* namespace Heap */
