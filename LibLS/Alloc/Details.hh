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

#include <LibLS/Forward.hh>

#include <LibLS/Core/ErrorOr.hh>

#ifdef IN_KERNEL
#    warning "Kernel implementation of new"
#else
#    include <LibRT/New.hh>
#endif

namespace Details {

auto __heap_plug_alloc_mem(usize size) -> ErrorOr<void*>;
auto __heap_plug_dealloc_mem(void* ptr, usize size) -> void;

} /* namespace Details */
