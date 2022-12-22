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

#include <CCLang/Forward.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/IntTypes.hh>

#ifdef IN_KERNEL
#    warning "Kernel implementation of new"
#else
#    include <LibRT/New.hh>
#endif

namespace Details {

auto __rt_heap_plugin_alloc_mem(usize size) -> ErrorOr<void*>;
auto __rt_heap_plugin_dealloc_mem(void* ptr, usize size) -> void;

} /* namespace Details */
